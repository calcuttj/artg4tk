# services CMakeLists

# NOTE-1(JVY): as of art v1_15_01, the TFileService stuff needs to be linked explicitly
#            it was NOT the case in the v1_12-series, and I do NOT know why the hell this change !...
#            need to talk to M.P. about that
#
# NOTE-2(JVY): I don't think I need "${XERCESCLIB}" at this point; but if so, just include it...
#

find_program(ROOT_CONFIG_EXECUTABLE root-config PATHS ${ROOTSYS}/bin $ENV{ROOTSYS}/bin)
execute_process(
    COMMAND ${ROOT_CONFIG_EXECUTABLE} --libs 
    OUTPUT_VARIABLE ROOT_LIBRARIES 
    OUTPUT_STRIP_TRAILING_WHITESPACE)

art_make( LIB_LIBRARIES  
          art_Framework_Services_Optional_TFileService_service
	  "artg4tk_G4PhysModelParamStudy_AnalysisProducts" 
	  "artg4tk_G4PhysModelParamStudy_AnalysisProducts_dict" 
	  "artg4tk_ExpDataAccess"
	  "${G4_LIB_LIST}"
	  "${ROOT_LIBRARIES}" )

# install_headers()
