cet_build_plugin(PhysicsList art::service
  LIBRARIES PUBLIC
  artg4tk::services_PhysicsListHolder_service
  artg4tk::lists
  Geant4::G4physicslists
  Geant4::G4run
)

install_headers()
install_source()
