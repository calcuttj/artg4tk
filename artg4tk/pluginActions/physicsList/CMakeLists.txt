art_make(
  SERVICE_LIBRARIES
    art_Framework_Services_Registry
    artg4tk_services_PhysicsListHolder_service
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    canvas
    cetlib_except
    clhep
    fhiclcpp
    ${G4PHYSICSLISTS}
    ${G4RUN}
)

install_headers()
