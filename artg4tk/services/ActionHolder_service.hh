// Declarations for the @ActionHolderService@ Art service.

// @ActionHolderService@ is a globally-accessible service that manages the action
// objects for a simulation. An action object has a multitude of hooks into
// various points during event creation and processing. All action objects
// must be registered with this service in order to function.

// Any class can @#include@ and access the @ActionHolderService@ service to get either
// a collection of registered action objects or a specific action object given
// a name.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

#ifndef artg4tk_services_ActionHolder_service_hh
#define artg4tk_services_ActionHolder_service_hh

// Includes
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
#include "fhiclcpp/fwd.h"

namespace art {
  class ProducesCollector;
  class Event;
  class Run;
}

#include <map>

class G4Run;
class G4Event;
class G4Track;
class G4Step;

// Everything for the Art G4 simulation goes in the @artg4tk@ namespace
namespace artg4tk {

  class ActionBase;
  class RunActionBase;
  class EventActionBase;
  class TrackingActionBase;
  class SteppingActionBase;
  class StackingActionBase;
  class PrimaryGeneratorActionBase;

  class ActionHolderService {
  public:
    explicit ActionHolderService(fhicl::ParameterSet const&);

    // This method registers the passed action object with the service
    void registerAction(RunActionBase* const action);
    void registerAction(EventActionBase* const action);
    void registerAction(TrackingActionBase* const action);
    void registerAction(SteppingActionBase* const action);
    void registerAction(StackingActionBase* const action);
    void registerAction(PrimaryGeneratorActionBase* const action);

    // Call ActionBase::initialize for each action
    void initialize();

    // h3. Art-specific methods

    // Tell each action to notify Art of what it will be producing.
    void callArtProduces(art::ProducesCollector& prod);

    // Set/get the current Art event
    void
    setCurrArtEvent(art::Event& e)
    {
      currentArtEvent_ = &e;
    }
    art::Event&
    getCurrArtEvent()
    {
      return (*currentArtEvent_);
    }

    // Set/get the current Art Run
    void
    setCurrArtRun(art::Run& r)
    {
      currentArtRun_ = &r;
    }
    art::Run&
    getCurrArtRun()
    {
      return (*currentArtRun_);
    }

    // h3. Action methods

    // h4. Event Actions
    void beginOfEventAction(const G4Event*);
    void endOfEventAction(const G4Event*);

    // h4. Tracking actions
    void preUserTrackingAction(const G4Track*);
    void postUserTrackingAction(const G4Track*);

    // h4. Stepping actions
    void userSteppingAction(const G4Step*);

    // h4. Stacking actions
    bool killNewTrack(const G4Track*);

    // h4. Primary Generator actions
    void generatePrimaries(G4Event*);

  private:
    // A collection of all our actions, arranged by name
    std::map<std::string, RunActionBase*> runActionsMap_{};
    std::map<std::string, EventActionBase*> eventActionsMap_{};
    std::map<std::string, TrackingActionBase*> trackingActionsMap_{};
    std::map<std::string, SteppingActionBase*> steppingActionsMap_{};
    std::map<std::string, StackingActionBase*> stackingActionsMap_{};
    std::map<std::string, PrimaryGeneratorActionBase*> primaryGeneratorActionsMap_{};

    // Hold on to the current Art event
    art::Event* currentArtEvent_{nullptr};

    // Hold on to the current Art run
    art::Run* currentArtRun_{nullptr};

    // An uber-collection of all registered actions, arranged by name
    std::map<std::string, ActionBase*> allActionsMap_{};

    // Register the action
    template <typename A>
    void doRegisterAction(A* const action, std::map<std::string, A*>& actionMap);
  };
} // namespace artg4tk

DECLARE_ART_SERVICE(artg4tk::ActionHolderService, LEGACY)

#endif /* artg4tk_services_ActionHolder_service_hh */
