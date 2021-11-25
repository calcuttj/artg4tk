// ExampleGeneralActionService is the service that provides general actions for
// artg4tk.
// To use this action, all you need to do is put it in the services section
// of the configuration file, like this:
//
// services: {
//   ...
//   user: {
//     ExampleGeneralActionService: {}
//     ...
//   }
// }

// The parameter set passed in to the constructor of this object must contain
// the following parameters:
// - name (string): a name describing the action. It is highly recommended
//       that you stick with the default name for this class.
//       Default is 'exampleGeneral'.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: August 2012

#ifndef artg4tk_pluginActions_general_ExampleGeneralAction_service_hh
#define artg4tk_pluginActions_general_ExampleGeneralAction_service_hh

// Includes for exampleGeneral action service
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
#include "fhiclcpp/fwd.h"

// Get the base class
#include "artg4tk/actionBase/RunActionBase.hh"
#include "artg4tk/actionBase/SteppingActionBase.hh"
#include "artg4tk/actionBase/TrackingActionBase.hh"

// Other includes
class G4Run;
class G4Step;
class G4Track;

namespace artg4tk {
  class ExampleGeneralActionService : public artg4tk::TrackingActionBase,
                                      public artg4tk::RunActionBase,
                                      public artg4tk::SteppingActionBase {
  public:
    explicit ExampleGeneralActionService(fhicl::ParameterSet const&);

    // Override preUserTrackingAction method to decide whether or not to
    // track a given particle
    void preUserTrackingAction(const G4Track* currTrack) override;

    // Override userSteppingAction to suspend any tracks that enter the
    // calorimeter.
    void userSteppingAction(const G4Step* theStep) override;

    // Override the beginOfRunAction method to initialize the random number
    // store.
    void beginOfRunAction(const G4Run* currentRun) override;
  };
} // namespace artg4tk

DECLARE_ART_SERVICE(artg4tk::ExampleGeneralActionService, LEGACY)

#endif /* artg4tk_pluginActions_general_ExampleGeneralAction_service_hh */
