# ======================================================================
#  artg4tk main build file
#
#  . mrb slp   # Run if you have local products to set up
#  . mrb s     # Needed to set up local development products
#  mrb b       # Build
#  mrb t       # Test (if applicable)
#  mrb i       # Install to local products
#  gm2 -c <fhicl_file>  # Run!
#
# ======================================================================

# use cmake 2.8 or later
cmake_minimum_required (VERSION 2.8)

project(artg4tk)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()

set(CMAKE_MODULE_PATH $ENV{ART_DIR}/Modules
$ENV{CETBUILDTOOLS_DIR}/Modules
${CMAKE_MODULE_PATH})

include(CetCMakeEnv)
cet_cmake_env()

set_install_root()

# g-2 wants the product name in the library name
set (ART_MAKE_PREPEND_PRODUCT_NAME TRUE)

if ( ${APPLE} )
cet_set_compiler_flags(DIAGS CAUTIOUS
WERROR
EXTRA_FLAGS -pedantic

)
else()
cet_set_compiler_flags(DIAGS CAUTIOUS
WERROR
EXTRA_FLAGS -pedantic

)
endif()

cet_report_compiler_flags()

# Locate some UPS areas (the versions are just minimum required) and setup include paths
# These are needed for Art stuff

find_ups_product(cetbuildtools v4__12_05)
find_ups_geant4(v4_10_1_p02)
find_ups_product(xerces_c v3_1_1c)
find_ups_product(art v1_15_01)
find_ups_product(fhiclcpp v3_06_01)
find_ups_product(messagefacility v1_14_07)

# Enable visualization using OpenGL in x windows
add_definitions(-DG4_STORE_TRAJECTORY -DG4VERBOSE -DG4UI_USE -DG4VIS_USE -DG4UI_USE_TCSH -DG4INTY_USE_XT -DG4VIS_USE_OPENGLX -DG4VIS_USE_OPENGL)

# We need xerces_c for a geant library
#find_library( XERCESCLIB  NAMES xerces-c PATHS $ENV{XERCESCROOT}/lib NO_DEFAULT_PATH )

# macros for dictionary and simple_plugin
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

# Build these directories
add_subdirectory( geantInit )
add_subdirectory( actionBase )
add_subdirectory( Core )
add_subdirectory( services )
add_subdirectory( pluginActions )
add_subdirectory( pluginDetectors )
add_subdirectory( DataProducts )
add_subdirectory( EventGenerators )
add_subdirectory( Analysis )
add_subdirectory( util )
#
# UNcomment to activate G4 model param study tools
#
# ---> add_subdirectory( G4PhysModelParamStudy )

# ups - table and config files

add_subdirectory(ups)


# packaging utility

include(UseCPack)






