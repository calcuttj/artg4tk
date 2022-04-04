include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

# Hack to ensure that we can type '#include "Geant4/G4XYZ.hh"' instead
# of '#include "G4XYZ.hh"'.
include_directories("${Geant4_INCLUDE_DIRS}/../") ### MIGRATE-ACTION-RECOMMENDED (migrate-3.09.03) - avoid directory-scope functions: use target_link_libraries() with target semantics or target_include_directories() whenever possible

add_subdirectory(actionBase)
add_subdirectory(Analysis)
add_subdirectory(Core)
add_subdirectory(DataProducts)
add_subdirectory(EventGenerators)
#add_subdirectory(ExpDataAccess)
#add_subdirectory(G4PhysModelParamStudy)
add_subdirectory(geantInit)
add_subdirectory(lists)
add_subdirectory(pluginActions)
add_subdirectory(pluginDetectors)
add_subdirectory(services)
add_subdirectory(util)
