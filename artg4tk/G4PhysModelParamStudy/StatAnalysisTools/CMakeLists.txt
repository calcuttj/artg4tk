find_library(ROOMULIB NAMES plotutils PATHS $ENV{ROOMU_SYS}/lib NO_DEFAULT_PATH)

include_directories($ENV{ROOMU_SYS})

art_make(
  LIB_LIBRARIES
    ${ROOMULIB}
)
