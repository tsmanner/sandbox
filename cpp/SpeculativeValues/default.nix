{build_functions, callPackage, catch, TableFormat, object_ptr, pkgs}:

let

  self = rec {
    # Installation of the NumberRanges library
    lib = pkgs.stdenv.mkDerivation rec {
      name = "SpeculativeValues";
      system = builtins.currentSystem;
      src = ./.;
      buildCommand = "mkdir $out && cp $src/*.h $out/";
    };

    # Compilation of the test executable
    test_compile = build_functions {
      inherit catch;
      inherit lib;
      package_name = "SpeculativeValuesTest";
      source_root = ./.;
      source_files = [
        "tests/test_main.cpp"
      ];
      gcc_versions = [
        "gcc48"
        "gcc8"
      ];
      include_paths = [
        lib
        catch
        object_ptr
        TableFormat
      ];
      flags = [
        "--std=c++11"
        "-Wall"
      ];
    };

    # Execution of the test executable
    test = {
      gcc48 = pkgs.stdenv.mkDerivation {
        name = "SpeculativeValuesTestRun-gcc48";
        system = builtins.currentSystem;
        buildCommand = "${test_compile.gcc48.out} | tee $out";
      };

      gcc8 = pkgs.stdenv.mkDerivation {
        name = "SpeculativeValuesTestRun-gcc8";
        system = builtins.currentSystem;
        buildCommand = "${test_compile.gcc8.out} | tee $out";
      };
    };
  };

in

  self
