#include "artg4tk/pluginDetectors/gdml/ByParticle.hh"
#include "artg4tk/pluginDetectors/gdml/CalorimeterHit.hh"
#include "artg4tk/pluginDetectors/gdml/DRCalorimeterHit.hh"
#include "artg4tk/pluginDetectors/gdml/PhotonHit.hh"
#include "artg4tk/pluginDetectors/gdml/TrackerHit.hh"
#include "artg4tk/pluginDetectors/gdml/myInteractionArtHitData.hh"
#include "canvas/Persistency/Common/Wrapper.h"
template class art::Wrapper<artg4tk::ByParticle>;
template class art::Wrapper<artg4tk::CalorimeterHitCollection>;
template class art::Wrapper<artg4tk::DRCalorimeterHitCollection>;
template class art::Wrapper<artg4tk::PhotonHitCollection>;
template class art::Wrapper<artg4tk::TrackerHitCollection>;
template class art::Wrapper<artg4tk::myInteractionArtHitDataCollection>;
