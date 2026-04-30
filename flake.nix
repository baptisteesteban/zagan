{
  description = "Baptiste Esteban personal Image Processing Library";

  inputs = {
    nixpkgs = {
      type = "github";
      repo = "nixpkgs";
      owner = "NixOS";
      ref = "nixos-25.11";
    };

    flake-parts = {
      type = "github";
      repo = "flake-parts";
      owner = "hercules-ci";
    };
  };

  outputs = {flake-parts, ...} @ inputs:
    flake-parts.lib.mkFlake {inherit inputs;} {
      imports = [
        ./flake
      ];
    };
}
