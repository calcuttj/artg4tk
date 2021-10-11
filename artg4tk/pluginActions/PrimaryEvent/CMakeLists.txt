cet_build_plugin(PrimaryEventAction art::service
  LIBRARIES
    PUBLIC
      messagefacility::MF_MessageLogger
    PRIVATE
      art::Framework_Principal
      artg4tk_actionBase
      artg4tk_services_ActionHolder_service
      Geant4::G4event
)
