cet_build_plugin(KillerAction artg4tk::ActionService
  LIBRARIES
  PUBLIC
  messagefacility::MF_MessageLogger
  PRIVATE
  Geant4::G4processes
  Geant4::G4track
)

install_headers()
install_source()
