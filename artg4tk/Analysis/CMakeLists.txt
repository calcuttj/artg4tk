set(FRAMEWORK_MODULES
  CheckCalorimeterHits
  CheckDRCalorimeterHits
  CheckGenParticle
  CheckHits
  CheckInteractions
  CheckPhotonHits
  CheckProtonProduction
  CheckTrackerHits)

foreach(MODULE_NAME IN LISTS FRAMEWORK_MODULES)
  cet_build_plugin(${MODULE_NAME} art::module
    LIBRARIES PRIVATE
    art::Framework_Services_Registry
    art_root_io::TFileService_service
    artg4tk::DataProducts_EventGenerators
    ROOT::Hist
    ROOT::Core
    ROOT::Tree
    )
endforeach()

install_source()
