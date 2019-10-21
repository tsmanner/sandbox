{pkgs, support}:

pkgs.stdenv.mkDerivation rec {
    name = "TableFormat";
    system = builtins.currentSystem;
    buildCommand = "mkdir $out && cp $src $out/TableFormat.h";
    src = ./TableFormat.h;

}
