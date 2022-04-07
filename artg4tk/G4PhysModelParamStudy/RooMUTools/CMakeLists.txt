cet_build_plugin(RooMUHistSummary art::EDAnalyzer NO_INSTALL
  LIBRARIES PRIVATE
  artg4tk_G4PhysModelParamStudy_AnalysisBase
  artg4tk_G4PhysModelParamStudy_AnalysisProducts
  art_root_io::TFileService_service
  ${ROOMULIB}
  ROOT::Hist
  ROOT::RIO
  ROOT::Core
)

target_include_directories(artg4tk_G4PhysModelParamStudy_RooMUTools_RooMUHistSummary_module
  PRIVATE $ENV{ROOMU_SYS})
