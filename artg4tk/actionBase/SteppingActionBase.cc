#include "artg4tk/actionBase/SteppingActionBase.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4tk/services/ActionHolder_service.hh"

artg4tk::SteppingActionBase::SteppingActionBase(std::string name) : ActionBase{move(name)}
{
  art::ServiceHandle<artg4tk::ActionHolderService>()->registerAction(this);
}

artg4tk::SteppingActionBase::~SteppingActionBase() = default;
