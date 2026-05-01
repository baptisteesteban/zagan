{
  perSystem = {pkgs, ...}: {
    packages.default = pkgs.stdenv.mkDerivation {
      name = "zagan";
      version = "dev";
      src = ../.;
      nativeBuildInputs = with pkgs; [cmake ninja gcc];
      configurePhase = ''
        cmake --preset release --install-prefix $out
      '';
      buildPhase = ''
        cmake --build --preset release
      '';
      installPhase = ''
        mkdir -p $out
        cmake --install build/Release
      '';
    };
  };
}
