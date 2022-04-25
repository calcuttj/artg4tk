cet_make_library(SOURCE ArtG4tkParticle.cc
  LIBRARIES PUBLIC
  Geant4::G4particles CLHEP::Vector)
art_dictionary(DICTIONARY_LIBRARIES artg4tk::DataProducts_G4DetectorHits)

install_headers()
install_source()
