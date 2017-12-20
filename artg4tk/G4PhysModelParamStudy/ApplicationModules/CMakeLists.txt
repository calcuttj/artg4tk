# services CMakeLists

# NOTE(JVY): as of art v1_15_01, the TFileService stuff needs to be linked explicitly
#            it was NOT the case in the v1_12-series, and I do NOT know why the hell this change !...
#            need to talk to M.P. when he's back from "art school"
# 

set_source_files_properties(ModelParamStudyProducer_module.cc PROPERTIES COMPILE_FLAGS -I$ENV{G4INCLUDE}/Geant4)

art_make( MODULE_LIBRARIES  
	  "artg4tk_G4PhysModelParamStudy_G4Components"
	   "artg4tk_geantInit"
	   "artg4tk_pluginDetectors_gdml"
	  "${G4_LIB_LIST}" 
	)

#install_headers()
