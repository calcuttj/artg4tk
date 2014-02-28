# Clock CMakeLists.txt ABC

art_make( SERVICE_LIBRARIES 
	  artg4tk_services_ActionHolder_service 
	  ${XERCESCLIB} 
	  ${G4_LIB_LIST}
	)

install_headers()
