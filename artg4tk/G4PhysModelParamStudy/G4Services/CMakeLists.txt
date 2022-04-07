cet_build_plugin(PhysModelConfig art::service NO_INSTALL
  LIBRARIES PRIVATE artg4tk::G4PhysModelParamStudy_G4Components)

cet_build_plugin(ProcLevelSimSetup art::service NO_INSTALL
  LIBRARIES
  PUBLIC
  messagefacility::MF_MessageLogger
  PRIVATE
  Geant4::G4materials
  Geant4::G4track
  Geant4::G4geometry
  Geant4::G4global
  CLHEP::Random
)


