{
  description = "Baptiste Esteban personal Image Processing Library";

  inputs = {
    nixpkgs = {
      type = "github";
      repo = "nixpkgs";
      owner = "NixOS";
      ref = "nixos-25.11";
    };
  };

  outputs = {nixpkgs, ...}: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {inherit system;};
  in {
    devShell.${system} = pkgs.mkShell {
      buildInputs = with pkgs; [cmake ninja gcc clang-tools gtest];
    };

    formatter.${system} = pkgs.alejandra;
  };
}
