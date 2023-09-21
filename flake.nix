{
  description = "SCEPCAL";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/22.11";
    utils.url = "github:numtide/flake-utils";
    hepnix.url = "github:wonyongc/hepnix";
    hepnix.inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = { self, nixpkgs, utils, hepnix, ... }@inputs:

    inputs.utils.lib.eachSystem [ "x86_64-linux" "aarch64-darwin" ]

    (system: let
      pkgname = "SCEPCAL";

      overlays = hepnix.overlays;

      pkgs = import nixpkgs {
        inherit system overlays;
        config.allowUnfree = true;
      };

      overlaid = ps: with ps; [
        podio
        edm4hep
        VecCore
        VecGeom
        vdt
        SIO
        LCIO
        geant4
        dd4hep
        evtgen
        gaudi
        K4FWCore
        k4SimGeant4
        k4Gen
      ];

      in {

        packages.default = pkgs.stdenv.mkDerivation rec {
          pname = pkgname;
          version = "flake-test";

          src = self;

          buildInputs = hepnix.core pkgs ++ overlaid pkgs;

          nativeBuildInputs = hepnix.wrappers pkgs;
        };
      }
    );
}