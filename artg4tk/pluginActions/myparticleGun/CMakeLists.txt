cet_build_plugin(HepevtInputAction artg4tk::ActionService
  LIBRARIES
    PUBLIC
      Geant4::G4event
    PRIVATE
      artg4tk::services_ActionHolder_service
      messagefacility::MF_MessageLogger
)

cet_build_plugin(myParticleGunAction artg4tk::ActionService
  LIBRARIES
    PUBLIC
      Geant4::G4event
    PRIVATE
      artg4tk::services_ActionHolder_service
      messagefacility::MF_MessageLogger
)

install_headers()
install_source()
