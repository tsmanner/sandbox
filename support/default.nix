{fetchurl}:

let

  self = rec {

    fetchFromGitHubRelease = {org, repo, release, asset, sha256}:
      # sha256 is the hash of the file.  If the file changes, you see the new hash by running:
      #   $ nix-prefetch-url https://github.com/${org}/${repo}/releases/download/${release}/${asset}
      fetchurl {
        url = "https://github.com/${org}/${repo}/releases/download/${release}/${asset}";
        inherit sha256;
      }
    ;

  };

in

  self
