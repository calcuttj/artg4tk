#include "artg4tk/geantInit/ArtG4SteppingAction.hh"
#include "artg4tk/services/ActionHolder_service.hh"

artg4tk::ArtG4SteppingAction::ArtG4SteppingAction(ActionHolderService* actionHolder)
  : actionHolder_{actionHolder}
{}

void
artg4tk::ArtG4SteppingAction::UserSteppingAction(G4Step const* currentStep)
{
  actionHolder_->userSteppingAction(currentStep);
}
