#ifndef SCEPCALSiPMSD_h
#define SCEPCALSiPMSD_h 1

#include "SiPMHit.h"
#include "SCEPCALSegmentation.h"
#include "DD4hep/Segmentations.h"

#include "G4VSensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"

namespace drc {
    class SCEPCALSiPMSD : public G4VSensitiveDetector {
    public:
        SCEPCALSiPMSD(const std::string aName, const std::string aReadoutName, const dd4hep::Segmentation& aSeg);
        ~SCEPCALSiPMSD();

        virtual void Initialize(G4HCofThisEvent* HCE) final;
        virtual bool ProcessHits(G4Step* aStep, G4TouchableHistory*) final;

    private:
        SiPMHitsCollection* fHitCollection;
        dd4hep::DDSegmentation::SCEPCALSegmentation* fSeg;
        G4int fHCID;

        G4int fWavBin;
        G4int fTimeBin;
        G4float fWavlenStart;
        G4float fWavlenEnd;
        G4float fTimeStart;
        G4float fTimeEnd;
        G4float fWavlenStep;
        G4float fTimeStep;

        G4double wavToE(G4double wav) { return h_Planck*c_light/wav; }

        float findWavCenter(G4double en);
        float findTimeCenter(G4double stepTime);
    };
}

#endif
