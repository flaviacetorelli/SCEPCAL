#ifndef SiPMHit_h
#define SiPMHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

#include "DD4hep/Objects.h"
#include "DD4hep/Segmentations.h"

namespace drc {
  class SiPMHit : public G4VHit {
  public:
    typedef std::map<float, int> SCEPCALsimTimeStruct;
    typedef std::map<float, int> SCEPCALsimWavlenSpectrum;

    SiPMHit(float wavSampling, float timeSampling);
    SiPMHit(const SiPMHit &right);
    virtual ~SiPMHit();

    const SiPMHit& operator=(const SiPMHit &right);
    G4bool operator==(const SiPMHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void* aHit);

    virtual void Draw() {};
    virtual void Print() {};

    void photonCount() { fPhotons++; }
    unsigned long GetPhotonCount() const { return fPhotons; }

    void SetSiPMnum(dd4hep::DDSegmentation::CellID n) { fSiPMnum = n; }
    const dd4hep::DDSegmentation::CellID& GetSiPMnum() const { return fSiPMnum; }

    void CountWavlenSpectrum(float center);
    const SCEPCALsimWavlenSpectrum& GetWavlenSpectrum() const { return fWavlenSpectrum; }

    void CountTimeStruct(float center);
    const SCEPCALsimTimeStruct& GetTimeStruct() const { return fTimeStruct; }

    float GetSamplingTime() { return mTimeSampling; }
    float GetSamplingWavlen() { return mWavSampling; }

  private:
    dd4hep::DDSegmentation::CellID fSiPMnum;
    unsigned long fPhotons;
    SCEPCALsimWavlenSpectrum fWavlenSpectrum;
    SCEPCALsimTimeStruct fTimeStruct;
    float mWavSampling;
    float mTimeSampling;
  };

  typedef G4THitsCollection<SiPMHit> SiPMHitsCollection;
  extern G4ThreadLocal G4Allocator<SiPMHit>* SiPMHitAllocator;

  inline void* SiPMHit::operator new(size_t) {
    if (!SiPMHitAllocator) SiPMHitAllocator = new G4Allocator<SiPMHit>;
    return (void*)SiPMHitAllocator->MallocSingle();
  }

  inline void SiPMHit::operator delete(void*aHit) {
    SiPMHitAllocator->FreeSingle((SiPMHit*) aHit);
  }
}

#endif
