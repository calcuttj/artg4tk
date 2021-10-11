#include "artg4tk/geantInit/ArtG4StackingAction.hh"
#include "artg4tk/services/ActionHolder_service.hh"

artg4tk::ArtG4StackingAction::ArtG4StackingAction(ActionHolderService* actionHolder)
  : actionHolder_{actionHolder}
{}

G4ClassificationOfNewTrack
artg4tk::ArtG4StackingAction::ClassifyNewTrack(G4Track const* currTrack)
{
  bool const killTrack = actionHolder_->killNewTrack(currTrack);
  return killTrack ? fKill : fUrgent;
}
