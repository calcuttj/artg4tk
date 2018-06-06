# ======================================================================
#  artg4tk main build file
#
#  . mrb slp   # Run if you have local products to set up
#  . mrbsetenv     # Needed to set up local development products
#  mrb b       # Build
#  mrb t       # Test (if applicable)
#  mrb i       # Install to local products
#  art -c <fhicl_file>  # Run!
#
# ======================================================================

# use cmake 3.7 or later
cmake_minimum_required (VERSION 3.7)

project(artg4tk)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()

set(CMAKE_MODULE_PATH $ENV{CANVAS_DIR}/Modules $ENV{CANVAS_DIR}/Modules
$ENV{CETBUILDTOOLS_DIR}/Modules
${CMAKE_MODULE_PATH})

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS
WERROR
EXTRA_FLAGS -pedantic
)

cet_report_compiler_flags()

# Locate some UPS areas and setup include paths
# These are needed for Art stuff

find_ups_product(cetbuildtools)
#
find_ups_product(xerces_c)
find_ups_product(art)
find_ups_product(canvas)
find_ups_product(fhiclcpp)
find_ups_product(messagefacility)
find_ups_product(larcoreobj)
find_ups_geant4()

# macros for dictionary and simple_plugin
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

# Build these directories
add_subdirectory( artg4tk )
add_subdirectory( fcl )
add_subdirectory( gdml )


# ups - table and config files

add_subdirectory(ups)


# packaging utility

include(UseCPack)






