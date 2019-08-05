// ArtG4TrackingAction.cc provides definitions for Art G4's built-in tracking action

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// Include header
#include "artg4tk/geantInit/ArtG4TrackingAction.hh"

// Other local includes
#include "artg4tk/services/ActionHolder_service.hh"

// Art
#include "art/Framework/Services/Registry/ServiceHandle.h"

// Called after the creation of a track and before it's actually simulated
void artg4tk::ArtG4TrackingAction::PreUserTrackingAction(const G4Track* currTrack)
{
  // Get the action holder service
  art::ServiceHandle<ActionHolderService> actionHolder;
  
  // Run preUserTrackingAction
  actionHolder -> preUserTrackingAction(currTrack);
}

// Called once a track has been stopped
void artg4tk::ArtG4TrackingAction::PostUserTrackingAction(const G4Track* currTrack)
{
  // Get the action holder service
  art::ServiceHandle<ActionHolderService> actionHolder;
  
  // Run postUserTrackingAction
  actionHolder -> postUserTrackingAction(currTrack);
}
