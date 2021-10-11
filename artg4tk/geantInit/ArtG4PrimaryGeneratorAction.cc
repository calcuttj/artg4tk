#include "artg4tk/geantInit/ArtG4PrimaryGeneratorAction.hh"
#include "artg4tk/services/ActionHolder_service.hh"

artg4tk::ArtG4PrimaryGeneratorAction::ArtG4PrimaryGeneratorAction(ActionHolderService* actionHolder)
  : actionHolder_{actionHolder}
{}

void
artg4tk::ArtG4PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  actionHolder_->generatePrimaries(anEvent);
}
