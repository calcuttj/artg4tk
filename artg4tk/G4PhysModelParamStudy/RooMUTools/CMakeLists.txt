#
find_library( ROOMULIB NAMES plotutils PATHS $ENV{ROOMU_SYS}/lib NO_DEFAULT_PATH )
## ---> MESSAGE( STATUS "ROOMULIB: ${ROOMULIB}" )
#
include_directories($ENV{ROOMU_SYS})
#
art_make( MODULE_LIBRARIES "${ROOMULIB}" )

# install_headers()
