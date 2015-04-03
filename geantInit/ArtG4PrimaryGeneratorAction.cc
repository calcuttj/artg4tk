// ArtG4PrimaryGeneratorAction.cc provides definitions for Art G4's built-in
// primary generator action

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// Include header
#include "artg4tk/geantInit/ArtG4PrimaryGeneratorAction.hh"

// Other local-ish includes
#include "artg4tk/services/ActionHolder_service.hh"

// Art
#include "art/Framework/Services/Registry/ServiceHandle.h"

// Called to create primaries for an event
void artg4tk::ArtG4PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  // Get the action holder service
  art::ServiceHandle<ActionHolderService> actionHolder;
  
  // Run generatePrimaries
  actionHolder -> generatePrimaries(anEvent);
  
}
