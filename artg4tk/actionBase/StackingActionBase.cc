#include "artg4tk/actionBase/StackingActionBase.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4tk/services/ActionHolder_service.hh"

artg4tk::StackingActionBase::StackingActionBase(std::string name) : ActionBase{move(name)}
{
  art::ServiceHandle<artg4tk::ActionHolderService>()->registerAction(this);
}

artg4tk::StackingActionBase::~StackingActionBase() = default;
