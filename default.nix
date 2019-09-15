let

  pkgs = import <nixpkgs> {};
  callPackage = pkgs.lib.callPackageWith (pkgs // self);

  self = rec {
    cpp = callPackage ./cpp {};

    cppCatch = callPackage ./cpp/_Catch2_ {};

    cppParity = callPackage ./cpp/Parity {};
    cppParityTest = callPackage ./cpp/Parity/tests.nix {};

    # cppDistributionNetwork = callPackage ./cpp/DistributionNetwork {};

  };

in

  self
