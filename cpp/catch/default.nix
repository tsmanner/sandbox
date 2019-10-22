{pkgs, support}:

pkgs.stdenv.mkDerivation rec {
    name = "catch";
    system = builtins.currentSystem;
    buildCommand = "mkdir $out && cp $src $out/catch.hpp";

    src = support.fetchFromGitHubRelease {
        org = "catchorg";
        repo = "Catch2";
        release = "v2.9.2";
        asset = "catch.hpp";
        sha256 = "1myzhpf8dwh88rji2dd6z9lc80q0gkyq27vgsp38swsg5li5zzfn";
    };
}
