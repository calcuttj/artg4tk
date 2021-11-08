cet_build_plugin(artg4Main art::module
  LIBRARIES PRIVATE
    artg4tk::geantInit
    artg4tk::services_DetectorHolder_service
    artg4tk::services_PhysicsListHolder_service
    Geant4::G4interfaces
)

install_headers()
