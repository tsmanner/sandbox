{pkgs}:

pkgs.stdenv.mkDerivation {
    name = "Catch";
    system = builtins.currentSystem;
    src = ./.;
    buildCommand = "mkdir $out && cp $src/catch.hpp $out/catch.hpp";
}
