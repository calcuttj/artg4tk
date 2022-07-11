cet_make_library(SOURCE
  G4MuonLaser.cc
  LIBRARIES PUBLIC
  Geant4::G4global
  Geant4::G4particles
  #Geant4::G4globman
  #Geant4::G4partman
)

install_headers()
install_source()

