find_package(CURL)
if(CURL_FOUND)
  include_directories(${CURL_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "Could not find curl")
endif()

art_make(
  LIB_LIBRARIES
    ${CURL_LIBRARIES}
    MF_MessageLogger
)
