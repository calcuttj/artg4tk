cet_make_library(SOURCE
  ArCaptureGammas.cc
  ArParticleHPCapture.cc
  ArParticleHPCaptureFS.cc
  MyG4BiasedBertiniNeutronBuilder.cc
  MyG4HadronPhysicsQGSP_BERT_ArHP.cc
  MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias.cc
  MyG4NeutronPHPBuilder.cc
  MyQGSP_BERT_ArHP.cc
  MyQGSP_BERT_HP_NeutronXSBias.cc
  LIBRARIES PUBLIC
  Geant4::G4physicslists
)

install_headers()
install_source()
