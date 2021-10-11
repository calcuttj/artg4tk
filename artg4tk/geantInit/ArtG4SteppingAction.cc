// ArtG4SteppingAction.cc provides definitions for Art G4's built-in stepping
// action.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// Include header
#include "artg4tk/geantInit/ArtG4SteppingAction.hh"

// Other local includes
#include "artg4tk/services/ActionHolder_service.hh"

// Art
#include "art/Framework/Services/Registry/ServiceHandle.h"

// Called at the end of each step
void
artg4tk::ArtG4SteppingAction::UserSteppingAction(const G4Step* currentStep)
{
  // Get the action holder service
  art::ServiceHandle<ActionHolderService> actionHolder;

  // Run userSteppingAction
  actionHolder->userSteppingAction(currentStep);
}
