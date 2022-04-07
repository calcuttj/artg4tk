cet_make_library(NO_INSTALL
  LIBRARY_NAME_VAR dict_name
  SOURCE
  BeamThinTargetConfig.cc
  RecordChi2.cc
  LIBRARIES
  PUBLIC
  ROOT::Core
  PRIVATE
  canvas::canvas
)

# Create an alias to allow build_dictionary() to obtain information
# about the library into which the object compiled from the generated
# .cpp will be put.
add_library(${dict_name}_dict ALIAS ${dict_name})

# Generate the dictionary source without generating a shared library.
build_dictionary(${dict_name} NO_LIBRARY NO_INSTALL)

# Add the generated dictionary source to the source list of the main
# library for this directory.
target_sources(${dict_name}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/${dict_name}_dict.cpp)
