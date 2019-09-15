{cpp, cppParity, pkgs}:

{
  gcc48 = pkgs.stdenv.mkDerivation {
    name = "ParityTest-gcc48";
    system = builtins.currentSystem;
    buildCommand = "${cppParity.gcc48.out} --use-colour=yes | tee $out";
  };

  gcc8 = pkgs.stdenv.mkDerivation {
    name = "ParityTest-gcc8";
    system = builtins.currentSystem;
    buildCommand = "${cppParity.gcc8.out} --use-colour=yes | tee $out";
  };
}

