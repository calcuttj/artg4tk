art_make(
  LIB_LIBRARIES
    art_Framework_Services_Registry
    artg4tk_services_ActionHolder_service
    artg4tk_services_DetectorHolder_service
    ${Boost_SYSTEM_LIBRARY}
    canvas
    cetlib_except
    clhep
    ${G4EVENT}
    ${G4GLOBAL}
    ${G4INTERCOMS}
    ${G4RUN}
    ${G4TRACKING}
)

install_headers()
