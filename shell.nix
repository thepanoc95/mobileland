{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "ravynos-build";
  
  buildInputs = with pkgs; [
    gnumake
    cmake
    ninja
    pkg-config
    flex
    bison
    pax-utils
    cpio
    zstd
    texinfo
    gawk
    gnu-sed
    coreutils
    findutils
    diffutils
    grep
    gzip
    tar
    xz
    bzip2
    python3
    python3Packages.plistlib
    vim
    git
    curl
    wget
    rsync
    which
    tree
    jq
    libplist
  ];

  NIX_SSL_CERT_FILE = "${pkgs.cacert}/etc/ssl/certs/ca-bundle.crt";

  TARGET_OS = "iPhoneOS";
  IPHONEOS_DEPLOYMENT_TARGET = "15.0";
  ARM_VERSION = "11";

  DARWIN_VERSION = "19.6.0";
  MACOSX_VERSION = "10.15.6";

  shellHook = ''
    echo "=============================================="
    echo "ravynOS Build Environment"
    echo "XNU Version: Darwin 19.6 (macOS 10.15.6)"
    echo "Target: iPhoneOS"
    echo "=============================================="
    echo ""
    echo "This shell provides basic build tools."
    echo "For full ravynOS build, use: nix develop"
    echo ""
    echo "Environment variables set:"
    echo "  TARGET_OS=$TARGET_OS"
    echo "  IPHONEOS_DEPLOYMENT_TARGET=$IPHONEOS_DEPLOYMENT_TARGET"
    echo "  ARM_VERSION=$ARM_VERSION"
    echo "  DARWIN_VERSION=$DARWIN_VERSION"
    echo ""
    echo "Setup steps:"
    echo "1. Extract SDK to:"
    echo "   /workspace/project/build/Developer/Platforms/ravynOS.platform/Developer/SDKs/"
    echo ""
    echo "2. Extract toolchain to:"
    echo "   /workspace/project/build/Developer/Platforms/ravynOS.platform/Developer/Toolchains/"
    echo ""
    echo "3. Source environment:"
    echo "   nix build .#ravynos-shell"
    echo "   source result/setup-env.sh"
    echo "=============================================="
  '';
}
