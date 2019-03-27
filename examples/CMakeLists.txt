file(GLOB fcl_files *.fcl *.gdml *.xml *.xsd *.C *.mac)

install(
  FILES ${fcl_files}
  DESTINATION ${product}/${version}/${PROJECT_NAME}/examples
)

foreach(aFile ${fcl_files})
  get_filename_component(basename ${aFile} NAME)
  configure_file(${aFile} ${CMAKE_BINARY_DIR}/examples/${basename} COPYONLY)
endforeach(aFile)
