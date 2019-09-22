{lib, pkgs, support}:

let

  callPackage = pkgs.lib.callPackageWith (pkgs // self);

  self = rec {
    inherit support;
    resolve_gcc_pkg = gccname: pkgs."${gccname}";

    build_function = package_name: source_root: gcc_version: source_files: include_paths: flags:
      pkgs.stdenv.mkDerivation  {
        inherit flags;
        inherit gcc_version;
        inherit package_name;
        inherit source_root;
        inherit source_files;
        name = "${package_name}-${gcc_version}";
        system = builtins.currentSystem;
        gcc_version_pkg = resolve_gcc_pkg gcc_version;
        source_file_paths = map (source_file: "${source_root}/${source_file}") source_files ;
        include_flags = map ( include_path: "-I${include_path}" ) include_paths ;
        buildCommand = "$gcc_version_pkg/bin/g++ $include_flags $flags $source_file_paths -o $out";
      };

    build_functions = args @ {include_paths ? [], ...}:
      lib.attrsets.genAttrs args.gcc_versions (
        gcc_version: build_function args.package_name args.source_root gcc_version args.source_files include_paths args.flags
      )
    ;


    catch = callPackage ./catch {};

    parity = callPackage ./Parity {};

    bits = callPackage ./BitManipulation {};

    # DistributionNetwork = callPackage ./DistributionNetwork {};

  };

in

  self
