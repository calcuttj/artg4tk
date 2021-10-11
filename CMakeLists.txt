cmake_minimum_required (VERSION 3.19 FATAL_ERROR)

find_package(cetmodules REQUIRED)
project(artg4tk VERSION 10.03.00 LANGUAGES CXX)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS WERROR NO_UNDEFINED EXTRA_FLAGS -pedantic)

cet_report_compiler_flags()

find_package(art REQUIRED)
find_package(art_root_io REQUIRED)
find_package(Boost REQUIRED)
find_package(canvas REQUIRED)
find_package(cetlib REQUIRED)
find_package(cetlib_except REQUIRED)
find_package(CLHEP REQUIRED)
find_package(fhiclcpp REQUIRED)
find_package(Geant4 REQUIRED COMPONENTS gdml)
find_package(messagefacility REQUIRED)
find_package(Range REQUIRED)

add_subdirectory(artg4tk)
add_subdirectory(examples)
add_subdirectory(fcl)

cet_cmake_config()
