# services CMakeLists

# find_library( FHICLLIB NAMES fhiclcpp PATHS $ENV{FHICLCPP_LIB} NO_DEFAULT_PATH )

# art_make( LIB_LIBRARIES  "${XERCESCLIB}" "${G4_LIB_LIST}" "${FHICLLIB}")
art_make( LIB_LIBRARIES  "${XERCESCLIB}" "${G4_LIB_LIST}" )

# install_headers()
