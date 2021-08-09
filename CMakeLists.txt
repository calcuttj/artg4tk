
cmake_minimum_required (VERSION 3.12...3.18 FATAL_ERROR)

find_package(cetbuildtools REQUIRED)
project(artg4tk)

list(APPEND CMAKE_MODULE_PATH $ENV{CANVAS_ROOT_IO_DIR}/Modules $ENV{ART_DIR}/Modules)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS WERROR NO_UNDEFINED EXTRA_FLAGS -pedantic)

#if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined,--no-allow-shlib-undefined,--as-needed")
#set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULELINKER_FLAGS} -Wl,--no-undefined,--no-allow-shlib-undefined,--as-needed")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined,--no-allow-shlib-undefined,--as-needed")
#endif()

cet_report_compiler_flags()

find_ups_product(art)
find_ups_product(art_root_io)
find_ups_boost()
find_ups_product(canvas)
find_ups_product(canvas_root_io)
find_ups_product(cetlib)
find_ups_product(cetlib_except)
find_ups_product(clhep)
find_ups_product(hep_concurrency)
find_ups_product(fhiclcpp)
find_ups_geant4()
find_ups_product(messagefacility)

include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

add_subdirectory(artg4tk)
add_subdirectory(examples)
add_subdirectory(fcl)
add_subdirectory(ups)

# packaging utility - must be included last
include(UseCPack)
