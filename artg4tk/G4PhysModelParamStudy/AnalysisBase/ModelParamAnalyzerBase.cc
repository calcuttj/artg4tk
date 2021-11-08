
#include "artg4tk/G4PhysModelParamStudy/AnalysisBase/ModelParamAnalyzerBase.hh"

// G4-specific headers
#include "Geant4/G4DynamicParticle.hh"
#include "Geant4/G4HadronCrossSections.hh"
#include "Geant4/G4Material.hh"
#include "Geant4/G4NistManager.hh"
#include "Geant4/G4ParticleTable.hh"

// Root-specific headers
#include "TObjString.h"

#include "CLHEP/Units/SystemOfUnits.h"

#include "art/Framework/Principal/Event.h"
//
#include "art_root_io/TFileService.h"
//
// Run/Eevent data products
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkVtx.hh" // Event data product
// Incoming Event data products
#include "artg4tk/DataProducts/EventGenerators/GenParticle.hh"
#include "artg4tk/DataProducts/EventGenerators/GenParticleCollection.hh"

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

artg4tk::ModelParamAnalyzerBase::ModelParamAnalyzerBase(const fhicl::ParameterSet& p)
  : artg4tk::AnalyzerWithExpDataBase(p)
  , // art::EDAnalyzer(p),
  fXSecInit(false)
{

  fProdLabel = p.get<std::string>("ProductLabel");

  fConsistentRunConditions = true;
}

artg4tk::ModelParamAnalyzerBase::~ModelParamAnalyzerBase()
{
  // do I need any of G4 and/or Root delete's here ?!
  // or will TFileService take care of that ?!?!
}

void
artg4tk::ModelParamAnalyzerBase::beginRun(const art::Run& r)
{

  // move particletable prep (if needed) to begin run in order to avoid conflicts
  // with standard Geant4 app initialization (due to internal details of core Geant4)
  //
  if (!(G4ParticleTable::GetParticleTable()->GetReadiness())) {
    prepareG4PTable();
  }

  art::Handle<ArtG4tkModelConfig> physcfg;
  r.getByLabel(fProdLabel, physcfg);
  if (!physcfg.isValid()) {
    fLogInfo << " handle to run product (model/physics config) is NOT valid"; // << std::endl;
    return;
  }

  art::ServiceHandle<art::TFileService> tfs;

  // NOTE-1(JVY): The TObjArray (of TObjString's) will be created in the right directory of the
  // TFile.
  //              However, giving it an explicit name here will NOT help the I/O - see another note
  //              later.
  //
  // NOTE-2(JVY): Can NOT call makeAndRegister<TObjArray> because
  //              TObjArray does NOT have a SetTitle(...) method
  //
  fModelConfig = tfs->make<TObjArray>();

  std::vector<std::string> cfgmodels = physcfg->GetConfiguredModels();
  for (unsigned int i = 0; i < cfgmodels.size(); ++i) {
    const std::vector<std::pair<std::string, double>>* mcfg = physcfg->GetConfig(cfgmodels[i]);
    for (unsigned int ii = 0; ii < mcfg->size(); ++ii) {
      std::string cfginfo = cfgmodels[i] + ": ";
      cfginfo += ((*mcfg)[ii].first + "=");
      std::ostringstream os;
      os << (*mcfg)[ii].second;
      cfginfo += os.str();
      fModelConfig->Add(new TObjString(cfginfo.c_str()));
    }
  }

  // NOTE-2(JVY): One must explicitly call the Write(...) method on the TObjArray (unlike e.g.
  // TH-whatever),
  //              and give theobject's name at this point, plus one should specify to use a "single
  //              key" option on write, because otherwise Root will dump it as a sequence of
  //              whatever objects one stores in the array, using the same name for each one, so
  //              accessing them will be a headache.
  //
  fModelConfig->Write("Geant4ModelConfig",
                      1); // NOTE(JVY): 2nd arg tells TObjArray to be written
                          //            with the use of a "single key", i.e. as an TObjArray,
                          //            not a sequence of TObjects (TObjString's)

  return;
}

void
artg4tk::ModelParamAnalyzerBase::endJob()
{

  if (!fConsistentRunConditions) {
    fLogInfo << " Run Conditions (beam id and/or momentum) are NOT consistent; no information will "
                "be written out ";
    return;
  }

  AnalyzerWithExpDataBase::endJob();

  art::ServiceHandle<art::TFileService> tfs;

  //   BeamThinTargetConfig* tconf =
  tfs->makeAndRegister<BeamThinTargetConfig>(
    "Beam_ThinTarget_Config", "Beam-Momentum-TargetNucleus", fBTConf);
  //   std::cout << " tconf : " << std::endl;
  //   tconf->Print();

  return;
}

void
artg4tk::ModelParamAnalyzerBase::initXSecOnTarget(const std::string& mname,
                                                  const artg4tk::ArtG4tkParticle& part)
{

  // NOTE/FIXME (JV): probably it's better to take the beam particle
  //                  directly from the generator record, via the event !!!

  std::string tmp = mname;

  //   if ( mname.find("G4_") == std::string::npos )
  if (mname == "LAr") {
    tmp = "G4_Ar";
  }

  if (tmp.find("G4_") == std::string::npos) {
    tmp = "G4_" + mname;
  }

  G4Material* mat = G4NistManager::Instance()->FindOrBuildMaterial(tmp);
  assert(mat);

  const G4Element* elm = mat->GetElement(0);

  int A = (int)(elm->GetN() + 0.5);
  int Z = (int)(elm->GetZ() + 0.5);
  assert(A);
  assert(Z);

  G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* g4pd = ptable->FindParticle(part.GetPDG());
  assert(g4pd);

  //   G4DynamicParticle g4pdyn( g4pd, (part.GetMomentum().vect())*CLHEP::GeV );
  G4DynamicParticle g4pdyn(g4pd, part.GetMomentum().vect());
  fXSecOnTarget = (G4HadronCrossSections::Instance())->GetInelasticCrossSection(&g4pdyn, Z, A);

  fXSecOnTarget /= CLHEP::millibarn;

  fXSecInit = true;

  return;
}

bool
artg4tk::ModelParamAnalyzerBase::ensureBeamTgtConfig(const art::Event& e)
{

  if (!fConsistentRunConditions) {
    fLogInfo << " Run Conditions (beam id and/or momentum) are NOT consistent ";
    return false;
  }

  art::Handle<GenParticleCollection> primgenparts;
  e.getByLabel("PrimaryGenerator", primgenparts);
  if (!primgenparts.isValid()) {
    fLogInfo << " Handle to Generator Particle(s) is NOT valid "; //  << std::endl;
    return false;
    ;
  }

  if (primgenparts->empty()) {
    fLogInfo << " Generator Particle(s) empty "; // << std::endl;
    return false;
  }

  if (primgenparts->size() > 1) {
    fLogInfo << " Multiple Generator Particles - only 1 (one) is allowed "; // << std::endl;
    return false;
  }

  GenParticleCollection::const_iterator i = primgenparts->begin();
  if (fBTConf.GetBeamPartID() == 0)
    fBTConf.SetBeamPartID(i->pdgId());
  if (fBTConf.GetBeamPartID() != i->pdgId())
    fConsistentRunConditions = false;
  if (fBTConf.GetBeamMomentum() < 0.)
    fBTConf.SetBeamMomentum(i->momentum().vect().mag());
  if (fabs(fBTConf.GetBeamMomentum() - i->momentum().vect().mag()) > 1.e-10)
    fConsistentRunConditions = false;

  art::Handle<ArtG4tkVtx> firstint;
  e.getByLabel(fProdLabel, firstint);
  if (!firstint.isValid()) {
    fLogInfo << " handle to 1st hadronic interaction is NOT valid"; // << std::endl;
    return false;
  }

  std::string tgtmatname = firstint->GetMaterialName();
  std::string tmp = tgtmatname;
  if (tgtmatname == "LAr") {
    tmp = "G4_Ar";
  }
  if (tmp.find("G4_") == std::string::npos) {
    tmp = "G4_" + tgtmatname;
  }
  // --->   G4Material* mat = G4NistManager::Instance()->FindOrBuildMaterial(tgtmatname.c_str());
  G4Material* mat = G4NistManager::Instance()->FindOrBuildMaterial(tmp.c_str());

  if (fBTConf.GetTargetID() == 0)
    fBTConf.SetTargetID((int)(mat->GetZ() + 0.5));
  if (fBTConf.GetTargetID() != (int)(mat->GetZ() + 0.5))
    fConsistentRunConditions = false;

  if (!fConsistentRunConditions) {
    fLogInfo << " Run Conditions (beam id and/or momentum) are NOT consistent ";
    return false;
  }

  return fConsistentRunConditions;
}

#include "Geant4/G4BaryonConstructor.hh"
#include "Geant4/G4BosonConstructor.hh"
#include "Geant4/G4GenericIon.hh"
#include "Geant4/G4IonConstructor.hh"
#include "Geant4/G4IonTable.hh"
#include "Geant4/G4LeptonConstructor.hh"
#include "Geant4/G4MesonConstructor.hh"
#include "Geant4/G4ProcessManager.hh"

void
artg4tk::ModelParamAnalyzerBase::prepareG4PTable()
{

  // physics needs to be initialized before the 1st use of particle table,
  // because it constructs particles - otherwise the table is just empty
  //
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  // This is needed because starting 9.6.ref05 G4IonTable::CreateIon(...)
  // explicitly checks if generic ion has a process manager
  //
  G4GenericIon* gion = G4GenericIon::GenericIon();
  gion->SetProcessManager(new G4ProcessManager(gion));
  //
  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4BosonConstructor pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();

  // NOTE(JVY): This does not do much - it only puts in 6 ions (incl.p).
  //            Other ions get created on the fly but apparently one needs
  //            to know A & Z, and that's how the code is generated...
  //
  G4IonTable* ionTable = partTable->GetIonTable();
  ionTable->CreateAllIon();
  ionTable->CreateAllIsomer();

  partTable->SetReadiness();

  return;
}
