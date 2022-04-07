cet_build_plugin(ActionHolder art::service
  LIBRARIES
  PRIVATE messagefacility::MF_MessageLogger range-v3::range-v3)
cet_build_plugin(DetectorHolder art::service
  LIBRARIES
  PRIVATE messagefacility::MF_MessageLogger)
cet_build_plugin(RunSettings art::service
  LIBRARIES
  PRIVATE messagefacility::MF_MessageLogger)
cet_build_plugin(PhysicsListHolder art::service
  LIBRARIES
  PRIVATE
  Geant4::G4run)
cet_build_plugin(Dummy art::service
  LIBRARIES
  PRIVATE
  art::Framework_Core
  messagefacility::MF_MessageLogger)

install_headers()
install_source()
