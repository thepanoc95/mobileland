# Default entry point for nix develop
# This simply re-exports the main flake's devShell

(import ./flake.nix).devShells.x86_64-linux.default
