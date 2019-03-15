# cmake driver file for @ups/@

#execute_process( COMMAND date
#                 OUTPUT_VARIABLE datime
#                 OUTPUT_STRIP_TRAILING_WHITESPACE )

# create package configuration and version files

process_ups_files()
cet_cmake_config()

#configure_file ( ${artg4tk_SOURCE_DIR}/ups/artg4tk-config.cmake.in
#                 ${artg4tk_BINARY_DIR}/ups/artg4tk-config.cmake @ONLY )
#configure_file ( ${artg4tk_SOURCE_DIR}/ups/artg4tk-config-version.cmake.in
#                 ${artg4tk_BINARY_DIR}/ups/artg4tk-config-version.cmake @ONLY )
#
#install (FILES ${artg4tk_BINARY_DIR}/ups/artg4tk-config.cmake
#               ${artg4tk_BINARY_DIR}/ups/artg4tk-config-version.cmake
#	 DESTINATION ${flavorqual_dir}/lib/artg4tk-${version} )
#
## Add setup line to source_for_local.sh in local products area (no qualifier needed here)
#install(CODE "file(APPEND ${CMAKE_INSTALL_PREFIX}/source_for_local.sh \"\nsetup -j ${product} ${version} -q \\\"${full_qualifier}\\\"\")")

