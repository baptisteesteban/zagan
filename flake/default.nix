{
  systems = ["x86_64-linux"];
  imports = [
    ./formatter.nix
    ./packages.nix
    ./shell.nix
  ];
}
