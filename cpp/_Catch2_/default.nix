{pkgs}:

pkgs.stdenv.mkDerivation {
    name = "Catch";
    system = builtins.currentSystem;
    buildCommand = "";
    src = ./.;
}
