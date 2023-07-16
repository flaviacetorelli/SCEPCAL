#include "SCEPCALSiPMSD.h"

#include "DD4hep/Detector.h"
#include "DDG4/Factories.h"

namespace dd4hep {
namespace sim {


  static G4VSensitiveDetector* create_SCEPCALSiPMSD(const std::string& aDetectorName, dd4hep::Detector& aLcdd) {
      std::string readoutName = aLcdd.sensitiveDetector(aDetectorName).readout().name();
      return new drc::SCEPCALSiPMSD(aDetectorName,readoutName,aLcdd.sensitiveDetector(aDetectorName).readout().segmentation());
  }

}
}

DECLARE_EXTERNAL_GEANT4SENSITIVEDETECTOR(SCEPCALSiPMSD,dd4hep::sim::create_SCEPCALSiPMSD)
