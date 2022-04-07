cet_make_library(INTERFACE
  SOURCE DetectorBase.hh RunSettingsBase.hh
  LIBRARIES INTERFACE
  artg4tk::services_DetectorHolder_service
  artg4tk::services_RunSettings_service
  art::Framework_Principal
  art::Framework_Services_Registry
  fhiclcpp::fhiclcpp
)  

cet_build_plugin(artg4Main art::module
  LIBRARIES PRIVATE
    artg4tk::geantInit
    artg4tk::services_DetectorHolder_service
    artg4tk::services_PhysicsListHolder_service
    Geant4::G4interfaces
)

install_headers()
install_source()
