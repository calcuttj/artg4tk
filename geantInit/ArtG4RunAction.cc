// ArtG4RunAction.cc provides definitions for Art G4's built-in run action.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// Include header
#include "artg4tk/geantInit/ArtG4RunAction.hh"

// Other local includes
#include "artg4tk/services/ActionHolder_service.hh"

// Art
#include "art/Framework/Services/Registry/ServiceHandle.h"


// Called at the beginning of each run:
void artg4tk::ArtG4RunAction::BeginOfRunAction(const G4Run * currentRun)
{
  // Get the action holder service
  art::ServiceHandle<ActionHolderService> actionHolder;
  
  // Run beginOfRunAction
  actionHolder -> beginOfRunAction(currentRun);
  
  // Actions can write out data at the begin run if necessary
  actionHolder -> fillRunBeginWithArtStuff();
  
}
// Called at the end of each run:
void artg4tk::ArtG4RunAction::EndOfRunAction(const G4Run * currentRun)
{
  // Get the action holder service
  art::ServiceHandle<ActionHolderService> actionHolder;
  
  // Run endOfRunAction
  actionHolder -> endOfRunAction(currentRun);
  
  // Actions need to write out their run data, if they have any
  actionHolder -> fillRunEndWithArtStuff();
}
