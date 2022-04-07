find_package(CURL REQUIRED)

cet_make_library(NO_INSTALL
  SOURCE
  JSON2Data.cc
  VDBConnect.cc
  LIBRARIES
  PUBLIC
  ROOT::Hist
  CURL::libcurl
  PRIVATE
  messagefacility::MF_MessageLogger
)
