{
  description = "SCEPCAL";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/22.11";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils, ... }@inputs:

    inputs.utils.lib.eachSystem [ "x86_64-linux" ]

    (system: let
      pkgname = "SCEPCAL";

      hepcore = import /home/wonyongc/src/hep/hepnix/hepcore.nix;

      overlays = hepcore.overlays;

      pkgs = import nixpkgs {
        inherit system overlays;
        config.allowUnfree = true;
      };

      in {

        packages.default = pkgs.stdenv.mkDerivation rec {
          pname = pkgname;
          version = "flake-test";

          src = self;

          buildInputs = hepcore.core pkgs ++ (with pkgs; [ AIDA podio edm4hep VecCore VecGeom vdt SIO LCIO mygeant4 dd4hep evtgen hepPDT gaudi k4FWCore k4SimGeant4 k4Gen ]);

          nativeBuildInputs = hepcore.wrappers pkgs;

          prePhases = [ "sourcePhase" ];

          sourcePhase = ''
          source ${pkgs.mygeant4}/bin/geant4.sh
          source ${pkgs.dd4hep}/bin/thisdd4hep.sh
          '';
        };
      }
    );
}