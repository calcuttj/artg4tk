file(GLOB mac_files *.mac)

install(
  FILES ${mac_files}
  DESTINATION
${PROJECT_NAME}/macros )

foreach(aFile ${mac_files})
  get_filename_component(basename ${aFile} NAME)
  configure_file(${aFile} ${CMAKE_BINARY_DIR}/macros/${basename} COPYONLY)
endforeach(aFile)
