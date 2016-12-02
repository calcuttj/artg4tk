void rootlogon()
{

gSystem->Load("libCintex.so");
Cintex::Enable();
gSystem->Load("libartg4tk_G4PhysModelParamStudy_AnalysisProducts_dict.so");
// gSystem->Load("libartg4tk_G4PhysModelParamStudy_AnalysisProducts_map.so");
gSystem->Load("libartg4tk_G4PhysModelParamStudy_AnalysisProducts.so");

}
