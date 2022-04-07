cet_build_plugin(ModelParamStudyProducer art::EDProducer NO_INSTALL
  LIBRARIES PRIVATE
  artg4tk::G4PhysModelParamStudy_G4Services_PhysModelConfig_service
  artg4tk::geantInit
  artg4tk::services_DetectorHolder_service
  artg4tk::services_PhysicsListHolder_service
  artg4tk::DataProducts_G4ModelConfig
  artg4tk::DataProducts_EventGenerators
  art::Framework_Services_Registry
  messagefacility::MF_MessageLogger
  Geant4::G4run
  Geant4::G4persistency
  Geant4::G4digits_hits
  Geant4::G4geometry
  Geant4::G4global
)
  
cet_build_plugin(ProcLevelMPVaryProducer art::EDProducer NO_INSTALL
  LIBRARIES PRIVATE
  artg4tk::G4PhysModelParamStudy_G4Services_PhysModelConfig_service
  artg4tk::G4PhysModelParamStudy_G4Services_ProcLevelSimSetup_service
  artg4tk::G4PhysModelParamStudy_G4Components
  artg4tk::DataProducts_G4DetectorHits
  artg4tk::DataProducts_G4ModelConfig
  artg4tk::DataProducts_EventGenerators
  art::Framework_Services_Registry
  messagefacility::MF_MessageLogger
  Geant4::G4tracking
  Geant4::G4track
  Geant4::G4particles
  Geant4::G4materials
  )
