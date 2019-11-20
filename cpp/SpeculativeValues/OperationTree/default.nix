{
  build_functions,
  callPackage,
  NumberRanges,
  object_ptr,
  catch,
  pkgs
}:

let

  self = rec {
    # Installation of the OperationTree library
    lib = pkgs.stdenv.mkDerivation rec {
      name = "SpecOperationTree";
      system = builtins.currentSystem;
      src = ./.;
      buildCommand = "mkdir $out && cp $src/*.h $out/";
    };

    # Compilation of the test executable
    test_compile = build_functions {
      # inherit lib;
      package_name = "SpecOperationTreeTest";
      source_root = ./.;
      source_files = [
        "test_main.cpp"
      ];
      gcc_versions = [
        "gcc48"
        "gcc8"
      ];
      include_paths = [
        lib
        NumberRanges.lib
        object_ptr
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
        name = "SpecOperationTreeTestRun-gcc48";
        system = builtins.currentSystem;
        buildCommand = "${test_compile.gcc48.out} | tee $out";
      };

      gcc8 = pkgs.stdenv.mkDerivation {
        name = "SpecOperationTreeTestRun-gcc8";
        system = builtins.currentSystem;
        buildCommand = "${test_compile.gcc8.out} | tee $out";
      };
    };
  };

in

  self
