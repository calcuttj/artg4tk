art_make(
  SERVICE_LIBRARIES
    art_Framework_Core
    art_Persistency_Provenance
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    canvas
    cetlib_except
    fhiclcpp
    MF_MessageLogger
)

install_headers()
