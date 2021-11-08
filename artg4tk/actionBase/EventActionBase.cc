#include "artg4tk/actionBase/EventActionBase.hh"

artg4tk::EventActionBase::EventActionBase(std::string name) : ActionBase{move(name)}
{
  art::ServiceHandle<artg4tk::ActionHolderService>()->registerAction(this);
}

artg4tk::EventActionBase::~EventActionBase() = default;
