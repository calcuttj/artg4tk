// Implemenation for PhysicsListHolder_service

#include "artg4tk/services/PhysicsListHolder_service.hh"
#include "artg4tk/services/PhysicsListServiceBase.hh"

void
artg4tk::PhysicsListHolderService::registerPhysicsListService(PhysicsListServiceBase* pl)
{
  // There can be only one
  if (physicsListService_) {
    throw cet::exception("PhysicsListHolderService") << "A physics list is already registered.\n";
  }

  physicsListService_ = pl;
}

G4VUserPhysicsList*
artg4tk::PhysicsListHolderService::makePhysicsList() const
{
  // Make sure we have one
  if (!physicsListService_) {
    throw cet::exception("PhysicsListHolderService") << "No physics list has been registered.\n";
  }

  // Return the physics list (Geant is going to delete it)
  return physicsListService_->makePhysicsList();
}

void
artg4tk::PhysicsListHolderService::initializePhysicsList() const
{
  // Make sure we have one
  if (!physicsListService_) {
    throw cet::exception("PhysicsListHolderService") << "No physics list has been registered.\n";
  }

  physicsListService_->initializePhysicsList();
}
