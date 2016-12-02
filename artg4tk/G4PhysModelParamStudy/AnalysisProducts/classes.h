//
// Build a dictionary.
//
// #include <vector>
// #include <map>
// #include <string>
//
// #include "artg4tk/DataProducts/ThinTargetAnalysis/BeamTargetConfig.hh"
#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/BeamThinTargetConfig.hh"
#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/RecordChi2.hh"
#include "art/Persistency/Common/Wrapper.h"
template class art::Wrapper<BeamThinTargetConfig>;
template class art::Wrapper<InfoChi2>;
template class art::Wrapper<RecordChi2>;
