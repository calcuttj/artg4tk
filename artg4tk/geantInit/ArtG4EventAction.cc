// ArtG4EventAction.cc provides implementation of Art G4's built-in event action.

// Authors: Tasha Arvanitis, Adam Lyon

// Date: July 2012

#include "artg4tk/geantInit/ArtG4EventAction.hh"
#include "artg4tk/services/ActionHolder_service.hh"
#include "artg4tk/services/DetectorHolder_service.hh"

#include "Geant4/G4Event.hh"

artg4tk::ArtG4EventAction::ArtG4EventAction(ActionHolderService* actionHolder,
                                            DetectorHolderService* detectorHolder)
  : actionHolder_{actionHolder}, detectorHolder_{detectorHolder}
{}

// Called at the beginning of each event. Pass the call on to action objects
void
artg4tk::ArtG4EventAction::BeginOfEventAction(const G4Event* currentEvent)
{
  actionHolder_->beginOfEventAction(currentEvent);
}

// Called at the end of each event. Call detectors to convert hits for the
// event and pass the call on to the action objects.
void
artg4tk::ArtG4EventAction::EndOfEventAction(const G4Event* currentEvent)
{
  detectorHolder_->fillEventWithArtHits(currentEvent->GetHCofThisEvent());
  actionHolder_->endOfEventAction(currentEvent);
}
