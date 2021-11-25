#include "artg4tk/actionBase/TrackingActionBase.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4tk/services/ActionHolder_service.hh"

artg4tk::TrackingActionBase::TrackingActionBase(std::string name) : ActionBase{move(name)}
{
  art::ServiceHandle<artg4tk::ActionHolderService>()->registerAction(this);
}

artg4tk::TrackingActionBase::~TrackingActionBase() = default;
