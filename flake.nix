{
  description = "C++26 + reflection dev environment (GCC 16, glaze v7.8.4)";

  inputs = {
    # gcc16 is only in nixpkgs-unstable right now (GCC 16.1 shipped Apr 2026)
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        # Build a stdenv that uses gcc16 as the compiler.
        gcc16Stdenv = pkgs.overrideCC pkgs.stdenv pkgs.gcc16;

        # Glaze 7.9.1
        glazeSrc = pkgs.fetchFromGitHub {
          owner = "stephenberry";
          repo = "glaze";
          rev = "cadcadea26554cc4214769e358f981426e40a02a"; # v7.9.1
          hash = "sha256-NRRq5MGF2f5PW0teYnq58ELzson+U6KHVPaY6r30KLA=";
        };

        # Header-only glaze package built with gcc16, C++26 + reflection.
        glaze = gcc16Stdenv.mkDerivation {
          pname = "glaze";
          version = "7.9.1";
          src = glazeSrc;

          nativeBuildInputs = [ pkgs.cmake ];

          # Glaze's own tests/examples aren't needed to install headers.
          cmakeFlags = [
            "-Dglaze_BUILD_EXAMPLES=OFF"
            "-Dglaze_DEVELOPER_MODE=OFF"
          ];

          env.NIX_CFLAGS_COMPILE = "-std=c++26 -freflection";
        };

        # Example library target that links against glaze with the
        # requested C++26 + reflection flags. Uses the CMakeLists.txt
        # shipped alongside this flake.
        moe = gcc16Stdenv.mkDerivation {
          pname = "moe";
          version = "0.1.0";
          src = ./.;

          nativeBuildInputs = [ pkgs.cmake ];
          buildInputs = [ glaze pkgs.eigen];
        };

      in {
        packages = {
          inherit glaze moe;
          default = moe;
        };

        devShells.default = gcc16Stdenv.mkDerivation {
          name = "cpp26-reflection-shell";

          nativeBuildInputs = [
            pkgs.cmake
            pkgs.ninja
            pkgs.gdb
          ];

          buildInputs = [ glaze pkgs.eigen ];
        };
      });
}
