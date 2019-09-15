{lib, pkgs}:

rec {
  resolve_gcc_pkg = gccname: pkgs."${gccname}";

  build_function = package_name: source_root: gcc_version: source_files: include_paths:
    pkgs.stdenv.mkDerivation  {
      inherit package_name;
      inherit source_root;
      inherit gcc_version;
      inherit source_files;
      name = "${package_name}-${gcc_version}";
      system = builtins.currentSystem;
      gcc_version_pkg = resolve_gcc_pkg gcc_version;
      source_file_paths = map (source_file: "${source_root}/${source_file}") source_files ;
      include_flags = map ( include_path: "-I${include_path}" ) include_paths ;
      buildCommand = "$gcc_version_pkg/bin/g++ $include_flags --std=c++11 $source_file_paths -o $out";
    };

  build_functions = args @ {include_paths ? [], ...}:
    lib.attrsets.genAttrs args.gcc_versions (gcc_version: build_function args.package_name args.source_root gcc_version args.source_files include_paths);

}
