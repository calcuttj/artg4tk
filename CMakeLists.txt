########################################################################
# ArtG4tk
#
# NOTE: Build may be steered for internal development purposes by
#       setting the following CMake variables in cache (e.g. with cmake
#       -DXXXX:BOOL=ON):
#
#       * artg4tk_ExpDataAccess
#
#         Build the ExpDataAccess internal library.
#
#       * artg4tk_G4PhysModelParamStudy
#
#         Build the G4PhysModelParamStudy internal code set (implies
#         artg4tk_ExpDataAccess).
#
#       * artg4tk_INTERNAL
#
#         Build all internal code (including RooMUTools if
#         ENV{ROOMU_SYS} is defined and valid).
#
# Updated for cetmodules 3.10.00 and bitrot in internal code resolved -
# as far as possible without tests - by CG 2022-04-06.
########################################################################
cmake_minimum_required (VERSION 3.20 FATAL_ERROR)

find_package(cetmodules 3.10.00 REQUIRED) # For FindGeant4.cmake
project(artg4tk VERSION 10.05.00 LANGUAGES CXX)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS WERROR NO_UNDEFINED EXTRA_FLAGS -pedantic)

cet_report_compiler_flags(REPORT_THRESHOLD VERBOSE)

find_package(CLHEP COMPONENTS Vector REQUIRED EXPORT)
find_package(Geant4 REQUIRED COMPONENTS gdml EXPORT)
find_package(art_root_io REQUIRED EXPORT)
find_package(art REQUIRED EXPORT)
find_package(canvas_root_io REQUIRED)
find_package(canvas REQUIRED EXPORT)
find_package(messagefacility REQUIRED EXPORT)
find_package(fhiclcpp REQUIRED EXPORT)
find_package(cetlib_except REQUIRED EXPORT)
find_package(Range-v3 REQUIRED EXPORT)
find_package(ROOT COMPONENTS Core Hist Tree REQUIRED EXPORT)

cet_cmake_module_directories(Modules BINARY)

add_subdirectory(artg4tk)
add_subdirectory(examples)
add_subdirectory(fcl)

cet_cmake_config()
