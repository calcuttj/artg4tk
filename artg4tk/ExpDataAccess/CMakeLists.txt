# services CMakeLists
#
# FindCURL defines CURL_INCLUDE_DIRS and CURL_LIBRARIES
# might need to suggest path if on tev with curl in /usr/local/Anaconda3-5.0.1
# find_package(CURL HINTS /usr/local/Anaconda3-5.0.1)
find_package(CURL)
if(CURL_FOUND)
  include_directories(${CURL_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "Could not find curl")
endif()

art_make(LIB_LIBRARIES ${CURL_LIBRARIES} ${ROOT_CORE} ${ROOT_HIST} ${MF_MESSAGELOGGER})

# install_headers()
