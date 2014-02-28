# CMakeLists.txt file for scripts

# install all scripts files in this directory
file(GLOB scripts_files *.sh *template)
install( FILES ${scripts_files}
         DESTINATION ${product}/${version}/${PROJECT_NAME}/scripts )

# Also install in the build area
foreach(aFile ${scripts_files})
  get_filename_component( basename ${aFile} NAME )
  configure_file( ${aFile} ${CMAKE_BINARY_DIR}/scripts/${basename} COPYONLY )
endforeach(aFile)
