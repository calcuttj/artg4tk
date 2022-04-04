### MIGRATE-NO-ACTION
find_library(ROOMULIB NAMES plotutils PATHS $ENV{ROOMU_SYS}/lib NO_DEFAULT_PATH) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.09.03) - prefer find_package() with custom Find<pkg>.cmake or cet_find_library() with ENV <x> for transitivity, relocatability

include_directories($ENV{ROOMU_SYS}) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.09.03) - avoid directory-scope functions: use target_link_libraries() with target semantics or target_include_directories() whenever possible

art_make(
  MODULE_LIBRARIES
    art_root_io_TFileService_service
    artg4tk_G4PhysModelParamStudy_AnalysisBase
    ${ROOMULIB}
)
