#  This file is the main CMakeLists.txt file for the @artg4tk@ package. It is used by the g-2 build system.
# h2. Introduction 

# h2. Main code

# Ensure we are using a moden version of CMake
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# h3. Project definition

# --> Define the name of this project (replace @gm2artexamples@ with your 
# project name - use all lowercase)
PROJECT (artg4tk)

# h3. Load build system modules
# cetbuildtools contains our cmake modules
set( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
if( NOT CETBUILDTOOLS_VERSION )
  message( FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules" )
endif()
#set( CMAKE_MODULE_PATH $ENV{CETBUILDTOOLS_DIR}/Modules ${CMAKE_MODULE_PATH} )
set(CMAKE_MODULE_PATH $ENV{ART_DIR}/Modules
		      $ENV{CETBUILDTOOLS_DIR}/Modules
		      ${CMAKE_MODULE_PATH})

# Add any modules from this source necessary for building
#set(CMAKE_MODULE_PATH $ENV{GM2SOFTWARETOOLS_DIR}/Modules ${CMAKE_MODULE_PATH} )


# Include the necessary macros
include(CetCMakeEnv)
include(BuildPlugins)

include(FindUpsGeant4)

# h3. Configure this product

# Configure
cet_cmake_env()

# Do the same compiler flag stuff as Art
cet_set_compiler_flags(DIAGS VIGILANT WERROR 
   EXTRA_FLAGS -pedantic
   EXTRA_CXX_FLAGS -std=c++11
)
# ADD NO_UNDEFINED to ensure closed link

cet_have_qual( sse2 SSE2 )
if ( SSE2 )
   cet_add_compiler_flags(CXX -msse2 -ftree-vectorizer-verbose=2 -verbose=1)
endif()			      

cet_report_compiler_flags()


# Locate some UPS areas (the versions are just minimum required) and setup include paths
# These are needed for Art stuff

find_ups_product(cetbuildtools v3_06_02)
find_ups_product(xerces_c v3_1_1)
find_ups_geant4( v4_9_6_p02 )
find_ups_product(cpp0x v1_03_23)
find_ups_product(cppunit v1_12_1)
find_ups_product(clhep v2_1_3_1)
find_ups_product(cetlib v1_03_23)
find_ups_product(gccxml v0_9_20130621)
find_ups_product(art v1_08_04)
find_ups_product(fhiclcpp v2_17_10)
find_ups_product(messagefacility v1_10_24)
find_ups_root( v5_34_09 )
find_ups_boost( v1_53_0 )

# Enable visualization using OpenGL in x windows
add_definitions(-DG4_STORE_TRAJECTORY -DG4VERBOSE -DG4UI_USE -DG4VIS_USE -DG4UI_USE_TCSH -DG4INTY_USE_XT -DG4VIS_USE_OPENGLX -DG4VIS_USE_OPENGL)

# Make sure we have gcc
cet_check_gcc()


# We need xerces_c for a geant library
find_library( XERCESCLIB  NAMES xerces-c PATHS $ENV{XERCESCROOT}/lib NO_DEFAULT_PATH )

# Macros for art_make and simple plugins

include(ArtDictionary)
include(ArtMake)

#message(STATUS "-----> All Geant4 is ${G4_LIB_LIST}")

# Build these directories
add_subdirectory( geantInit )
add_subdirectory( actionBase )
add_subdirectory( Core  )
add_subdirectory( services )
add_subdirectory( pluginActions )
add_subdirectory( pluginDetectors )
add_subdirectory( util )
add_subdirectory( fcl )
add_subdirectory( ups )
add_subdirectory( Analysis )
add_subdirectory( EventGenerators )
add_subdirectory( DataProducts )



# Packaging facility
include(UseCPack)
