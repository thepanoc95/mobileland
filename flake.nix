{
  description = "ravynOS - XNU for ARM devices (Motorola Harpia/MSM8916)";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    let
      forEachSystem = systems: f: nixpkgs.lib.genAttrs systems (system: f system);

      # ravynOS version info
      ravynOS = {
        darwinVersion = "19.6.0";
        sdkVersion = "10.15.6";
        deploymentTarget = "13.6.1";
      };

      # ravynOS SDK and toolchain from repository
      ravynSDK = "${self}/Developer/ravynOS.sdk";
      ravynToolchain = "${self}/Developer/Toolchains/Default.xctoolchain";

      # Create development shell for a specific target
      mkRavynOSEnv = { pkgs, name, arch, armVersion }:
        pkgs.mkShell {
          name = "ravynos-${name}";

          packages = with pkgs; [
            gnumake cmake ninja pkg-config
            clang_17 lld_17 llvm_17
            targetPackages.stdenv.cc.bintools-wrapper
            flex bison
            python3 libplist libxml2 zlib libpng libssl
            cpio rsync coreutils findutils diffutils grep gzip tar xz bzip2
            gawk sed texinfo git curl wget jq bc libbsd
          ];

          env = {
            DARWIN_VERSION = ravynOS.darwinVersion;
            SDK_VERSION = ravynOS.sdkVersion;
            IPHONEOS_DEPLOYMENT_TARGET = ravynOS.deploymentTarget;
            TARGET_ARCH = arch;
            ARM_VERSION = toString armVersion;
            SDKROOT = ravynSDK;
            TOOLCHAIN_PATH = ravynToolchain;
            TARGET_OS = if arch == "x86_64" then "MacOSX" else "iPhoneOS";
            CC = "${ravynToolchain}/usr/bin/clang";
            CXX = "${ravynToolchain}/usr/bin/clang++";
            LD = "${ravynToolchain}/usr/bin/ld";
            ARCH_FLAGS = if arch == "arm64" then
              "-target arm64-apple-ios${ravynOS.deploymentTarget}"
            else if arch == "arm" then
              "-target arm-apple-ios${ravynOS.deploymentTarget}"
            else "";
            ARCH_CONFIGS = if arch == "arm64" then "ARM64" else if arch == "arm" then "ARM" else "X86";
          };

          shellHook = ''
            echo "=============================================="
            echo "ravynOS Development Environment"
            echo "Architecture: ${arch} (ARM${toString armVersion})"
            echo "Target: $TARGET_OS $IPHONEOS_DEPLOYMENT_TARGET"
            echo "SDK: $SDKROOT"
            echo "=============================================="
            export PATH="${ravynToolchain}/usr/bin:$PATH"
          '';
        };
    in
    {
      packages = forEachSystem [ "x86_64-linux" "aarch64-linux" ] (system:
        let pkgs = import nixpkgs { inherit system; };
        in {
          default = self.packages.${system}.harpia-arm64;
          harpia-arm = mkRavynOSEnv { inherit pkgs; name = "harpia-arm"; arch = "arm"; armVersion = 7; };
          harpia-arm64 = mkRavynOSEnv { inherit pkgs; name = "harpia-arm64"; arch = "arm64"; armVersion = 11; };
          dev-x86_64 = mkRavynOSEnv { inherit pkgs; name = "dev-x86_64"; arch = "x86_64"; armVersion = 0; };
        }
      );

      devShells = forEachSystem [ "x86_64-linux" "aarch64-linux" ] (system:
        self.packages.${system}
      );

      templates = {
        ravynos = { description = "ravynOS XNU build environment"; path = ./flake.nix; };
        harpia-arm = { description = "ravynOS for Harpia (32-bit ARM)"; path = ./flake.nix; };
        harpia-arm64 = { description = "ravynOS for Harpia (64-bit ARM)"; path = ./flake.nix; };
      };
    };
}
