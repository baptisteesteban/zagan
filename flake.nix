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

  outputs = {
    self,
    nixpkgs,
    ...
  }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {inherit system;};
    stdenv = pkgs.stdenv;
  in {
    packages.${system}.default = stdenv.mkDerivation {
      pname = "zagan";
      version = "dev";

      src = ./.;
      nativeBuildInputs = with pkgs; [cmake gnumake gcc ninja];

      configurePhase = ''
        cmake --preset release --install-prefix=$out
      '';
      buildPhase = ''
        cmake --build --preset release
      '';
      installPhase = ''
        mkdir $out
        cmake --install build/Release
      '';
    };

    devShell.${system} = pkgs.mkShell {
      packages = with pkgs; [clang-tools gtest];
      inputsFrom = with pkgs; [self.packages.${system}.default];
    };

    formatter.${system} = pkgs.alejandra;
  };
}
