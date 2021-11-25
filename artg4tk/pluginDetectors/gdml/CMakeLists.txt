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
    Geant4::G4digits_hits
    Geant4::G4event
    Geant4::G4geometry
    Geant4::G4global
    Geant4::G4graphics_reps
    Geant4::G4particles
    Geant4::G4persistency
    Geant4::G4tracking
    artg4tk::DataProducts_G4DetectorHits
    XercesC::XercesC
  )


cet_build_plugin(GDMLDetector art::service
  LIBRARIES
    Geant4::G4digits_hits
    Geant4::G4persistency
    XercesC::XercesC
    art::Framework_Core
    art::Framework_Principal
    art::Persistency_Provenance
    artg4tk::DataProducts_G4DetectorHits
    artg4tk::pluginDetectors_gdml
    artg4tk::services_DetectorHolder_service
    messagefacility::MF_MessageLogger
)

install_headers()
