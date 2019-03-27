find_library(ROOMULIB NAMES plotutils PATHS $ENV{ROOMU_SYS}/lib NO_DEFAULT_PATH)

include_directories($ENV{ROOMU_SYS})

art_make(
  MODULE_LIBRARIES
    art_Framework_Services_Optional_TFileService_service
    artg4tk_G4PhysModelParamStudy_AnalysisBase
    ${ROOMULIB}
)
