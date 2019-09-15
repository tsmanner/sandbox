let

  pkgs = import <nixpkgs> {};
  callPackage = pkgs.lib.callPackageWith (pkgs // self);

  self = rec {
    support = callPackage ./support {};
    cpp = callPackage ./cpp {};
  };

in

  self
