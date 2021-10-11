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
// KillerActionService.hh: example artg4tk stacking action that allows to
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

// Include guard
// Include guard
#ifndef KILLERACTION_SERVICE_HH
#define KILLERACTION_SERVICE_HH

// Includes for general action service
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
#include "fhiclcpp/fwd.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// Get the base class

#include "artg4tk/actionBase/StackingActionBase.hh"

namespace artg4tk {

  class KillerActionService : public artg4tk::StackingActionBase {
  public:
    KillerActionService(fhicl::ParameterSet const&);
    virtual ~KillerActionService();
    virtual bool killNewTrack(const G4Track* aTrack) override;

  private:
    // Member data!
    bool killPi0;
    bool killeta;
    bool killGammafromnCapture;
    // A message logger for this action
    mf::LogInfo logInfo_;
  };
} // end namespace artg4tk
using artg4tk::KillerActionService;
DECLARE_ART_SERVICE(KillerActionService, LEGACY)
#endif
