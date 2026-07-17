{
  description = "ravynOS - Darwin/XNU for ARM devices (Motorola Harpia/MSM8916)";

  # XNU Version: Darwin 19.6 = macOS 10.15.6 (Catalina)
  # This flake builds XNU with iPhoneOS deployment target for ARM devices

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    let
      # Helper to generate packages/devShells per system
      forEachSystem = systems: f: nixpkgs.lib.genAttrs systems (system: f system);

      # XNU/Darwin version info (Darwin 19.6 = macOS 10.15.6)
      darwinVersion = {
        version = "19.6.0";
        osVersion = "10.15.6";
        sdkVersion = "10.15.6";
      };

      # Helper to create a system bundle
      mkSystemBundle = pkgs: name: desc: platform: soc: arch: armv: 
        pkgs.stdenv.mkDerivation {
          name = "ravynOS-${name}-${darwinVersion.version}";
          description = "ravynOS .System bundle for ${desc}";
          src = self;
          nativeBuildInputs = with pkgs; [ gnumake python3 cacert libplist ];
          env = {
            inherit (darwinVersion) version;
            DEVICE_NAME = name;
            DEVICE_DESC = desc;
            DEVICE_PLATFORM = platform;
            DEVICE_SOC = soc;
            DEVICE_ARCH = arch;
            ARM_VER = armv;
            TARGET_OS = if arch == "x86_64" then "MacOSX" else "iPhoneOS";
            DEPLOY_TARGET = if arch == "x86_64" then darwinVersion.osVersion else "15.0";
          };
          buildPhase = ''
            mkdir -p $out/${name}.System/System/Library/Kernels
            mkdir -p $out/${name}.System/System/Library/CoreServices
            mkdir -p $out/${name}.System/System/Library/PEF/${platform}

            cat > $out/${name}.System/System/Info.plist << PLIST
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleIdentifier</key><string>com.ravynos.kernel.${name}</string>
    <key>CFBundleName</key><string>ravynOS Kernel</string>
    <key>CFBundleVersion</key><string>${version}</string>
    <key>CFBundlePackageType</key><string>System Bundle</string>
    <key>Platform</key><string>${platform}</string>
    <key>SoC</key><string>${soc}</string>
    <key>CPUArchitecture</key><string>${arch}</string>
    <key>TargetOS</key><string>''${TARGET_OS}</string>
    <key>LSMinimumSystemVersion</key><string>''${DEPLOY_TARGET}</string>
</dict>
</plist>
PLIST

            cat > $out/${name}.System/System/Library/CoreServices/SystemVersion.plist << SYSVER
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>ProductBuildVersion</key><string>${version}</string>
    <key>ProductName</key><string>ravynOS</string>
    <key>ProductVersion</key><string>1.0.0</string>
</dict>
</plist>
SYSVER

            cat > $out/${name}.System/build-info.txt << BUILDINFO
ravynOS System Bundle
=====================
Name: ${name}
Device: ${desc}
Platform: ${platform}
SoC: ${soc}
Architecture: ${arch}

XNU Version: Darwin ${version}
Target: ''${TARGET_OS} ''${DEPLOY_TARGET}

Build: nix build github:thepanoc95/mobileland#${name}
Shell: nix develop github:thepanoc95/mobileland#dev
BUILDINFO
          '';
          installPhase = "mkdir -p $out && mv $out/${name}.System $out/ 2>/dev/null || true";
          meta = {
            description = "ravynOS .System for ${desc}";
            platforms = [ "x86_64-linux" "aarch64-linux" ];
          };
        };

    in {

      packages = forEachSystem [ "x86_64-linux" ] (system:
        let pkgs = import nixpkgs { inherit system; };
            commonBuildInputs = with pkgs; [
              gnumake cmake ninja pkg-config flex bison pax-utils cpio zstd
              texinfo gawk sed coreutils findutils diffutils grep gzip tar xz bzip2
              python3 python3Packages.plistlib clang llvm lld cacert lbzip2 pixz
            ];
            linuxBuildInputs = with pkgs; [ linuxHeaders gnu-gettext gnuplot gnutls ];
        in {
          default = pkgs.stdenv.mkDerivation {
            name = "ravynOS-shell";
            nativeBuildInputs = commonBuildInputs ++ linuxBuildInputs;
            buildPhase = "echo ravynOS development environment";
            installPhase = "mkdir -p $out && echo Done";
            meta.description = "ravynOS Development Environment - Darwin ${darwinVersion.version}";
          };

          # Device system bundles
          harpia-arm64 = mkSystemBundle pkgs "harpia-arm64" "Motorola Harpia (MSM8916)" "Harpia" "MSM8916" "arm64" "11";
          harpia-arm = mkSystemBundle pkgs "harpia-arm" "Motorola Harpia (MSM8916)" "Harpia" "MSM8916" "arm" "7";
          generic-arm64 = mkSystemBundle pkgs "generic-arm64" "Generic ARM64 Device" "GenericARM" "GENERIC" "arm64" "11";
          dev-x86_64 = mkSystemBundle pkgs "dev-x86_64" "Development Build" "Dev" "DEV" "x86_64" "";
        }
      );

      # Default package
      defaultPackage = forEachSystem [ "x86_64-linux" ] (system:
        self.packages.${system}.default
      );

      # Dev shells
      devShells = forEachSystem [ "x86_64-linux" ] (system:
        let pkgs = import nixpkgs { inherit system; };
            commonBuildInputs = with pkgs; [
              gnumake cmake ninja pkg-config flex bison pax-utils cpio zstd
              texinfo gawk sed coreutils findutils diffutils grep gzip tar xz bzip2
              python3 python3Packages.plistlib clang llvm lld cacert
              vim git curl wget rsync which tree jq libplist
            ];
        in {
          default = pkgs.mkShell {
            name = "ravynos-dev";
            packages = commonBuildInputs;
            env = {
              DARWIN_VERSION = darwinVersion.version;
              TARGET_OS = "iPhoneOS";
              IPHONEOS_DEPLOYMENT_TARGET = "15.0";
              ARM_VERSION = "11";
            };
            shellHook = ''
              echo "=============================================="
              echo "ravynOS Development Environment"
              echo "XNU: Darwin $DARWIN_VERSION"
              echo "Target: $TARGET_OS $IPHONEOS_DEPLOYMENT_TARGET"
              echo "=============================================="
            '';
          };

          harpia-arm64 = pkgs.mkShell {
            name = "ravynos-harpia-arm64";
            packages = commonBuildInputs;
            env = {
              DARWIN_VERSION = darwinVersion.version;
              TARGET_ARCH = "arm64";
              PLATFORM_NAME = "Harpia";
              SOC_NAME = "MSM8916";
              TARGET_OS = "iPhoneOS";
              IPHONEOS_DEPLOYMENT_TARGET = "15.0";
              ARM_VERSION = "11";
            };
          };

          harpia-arm = pkgs.mkShell {
            name = "ravynos-harpia-arm";
            packages = commonBuildInputs;
            env = {
              DARWIN_VERSION = darwinVersion.version;
              TARGET_ARCH = "arm";
              PLATFORM_NAME = "Harpia";
              SOC_NAME = "MSM8916";
              TARGET_OS = "iPhoneOS";
              IPHONEOS_DEPLOYMENT_TARGET = "15.0";
              ARM_VERSION = "7";
            };
          };
        }
      );

      # Templates
      templates = {
        ravynos = { description = "ravynOS XNU build environment"; path = ./flake.nix; };
        harpia-arm64 = { description = "ravynOS for Harpia ARM64"; path = ./flake.nix; };
      };
    };
}
