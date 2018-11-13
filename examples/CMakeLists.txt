# testGeometry/ fcl cmakelists

# install all *.fcl *.gdml *.xml *.xsd files in this directory
file(GLOB fcl_files *.fcl *.gdml *.xml *.xsd *.C)
install( FILES ${fcl_files}
         DESTINATION ${product}/${version}/${PROJECT_NAME}/examples )

# Also install in the build area
foreach(aFile ${fcl_files})
  get_filename_component( basename ${aFile} NAME )
  configure_file( ${aFile} ${CMAKE_BINARY_DIR}/examples/${basename} COPYONLY )
endforeach(aFile)
