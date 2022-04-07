cet_build_plugin(HepevtInputAction art::service
  LIBRARIES
    PUBLIC
      artg4tk::actionBase
      Geant4::G4event
    PRIVATE
      artg4tk::services_ActionHolder_service
      messagefacility::MF_MessageLogger
)

cet_build_plugin(myParticleGunAction art::service
  LIBRARIES
    PUBLIC
      artg4tk::actionBase
      Geant4::G4event
    PRIVATE
      artg4tk::services_ActionHolder_service
      messagefacility::MF_MessageLogger
)

install_headers()
install_source()
