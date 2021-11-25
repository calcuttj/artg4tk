#include "artg4tk/actionBase/PrimaryGeneratorActionBase.hh"
#include "artg4tk/services/ActionHolder_service.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"

artg4tk::PrimaryGeneratorActionBase::PrimaryGeneratorActionBase(std::string name) : ActionBase{move(name)}
{
  art::ServiceHandle<artg4tk::ActionHolderService>()->registerAction(this);
}

artg4tk::PrimaryGeneratorActionBase::~PrimaryGeneratorActionBase() = default;
