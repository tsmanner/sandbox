{build_functions, callPackage, catch, TableFormat, object_ptr, pkgs}:

let

  self = rec {
    # Installation of the NumberRanges library

    # Compilation of the test executable
    test_compile = build_functions {
      package_name = "StringVStringView";
      source_root = ./.;
      source_files = [
        "Main.cpp"
      ];
      gcc_versions = [
        "gcc8"
      ];
      include_paths = [
      ];
      flags = [
        "--std=c++17"
        "-Wall"
      ];
    };

    # Execution of the test executable
    test = {
      gcc8 = pkgs.stdenv.mkDerivation {
        name = "SpeculativeValuesTestRun-gcc8";
        system = builtins.currentSystem;
        buildCommand = "${test_compile.gcc8.out} | tee $out";
      };
    };
  };

in

  self
