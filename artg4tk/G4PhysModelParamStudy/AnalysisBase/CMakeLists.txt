cet_make_library(NO_INSTALL
  SOURCE
  AnalyzerWithExpDataBase.cc
  ModelParamAnalyzerBase.cc
  LIBRARIES
  PUBLIC
  artg4tk::G4PhysModelParamStudy_AnalysisProducts
  artg4tk::ExpDataAccess
  art::Framework_Core
  PRIVATE
  artg4tk::DataProducts_G4DetectorHits
  artg4tk::DataProducts_G4ModelConfig
  art_root_io::TFileService_service
  Geant4::G4particles
  Geant4::G4processes
  Geant4::G4materials
  ROOT::Gpad
  ROOT::Hist
  ROOT::Core
  ROOT::RIO
)

# We need to know where the header is, but not link to any libraries.
target_include_directories(artg4tk_G4PhysModelParamStudy_AnalysisBase
  PRIVATE $<TARGET_GENEX_EVAL:CLHEP::CLHEP,$<TARGET_PROPERTY:CLHEP::CLHEP,INTERFACE_INCLUDE_DIRECTORIES>>)
