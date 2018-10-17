# Core directory

# Contains the action objects, run manager, physics list, and generic
# detector construction class.

# Create a library called "artg4tk_core"
art_make(MODULE_LIBRARIES artg4tk_geantInit artg4tk_pluginDetectors_gdml artg4tk_services_PhysicsListHolder_service ${G4_LIB_LIST})

install_headers() 
