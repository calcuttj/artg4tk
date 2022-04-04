art_make(
  LIB_LIBRARIES
    art::Utilities
    Geant4::G4physicslists
  SERVICE_LIBRARIES
    art::Framework_Core
) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.09.03) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_source()
