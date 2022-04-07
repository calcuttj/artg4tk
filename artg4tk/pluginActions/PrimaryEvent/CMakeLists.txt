cet_build_plugin(PrimaryEventAction art::service
  LIBRARIES
    PUBLIC
      artg4tk::actionBase
      Geant4::G4global
    PRIVATE
      art::Framework_Principal
      artg4tk::services_ActionHolder_service
      Geant4::G4event
)

install_headers()
install_source()
