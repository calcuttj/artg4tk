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



// Include guard
#ifndef EXAMPLEGENERALACTION_SERVICE_HH
#define EXAMPLEGENERALACTION_SERVICE_HH

// Includes for exampleGeneral action service
#include "fhiclcpp/fwd.h"
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"

// Get the base class
#include "artg4tk/actionBase/TrackingActionBase.hh"
#include "artg4tk/actionBase/RunActionBase.hh"
#include "artg4tk/actionBase/SteppingActionBase.hh"

// Other includes
class G4Run;
class G4Step;
class G4Track;

namespace artg4tk {

  class ExampleGeneralActionService 
    : public artg4tk::TrackingActionBase,
      public artg4tk::RunActionBase,
      public artg4tk::SteppingActionBase
  {
  public: 
    ExampleGeneralActionService(fhicl::ParameterSet const&);
    virtual ~ExampleGeneralActionService();

    // Overload preUserTrackingAction method to decide whether or not to
    // track a given particle
    virtual void preUserTrackingAction(const G4Track* currTrack);

    // Overload userSteppingAction to suspend any tracks that enter the 
    // calorimeter.
    virtual void userSteppingAction(const G4Step * theStep);

    // Overload the beginOfRunAction method to initialize the random number
    // store.
    virtual void beginOfRunAction(const G4Run * currentRun);

    // We don't need to add anything to the event, so we don't need
    // callArtProduces or fillEventWithArtStuff.

    // No private data members needed!

  };
} //namespace artg4tk
using artg4tk::ExampleGeneralActionService;
DECLARE_ART_SERVICE(ExampleGeneralActionService,LEGACY)

#endif
