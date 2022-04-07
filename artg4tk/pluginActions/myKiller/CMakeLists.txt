cet_build_plugin(KillerAction art::service
  LIBRARIES
  PUBLIC
  artg4tk::actionBase
  messagefacility::MF_MessageLogger
  PRIVATE
  Geant4::G4processes
  Geant4::G4track
)

install_headers()
install_source()
