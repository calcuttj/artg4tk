art_make(
  LIB_LIBRARIES
    artg4tk_services_ActionHolder_service
    artg4tk_services_DetectorHolder_service
    canvas
    cetlib_except
    ${CLHEP}
    ${G4EVENT}
    ${G4GLOBAL}
    ${G4INTERCOMS}
    ${G4RUN}
    ${G4TRACKING}
)

install_headers()
