{pkgs, support}:

pkgs.stdenv.mkDerivation rec {
    name = "object_ptr";
    system = builtins.currentSystem;
    buildCommand = "mkdir $out && cp $src $out/object_ptr.h";
    src = ./object_ptr.h;

}
