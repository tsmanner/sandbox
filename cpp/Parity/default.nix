{cpp, cppCatch}:

cpp.build_functions {
  inherit cppCatch;
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
    cppCatch.src
  ];
}
