art_dictionary()

cet_make_library(SOURCE
    CalorimeterSD.cc
    ColorReader.cc
    DRCalorimeterSD.cc
    HadIntAndEdepTrkSD.cc
    HadInteractionSD.cc
    PhotonSD.cc
    TrackerSD.cc
    LIBRARIES
    PUBLIC
    artg4tk::DataProducts_G4DetectorHits
    Geant4::G4digits_hits
    Geant4::G4persistency
    Geant4::G4track
    canvas::canvas
    PRIVATE
    Geant4::G4event
    Geant4::G4geometry
    Geant4::G4global
    Geant4::G4graphics_reps
    Geant4::G4particles
    Geant4::G4processes
    Geant4::G4tracking
  )

cet_build_plugin(GDMLDetector artg4tk::DetectorService
  LIBRARIES
  PUBLIC
  messagefacility::MF_MessageLogger
  PRIVATE
  artg4tk::pluginDetectors_gdml
  artg4tk::services_DetectorHolder_service
  artg4tk::DataProducts_G4DetectorHits
  art::Framework_Core
  Geant4::G4digits_hits
  Geant4::G4geometry
  Geant4::G4processes
  Geant4::G4global
)

install_headers()
install_source()
