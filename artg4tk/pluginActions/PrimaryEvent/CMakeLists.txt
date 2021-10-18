cet_build_plugin(PrimaryEventAction art::service
  LIBRARIES
    PUBLIC
      messagefacility::MF_MessageLogger
      artg4tk::actionBase
    PRIVATE
      art::Framework_Principal
      artg4tk::services_ActionHolder_service
      Geant4::G4event
)
