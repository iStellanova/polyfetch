{
  inputs.nixpkgs.url = "github:nixOS/nixpkgs/nixpkgs-unstable";
  outputs = { nixpkgs, ... }:
  let
    system = "x86_64-linux";
    pkgs = import nixpkgs { inherit system; };
  in {
    devShells.${system}.default = pkgs.mkShell {
      packages = with pkgs; [
        gcc
        gdb
        gnumake
        clang-tools
        valgrind
        hyperfine
        sqlite
      ];
    };
  };
}
