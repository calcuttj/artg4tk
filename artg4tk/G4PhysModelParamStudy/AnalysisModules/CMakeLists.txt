# We need to know where the header is, but not link to any libraries.
include_directories( ### MIGRATE-NO-ACTION
  $<TARGET_GENEX_EVAL:CLHEP::CLHEP,$<TARGET_PROPERTY:CLHEP::CLHEP,INTERFACE_INCLUDE_DIRECTORIES>>
)

cet_build_plugin(AnalyzerEdepTrk art::EDAnalyzer NO_INSTALL
  LIBRARIES PRIVATE
  artg4tk::G4PhysModelParamStudy_AnalysisBase
  artg4tk::DataProducts_G4DetectorHits
  artg4tk::DataProducts_EventGenerators
  artg4tk::pluginDetectors_gdml
  art_root_io::TFileService_service
  ROOT::Hist
)

cet_build_plugin(AnalyzerHARP art::EDAnalyzer NO_INSTALL
  LIBRARIES PRIVATE
  artg4tk::G4PhysModelParamStudy_StatAnalysisTools
  artg4tk::G4PhysModelParamStudy_AnalysisBase
  artg4tk::G4PhysModelParamStudy_AnalysisProducts
  artg4tk::DataProducts_G4DetectorHits
  art_root_io::TFileService_service
  ROOT::Hist
)

cet_build_plugin(AnalyzerIAEA art::EDAnalyzer NO_INSTALL
  LIBRARIES PRIVATE
  artg4tk::G4PhysModelParamStudy_StatAnalysisTools
  artg4tk::G4PhysModelParamStudy_AnalysisBase
  artg4tk::G4PhysModelParamStudy_AnalysisProducts
  artg4tk::DataProducts_G4DetectorHits
  art_root_io::TFileService_service
  ROOT::Hist
)

cet_build_plugin(AnalyzerITEP art::EDAnalyzer NO_INSTALL
  LIBRARIES PRIVATE
  artg4tk::G4PhysModelParamStudy_StatAnalysisTools
  artg4tk::G4PhysModelParamStudy_AnalysisBase
  artg4tk::G4PhysModelParamStudy_AnalysisProducts
  artg4tk::DataProducts_G4DetectorHits
  art_root_io::TFileService_service
  ROOT::Hist
)

cet_build_plugin(AnalyzerNA49 art::EDAnalyzer NO_INSTALL
  LIBRARIES PRIVATE
  artg4tk::G4PhysModelParamStudy_StatAnalysisTools
  artg4tk::G4PhysModelParamStudy_AnalysisBase
  artg4tk::G4PhysModelParamStudy_AnalysisProducts
  artg4tk::DataProducts_G4DetectorHits
  art_root_io::TFileService_service
  ROOT::Hist
)

cet_build_plugin(AnalyzerSASM6E art::EDAnalyzer NO_INSTALL
  LIBRARIES PRIVATE
  artg4tk::G4PhysModelParamStudy_StatAnalysisTools
  artg4tk::G4PhysModelParamStudy_AnalysisBase
  artg4tk::G4PhysModelParamStudy_AnalysisProducts
  artg4tk::DataProducts_G4DetectorHits
  art_root_io::TFileService_service
  ROOT::Hist
)
