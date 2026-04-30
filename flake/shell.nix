{
  perSystem = {pkgs, ...}: {
    devShells.default = pkgs.mkShell {
      packages = with pkgs; [
        clang-tools
        gtest
        cmake
        ninja
        gcc
      ];
    };
  };
}
