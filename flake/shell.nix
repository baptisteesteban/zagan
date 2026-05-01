{
  perSystem = {self', pkgs, ...}: {
    devShells.default = pkgs.mkShell {
      inputsFrom = [self'.packages.default];
      packages = with pkgs; [
        clang-tools
        gtest
      ];
    };
  };
}
