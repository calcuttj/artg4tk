// Provides the implementation for the @ActionHolderService@ service.
// For more comprehensive documentation, see the header file ActionHolderService.hh

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// Includes
#include "artg4tk/services/ActionHolder_service.hh"

#include "artg4tk/actionBase/EventActionBase.hh"
#include "artg4tk/actionBase/PrimaryGeneratorActionBase.hh"
#include "artg4tk/actionBase/RunActionBase.hh"
#include "artg4tk/actionBase/StackingActionBase.hh"
#include "artg4tk/actionBase/SteppingActionBase.hh"
#include "artg4tk/actionBase/TrackingActionBase.hh"

#include "messagefacility/MessageLogger/MessageLogger.h"
#include "range/v3/view.hpp"

#include <algorithm>
#include <iostream>

using ranges::views::values;

/////////////////////
// Message category
static std::string const msgctg = "ActionHolderService";

// Constructor doesn't do much with the passed arguments, but does initialize
// the logger for the service
artg4tk::ActionHolderService::ActionHolderService(fhicl::ParameterSet const&) {}

// Register actions
template <typename A>
void
artg4tk::ActionHolderService::doRegisterAction(A* const action,
                                               std::map<std::string, A*>& actionMap)
{
  mf::LogDebug(msgctg) << "Registering action " << action->myName();

  // Check if the name exists in the specific action map
  if (0 == actionMap.count(action->myName())) {
    // Add the action!
    actionMap.try_emplace(action->myName(), action);

    // Now, check whether the name exists in the overall map of all the actions
    // If so, move on (don't throw an exception, since a single action may need
    // to register in multiple maps). Otherwise, add it.
    allActionsMap_.try_emplace(action->myName(), action);
  }

  else {
    // We already have this action in the specific action map - this is bad!
    throw cet::exception("ActionHolderService")
      << "Duplicate action named " << action->myName() << ".\n";
  }
}

void
artg4tk::ActionHolderService::registerAction(RunActionBase* const action)
{
  std::cerr << "registering to   runActionsMap_\n";
  doRegisterAction(action, runActionsMap_);
}

void
artg4tk::ActionHolderService::registerAction(EventActionBase* const action)
{
  std::cerr << "registering to   eventActionsMap_\n";
  doRegisterAction(action, eventActionsMap_);
}

void
artg4tk::ActionHolderService::registerAction(TrackingActionBase* const action)
{
  std::cerr << "registering to   trackingActionsMap_\n";
  doRegisterAction(action, trackingActionsMap_);
}

void
artg4tk::ActionHolderService::registerAction(SteppingActionBase* const action)
{
  std::cerr << "registering to  steppingActionsMap_\n";
  doRegisterAction(action, steppingActionsMap_);
}

void
artg4tk::ActionHolderService::registerAction(StackingActionBase* const action)
{
  std::cerr << "registering to  stackingActionsMap_\n";
  doRegisterAction(action, stackingActionsMap_);
}

void
artg4tk::ActionHolderService::registerAction(PrimaryGeneratorActionBase* const action)
{
  std::cerr << "registering to  primaryGeneratorActionsMap_\n";
  doRegisterAction(action, primaryGeneratorActionsMap_);
}

// h3. Art-specific methods
void
artg4tk::ActionHolderService::callArtProduces(art::ProducesCollector& collector)
{
  for (auto action : eventActionsMap_ | values) {
    action->callArtProduces(collector);
  }
}

void
artg4tk::ActionHolderService::initialize()
{
  for (auto action : allActionsMap_ | values) {
    action->initialize();
  }
}

// h2. Action methods

// h3. Event action methods
void
artg4tk::ActionHolderService::beginOfEventAction(const G4Event* theEvent)
{
  for (auto action : eventActionsMap_ | values) {
    action->beginOfEventAction(theEvent);
  }
}

void
artg4tk::ActionHolderService::endOfEventAction(const G4Event* theEvent)
{
  for (auto action : eventActionsMap_ | values) {
    action->endOfEventAction(theEvent);
  }
}

// h3. Tracking action methods
void
artg4tk::ActionHolderService::preUserTrackingAction(const G4Track* theTrack)
{
  for (auto action : trackingActionsMap_ | values) {
    action->preUserTrackingAction(theTrack);
  }
}

void
artg4tk::ActionHolderService::postUserTrackingAction(const G4Track* theTrack)
{
  for (auto action : trackingActionsMap_ | values) {
    action->postUserTrackingAction(theTrack);
  }
}

// h3. Stepping actions
void
artg4tk::ActionHolderService::userSteppingAction(const G4Step* theStep)
{
  for (auto action : steppingActionsMap_ | values) {
    action->userSteppingAction(theStep);
  }
}

// h3. Stacking actions
bool
artg4tk::ActionHolderService::killNewTrack(const G4Track* newTrack)
{
  for (auto action : stackingActionsMap_ | values) {
    if (action->killNewTrack(newTrack)) {
      return true;
    }
  }
  return false;
}

// h3. Primary generator actions
void
artg4tk::ActionHolderService::generatePrimaries(G4Event* theEvent)
{
  for (auto action : primaryGeneratorActionsMap_ | values) {
    action->generatePrimaries(theEvent);
  }
}
