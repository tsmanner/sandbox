{build_functions, callPackage, catch, pkgs}:

let

  self = rec {
    # Installation of the BitManipulation library
    lib = pkgs.stdenv.mkDerivation rec {
      name = "BitManipulation";
      system = builtins.currentSystem;
      src = ./.;
      buildCommand = "mkdir $out && cp $src/*.h $out/";
    };

    # Compilation of the test executable
    test_compile = build_functions {
      inherit catch;
      inherit lib;
      package_name = "BitManipulationTest";
      source_root = ./.;
      source_files = [
        "Main.cpp"
      ];
      gcc_versions = [
        "gcc48"
        "gcc8"
      ];
      include_paths = [
        lib
        catch
      ];
      flags = [
        "--std=c++11"
        "-Wall"
      ];
    };

    # Execution of the test executable
    test = {
      gcc48 = pkgs.stdenv.mkDerivation {
        name = "BitManipulationTestRun-gcc48";
        system = builtins.currentSystem;
        buildCommand = "${test_compile.gcc48.out} | tee $out";
      };

      gcc8 = pkgs.stdenv.mkDerivation {
        name = "BitManipulationTestRun-gcc8";
        system = builtins.currentSystem;
        buildCommand = "${test_compile.gcc8.out} | tee $out";
      };
    };
  };

in

  self
