//
//               __        __ __  __  __
//   ____ ______/ /_____ _/ // / / /_/ /__
//  / __ `/ ___/ __/ __ `/ // /_/ __/ //_/
// / /_/ / /  / /_/ /_/ /__  __/ /_/ ,<
// \__,_/_/   \__/\__, /  /_/  \__/_/|_|
//               /____/
//
// artg4tk: art based Geant 4 Toolkit
//
//=============================================================================
// KillerActionService.cc: example artg4tk stacking action that allows to
// liik certain particles of interest ((p0, eta, gamma from nCapture)
// To use this, all you need to do is put it in the services section
// of the configuration file, like this:
//
// services: {
//   ...
//   user: {
//        KillerAction : {name: "KillerAction"
//        killPi0: true
//        killeta: false
//        killGammafromnCapture: false
//          }
//     . ..
//   }
// }
// Author: Hans Wenzel (Fermilab)
//=============================================================================

#include "artg4tk/pluginActions/myKiller/KillerAction_service.hh"

// G4 includes
#include "Geant4/G4Track.hh"
#include "Geant4/G4VProcess.hh"

using std::string;

artg4tk::KillerActionService::KillerActionService(fhicl::ParameterSet const& p)
  : StackingActionBase(p.get<string>("name", "myKillerAction"))
  , killPi0(p.get<bool>("killPi0"))
  , killeta(p.get<bool>("killeta"))
  , killGammafromnCapture(p.get<bool>("killGammafromnCapture"))
  , logInfo_("KillerAction")
{
  // Register ourselves with the ActionHolder
  //  art::ServiceHandle<ActionHolderService> actionHolder;
}

bool
artg4tk::KillerActionService::killNewTrack(const G4Track* aTrack)
{
  if (aTrack->GetParentID() == 0)
    return false;
  if (aTrack->GetDefinition()->GetParticleName() == "pi0") {
    if (killPi0) {
      return true;
    }
  }
  if (aTrack->GetDefinition()->GetParticleName() == "eta") {
    if (killeta) {
      return true;
    }
  }
  if (aTrack->GetDefinition()->GetParticleName() == "gamma") {
    if (aTrack->GetCreatorProcess()->GetProcessName() == "nCapture") {
      if (killGammafromnCapture) {
        return true;
      }
    }
  }
  return false;
}
// Destructor

artg4tk::KillerActionService::~KillerActionService() {}
