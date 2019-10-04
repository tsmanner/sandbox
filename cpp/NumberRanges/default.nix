{build_functions, callPackage, catch, pkgs}:

let

  self = rec {
    # Installation of the NumberRanges library
    lib = pkgs.stdenv.mkDerivation rec {
      name = "NumberRanges";
      system = builtins.currentSystem;
      src = ./.;
      buildCommand = "mkdir $out && cp $src/NumberRanges.h $out/NumberRanges.h";
    };

    # Compilation of the test executable
    test_compile = build_functions {
      inherit catch;
      inherit lib;
      package_name = "NumberRangesTest";
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
      ];
    };

    # Execution of the test executable
    test = {
      gcc48 = pkgs.stdenv.mkDerivation {
        name = "NumberRangesTestRun-gcc48";
        system = builtins.currentSystem;
        buildCommand = "${test_compile.gcc48.out} | tee $out";
      };

      gcc8 = pkgs.stdenv.mkDerivation {
        name = "NumberRangesTestRun-gcc8";
        system = builtins.currentSystem;
        buildCommand = "${test_compile.gcc8.out} | tee $out";
      };
    };
  };

in

  self
