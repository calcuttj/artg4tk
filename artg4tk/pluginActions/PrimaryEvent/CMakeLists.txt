cet_build_plugin(PrimaryEventAction artg4tk::ActionService
  LIBRARIES
    PUBLIC
      Geant4::G4global
    PRIVATE
      art::Framework_Principal
      artg4tk::services_ActionHolder_service
      Geant4::G4event
)

install_headers()
install_source()
