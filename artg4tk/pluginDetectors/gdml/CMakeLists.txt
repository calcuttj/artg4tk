set_source_files_properties(GDMLDetector_service.cc ColorReader.cc PROPERTIES COMPILE_FLAGS -I$ENV{G4INCLUDE}/Geant4 )

art_make(LIB_LIBRARIES
  artg4tk_DataProducts_G4DetectorHits
  artg4tk_services_DetectorHolder_service
  ${G4_LIB_LIST}
  ${CLHEP}
         EXCLUDE GDMLDetector_service.cc)

simple_plugin(GDMLDetector "service"
  artg4tk_pluginDetectors_gdml
  ${XERCESC}
  ${ART_PERSISTENCY_PROVENANCE}
  ${ART_FRAMEWORK_PRINCIPAL}
  ${MF_MESSAGELOGGER})

install_headers()
