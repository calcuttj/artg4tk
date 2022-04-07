find_package(ROOT COMPONENTS Core Gpad Hist RIO REQUIRED)
find_package(CLHEP COMPONENTS Random REQUIRED)

add_subdirectory(AnalysisProducts)
add_subdirectory(AnalysisBase)
add_subdirectory(AnalysisModules)
add_subdirectory(StatAnalysisTools)
add_subdirectory(G4Components)
add_subdirectory(G4Services)
add_subdirectory(ApplicationModules)

find_library(ROOMULIB NAMES plotutils PATHS ENV ROOMU_SYS NO_DEFAULT_PATH)
mark_as_advanced(ROOMULIB)

if (ROOMULIB)
  add_subdirectory(RooMUTools)
endif()
