{build_functions, catch}:

build_functions {
  inherit catch;
  package_name = "Parity";
  source_root = ./.;
  source_files = [
    "Main.cpp"
  ];
  gcc_versions = [
    "gcc48"
    "gcc8"
  ];
  include_paths = [
    catch
  ];
}
