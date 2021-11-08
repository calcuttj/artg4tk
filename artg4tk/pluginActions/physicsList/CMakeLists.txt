cet_build_plugin(PhysicsList art::service
 LIBRARIES PRIVATE
    artg4tk::services_PhysicsListHolder_service
    artg4tk::lists
    ${G4PHYSICSLISTS}
    ${G4RUN}
)

install_headers()
