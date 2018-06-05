# myKiller directory CMakeLists.txt

#art_make(SERVICE_LIBRARIES  "${XERCESCLIB}"  ${MF_MESSAGELOGGER}  "${ACTIONHOLDERSERVICE}" "${G4_LIB_LIST}"  ${allG4Libs})
art_make(SERVICE_LIBRARIES
  "${XERCESCLIB}"
   ${ART_PERSISTENCY_CLHEPDICTIONARIES_DICT}
   ${MF_MESSAGELOGGER}
  "artg4tk_services_ActionHolder_service"
  "artg4tk_actionBase"
  "${G4_LIB_LIST}"
 )
