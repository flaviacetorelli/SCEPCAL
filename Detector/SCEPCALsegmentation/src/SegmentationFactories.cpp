#include "DD4hep/Factories.h"
#include "DD4hep/detail/SegmentationsInterna.h"

using namespace dd4hep;
using namespace dd4hep::DDSegmentation;

namespace {
  template <typename T> dd4hep::SegmentationObject*
  create_segmentation(const dd4hep::BitFieldCoder* decoder) {
  return new dd4hep::SegmentationWrapper<T>(decoder);
  }
}

#include "SCEPCALSegmentation.h"
DECLARE_SEGMENTATION(SCEPCALSegmentation, create_segmentation<dd4hep::DDSegmentation::SCEPCALSegmentation>)
