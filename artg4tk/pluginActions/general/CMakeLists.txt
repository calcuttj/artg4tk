cet_build_plugin(ExampleGeneralAction art::service
  LIBRARIES
    artg4tk::actionBase
    artg4tk::services_ActionHolder_service
    Geant4::G4particles
    Geant4::G4run
)

install_headers()
install_source()
