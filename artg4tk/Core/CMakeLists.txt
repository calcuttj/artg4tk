cet_make_library(LIBRARY_NAME DetectorService INTERFACE
  SOURCE DetectorBase.hh
  LIBRARIES INTERFACE
  artg4tk::services_DetectorHolder_service
  art::Framework_Principal
  art::Framework_Services_Registry
  fhiclcpp::fhiclcpp
)

cet_make_library(LIBRARY_NAME RunSettingsService INTERFACE
  SOURCE RunSettingsBase.hh
  LIBRARIES INTERFACE
  artg4tk::services_RunSettings_service
  art::Framework_Services_Registry
  fhiclcpp::fhiclcpp
)

set(builder_dest Modules)
cet_make_plugin_builder(artg4tk::DetectorService
  art::service ${builder_dest}
  LIBRARIES CONDITIONAL artg4tk::DetectorService)
cet_make_plugin_builder(artg4tk::RunSettingsService
  art::service ${builder_dest}
  LIBRARIES CONDITIONAL artg4tk::RunSettingsService)

cet_build_plugin(artg4Main art::module
  LIBRARIES PRIVATE
    artg4tk::geantInit
    artg4tk::services_DetectorHolder_service
    artg4tk::services_PhysicsListHolder_service
    Geant4::G4interfaces
)

install_headers()
install_source()
