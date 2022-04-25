cet_make_library(
  SOURCE
  ActionBase.cc
  EventActionBase.cc
  PrimaryGeneratorActionBase.cc
  RunActionBase.cc
  StackingActionBase.cc
  SteppingActionBase.cc
  TrackingActionBase.cc
  LIBRARIES PRIVATE
  artg4tk::services_ActionHolder_service
  art::Framework_Services_Registry)

cet_make_plugin_builder(artg4tk::ActionService art::service Modules
  LIBRARIES CONDITIONAL artg4tk::actionBase)

install_headers()
install_source()
