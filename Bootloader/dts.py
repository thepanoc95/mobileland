#!/usr/bin/env python3
"""
gen_apple_dt.py - Generate Apple-format Flattened Device Tree binaries.

Produces the proprietary binary DT format used by XNU's pexpert/device_tree.c.
This is NOT standard libfdt format.

Usage:
    python3 gen_apple_dt.py <input.json> [-o output.dtb] [--dump]

The JSON input describes the device tree structure. See harpia_dt.json for an
example.

Binary format (all values little-endian):

    DeviceTreeNode {
        uint32  nProperties     // 0 = sentinel (end of tree)
        uint32  nChildren
        // followed by nProperties x DeviceTreeNodeProperty
        // followed by nChildren  x DeviceTreeNode (recursive)
    }

    DeviceTreeNodeProperty {
        char    name[32]        // NUL-terminated, max 31 chars
        uint32  length          // byte length of value
        // followed by value[...] padded to 4-byte alignment
    }
"""

import argparse
import json
import struct
import sys
import os

PROP_NAME_MAX = 31
ALIGN = 4


def align4(n):
    return (n + ALIGN - 1) & ~(ALIGN - 1)


def encode_string(s, length):
    """Encode a string into a fixed-size NUL-terminated buffer."""
    b = s.encode("ascii")
    if len(b) > length:
        raise ValueError(f"String '{s}' exceeds {length} bytes")
    return b + b"\x00" * (length - len(b))


class DTBuilder:
    """Builds an Apple-format flattened device tree."""

    def __init__(self):
        self.nodes = []

    def add_node(self, name, properties=None, children=None):
        """Add a top-level node to the tree.

        properties: list of (name, value_bytes) tuples
        children:   list of child node dicts (recursive)
        """
        self.nodes.append({
            "name": name,
            "properties": properties or [],
            "children": children or [],
        })

    def _encode_prop(self, name, value):
        """Encode a single property: name[32] + length + value + padding."""
        if len(name) > PROP_NAME_MAX:
            raise ValueError(f"Property name '{name}' exceeds {PROP_NAME_MAX} chars")
        name_buf = encode_string(name, 32)
        val_len = len(value)
        padded_len = align4(val_len)
        pad = b"\x00" * (padded_len - val_len)
        return name_buf + struct.pack("<I", val_len) + value + pad

    def _encode_node(self, node):
        """Encode a node and all its descendants recursively."""
        # The "name" property is how XNU identifies child nodes (FindChild).
        # Every node must have a "name" property.
        all_props = [("name", node["name"].encode("ascii"))] + node["properties"]
        n_properties = len(all_props)
        n_children = len(node["children"])

        buf = struct.pack("<II", n_properties, n_children)
        for prop_name, prop_value in all_props:
            buf += self._encode_prop(prop_name, prop_value)
        for child in node["children"]:
            buf += self._encode_node(child)
        return buf

    def build(self):
        """Serialize the entire tree to bytes."""
        buf = b""
        for node in self.nodes:
            buf += self._encode_node(node)
        # Sentinel node (nProperties=0, nChildren=0)
        buf += struct.pack("<II", 0, 0)
        return buf


# ---------------------------------------------------------------------------
# JSON helpers
# ---------------------------------------------------------------------------

def parse_cells(val, cell_size=4):
    """Parse a list of hex/int values into big-endian bytes.

    cell_size: bytes per cell (4 for32-bit, 8 for 64-bit)
    """
    out = b""
    for v in val:
        if isinstance(v, str):
            v = int(v, 0)
        if cell_size == 4:
            out += struct.pack(">I", v & 0xFFFFFFFF)
        else:
            out += struct.pack(">Q", v & 0xFFFFFFFFFFFFFFFF)
    return out


def parse_value(prop_name, prop_def, addr_cells=2, size_cells=2):
    """Convert a JSON property definition to bytes.

    Handles:
      - "type": "string"   -> NUL-terminated ASCII
      - "type": "cells"    -> array of uint32 big-endian
      - "type": "longcells"-> array of uint64 big-endian
      - "type": "int"      -> single uint32 little-endian
      - "type": "empty"    -> zero bytes (presence flag)
      - "type": "phandle"  -> single uint32 big-endian
      - "type": "reg"      -> encoded using addr_cells/size_cells
      - raw string          -> NUL-terminated ASCII
      - raw int             -> uint32 LE
      - raw list of ints    -> treated as cells (uint32 BE)
    """
    if isinstance(prop_def, str):
        return prop_def.encode("ascii") + b"\x00"

    if isinstance(prop_def, int):
        return struct.pack("<I", prop_def)

    if isinstance(prop_def, list):
        # Check if it's a reg property (list of [base, size] pairs)
        if prop_name == "reg":
            out = b""
            for entry in prop_def:
                if isinstance(entry, list) and len(entry) == 2:
                    base, size = entry
                    if isinstance(base, str):
                        base = int(base, 0)
                    if isinstance(size, str):
                        size = int(size, 0)
                    out += parse_cells([base], addr_cells * 4)
                    out += parse_cells([size], size_cells * 4)
                else:
                    out += parse_cells(entry if isinstance(entry, list) else [entry])
            return out
        return parse_cells(prop_def)

    if isinstance(prop_def, dict):
        t = prop_def.get("type", "cells")
        v = prop_def.get("value", [])
        if t == "string":
            if isinstance(v, list):
                v = v[0] if v else ""
            return v.encode("ascii") + b"\x00"
        elif t == "cells":
            return parse_cells(v if isinstance(v, list) else [v], 4)
        elif t == "longcells":
            return parse_cells(v if isinstance(v, list) else [v], 8)
        elif t == "int":
            val = v if isinstance(v, int) else (v[0] if isinstance(v, list) else 0)
            return struct.pack("<I", val)
        elif t == "phandle":
            val = v if isinstance(v, int) else (v[0] if isinstance(v, list) else 0)
            return struct.pack(">I", val)
        elif t == "empty":
            return b""
        elif t == "reg":
            out = b""
            entries = v if isinstance(v, list) else [v]
            for entry in entries:
                if isinstance(entry, list) and len(entry) == 2:
                    base, size = entry
                    if isinstance(base, str):
                        base = int(base, 0)
                    if isinstance(size, str):
                        size = int(size, 0)
                    out += parse_cells([base], addr_cells * 4)
                    out += parse_cells([size], size_cells * 4)
                else:
                    out += parse_cells(entry if isinstance(entry, list) else [entry])
            return out
        elif t == "stringlist":
            out = b""
            for s in (v if isinstance(v, list) else [v]):
                out += s.encode("ascii") + b"\x00"
            return out

    raise ValueError(f"Cannot encode property '{prop_name}': {prop_def!r}")


def build_tree_from_json(spec, addr_cells=2, size_cells=2):
    """Convert a JSON node list into a DTBuilder tree."""
    builder = DTBuilder()

    for node_spec in spec:
        node = _build_node(node_spec, addr_cells, size_cells)
        builder.nodes.append(node)

    return builder


def _build_node(spec, addr_cells, size_cells):
    """Recursively build a node dict from JSON."""
    name = spec["name"]
    properties = []

    # Per-node address/size cell overrides
    local_addr_cells = spec.get("#address-cells", addr_cells)
    local_size_cells = spec.get("#size-cells", size_cells)

    # Skip meta keys (but #address-cells/#size-cells ARE encoded as properties)
    skip_keys = {"name", "children"}
    for key, val in spec.items():
        if key in skip_keys:
            continue
        value_bytes = parse_value(key, val, local_addr_cells, local_size_cells)
        properties.append((key, value_bytes))

    children = []
    for child_spec in spec.get("children", []):
        children.append(_build_node(child_spec, local_addr_cells, local_size_cells))

    return {"name": name, "properties": properties, "children": children}


def dump_tree(spec, indent=0):
    """Pretty-print the tree structure for debugging."""
    prefix = "  " * indent
    name = spec.get("name", "?")
    print(f"{prefix}{name}")
    skip_keys = {"name", "children"}
    for key, val in spec.items():
        if key in skip_keys:
            continue
        if isinstance(val, dict):
            t = val.get("type", "cells")
            v = val.get("value", [])
            print(f"{prefix}  {key} = [{t}] {v}")
        elif isinstance(val, str):
            print(f'{prefix}  {key} = "{val}"')
        elif isinstance(val, list):
            print(f"{prefix}  {key} = {val}")
        else:
            print(f"{prefix}  {key} = {val}")
    for child in spec.get("children", []):
        dump_tree(child, indent + 1)


def main():
    parser = argparse.ArgumentParser(
        description="Generate Apple-format flattened device tree binaries."
    )
    parser.add_argument("input", help="JSON input file describing the device tree")
    parser.add_argument("-o", "--output", default=None,
                        help="Output .dtb file (default: <input>.dtb)")
    parser.add_argument("--dump", action="store_true",
                        help="Dump tree structure to stdout (no binary output)")
    parser.add_argument("--addr-cells", type=int, default=2,
                        help="#address-cells at root (default: 2)")
    parser.add_argument("--size-cells", type=int, default=2,
                        help="#size-cells at root (default: 2)")
    args = parser.parse_args()

    with open(args.input, "r") as f:
        spec = json.load(f)

    if args.dump:
        for node in spec:
            dump_tree(node)
        return

    builder = build_tree_from_json(spec, args.addr_cells, args.size_cells)
    dtb = builder.build()

    out_path = args.output
    if out_path is None:
        out_path = os.path.splitext(args.input)[0] + ".dtb"

    with open(out_path, "wb") as f:
        f.write(dtb)

    print(f"Wrote {len(dtb)} bytes to {out_path}")


if __name__ == "__main__":
    main()
