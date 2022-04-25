cet_build_plugin(ExampleGeneralAction artg4tk::ActionService
  LIBRARIES
    artg4tk::services_ActionHolder_service
    Geant4::G4particles
    Geant4::G4run
)

install_headers()
install_source()
