cet_build_plugin(HepevtInputAction art::service
  LIBRARIES
    PUBLIC
      artg4tk::actionBase
    PRIVATE
      artg4tk::services_ActionHolder_service
      Geant4::G4event
      messagefacility::MF_MessageLogger
)

cet_build_plugin(myParticleGunAction art::service
  LIBRARIES
    PUBLIC
      artg4tk::actionBase
    PRIVATE
      artg4tk::services_ActionHolder_service
      Geant4::G4event
      messagefacility::MF_MessageLogger
)
