{build_functions, lib, pkgs, support, catch, TableFormat, NumberRanges, object_ptr}:

let

  callPackage = pkgs.lib.callPackageWith (pkgs // self);

  self = rec {
    inherit build_functions;
    inherit support;
    inherit TableFormat;
    inherit NumberRanges;
    inherit object_ptr;
    inherit catch;
    resolve_gcc_pkg = gccname: pkgs."${gccname}";

    FunctionalSources = callPackage ./FunctionalSources {};
    OperationTree = callPackage ./OperationTree {};

  };

in

  self
