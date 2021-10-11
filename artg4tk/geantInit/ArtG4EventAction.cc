// ArtG4EventAction.cc provides implementation of Art G4's built-in event action.

// Authors: Tasha Arvanitis, Adam Lyon

// Date: July 2012

// Include header
#include "artg4tk/geantInit/ArtG4EventAction.hh"

// Other local includes
#include "artg4tk/services/ActionHolder_service.hh"
#include "artg4tk/services/DetectorHolder_service.hh"

// art
#include "art/Framework/Services/Registry/ServiceHandle.h"

// C++
#include <map>

// G4 includes
#include "Geant4/G4Event.hh"

using std::map;
using std::string;

// Called at the beginning of each event. Pass the call on to action objects
void
artg4tk::ArtG4EventAction::BeginOfEventAction(const G4Event* currentEvent)
{
  // Get the action holder service
  art::ServiceHandle<ActionHolderService> ahs;

  // Run beginOfEvent
  ahs->beginOfEventAction(currentEvent);
}

// Called at the end of each event. Call detectors to convert hits for the
// event and pass the call on to the action objects.
void
artg4tk::ArtG4EventAction::EndOfEventAction(const G4Event* currentEvent)
{
  // Convert geant hits to art for DETECTORS
  art::ServiceHandle<artg4tk::DetectorHolderService> dhs;
  dhs->fillEventWithArtHits(currentEvent->GetHCofThisEvent());

  // Run EndOfEventAction
  art::ServiceHandle<ActionHolderService> ahs;
  ahs->endOfEventAction(currentEvent);

  // Every ACTION needs to write out their event data now, if they have any
  // (do this within ArtG4EventAction) since some still need to be within
  // Geant
  ahs->fillEventWithArtStuff();
}
