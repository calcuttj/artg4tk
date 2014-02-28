# Core directory

# Contains the action objects, run manager, physics list, and generic
# detector construction class.

if (NOT G4VISHEPREP)
  cet_find_library( G4VISHEPREP  NAMES G4visHepRep PATHS ENV G4LIB )
  set(G4_LIB_LIST ${G4_LIB_LIST} ${G4VISHEPREP})
endif()


# Create a library called "artg4tk_core"
art_make(MODULE_LIBRARIES "artg4tk_geantInit" "${XERCESCLIB}" "${G4_LIB_LIST}" )

# Install header files into the products area
install_headers() 
