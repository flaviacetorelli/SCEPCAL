#include "SiPMHit.h"

G4ThreadLocal G4Allocator<drc::SiPMHit>* drc::SiPMHitAllocator = 0;

drc::SiPMHit::SiPMHit(float wavSampling, float timeSampling)
: G4VHit(),
  fSiPMnum(0),
  fPhotons(0),
  mWavSampling(wavSampling),
  mTimeSampling(timeSampling)
{}

drc::SiPMHit::~SiPMHit() {}

drc::SiPMHit::SiPMHit(const drc::SiPMHit &right)
: G4VHit() {
  fSiPMnum = right.fSiPMnum;
  fPhotons = right.fPhotons;
  fWavlenSpectrum = right.fWavlenSpectrum;
  fTimeStruct = right.fTimeStruct;
  mWavSampling = right.mWavSampling;
  mTimeSampling = right.mTimeSampling;
}

const drc::SiPMHit& drc::SiPMHit::operator=(const drc::SiPMHit &right) {
  fSiPMnum = right.fSiPMnum;
  fPhotons = right.fPhotons;
  fWavlenSpectrum = right.fWavlenSpectrum;
  fTimeStruct = right.fTimeStruct;
  mWavSampling = right.mWavSampling;
  mTimeSampling = right.mTimeSampling;
  return *this;
}

G4bool drc::SiPMHit::operator==(const drc::SiPMHit &right) const {
  return (fSiPMnum==right.fSiPMnum);
}

void drc::SiPMHit::CountWavlenSpectrum(float center) {
  auto it = fWavlenSpectrum.find(center);
  if (it==fWavlenSpectrum.end()) fWavlenSpectrum.insert(std::make_pair(center,1));
  else it->second++;
}

void drc::SiPMHit::CountTimeStruct(float center) {
  auto it = fTimeStruct.find(center);
  if (it==fTimeStruct.end()) fTimeStruct.insert(std::make_pair(center,1));
  else it->second++;
}
