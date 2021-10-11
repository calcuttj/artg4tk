#include "artg4tk/geantInit/ArtG4TrackingAction.hh"
#include "artg4tk/services/ActionHolder_service.hh"

artg4tk::ArtG4TrackingAction::ArtG4TrackingAction(ActionHolderService* actionHolder)
  : actionHolder_{actionHolder}
{}

void
artg4tk::ArtG4TrackingAction::PreUserTrackingAction(const G4Track* currTrack)
{
  actionHolder_->preUserTrackingAction(currTrack);
}

void
artg4tk::ArtG4TrackingAction::PostUserTrackingAction(const G4Track* currTrack)
{
  actionHolder_->postUserTrackingAction(currTrack);
}
