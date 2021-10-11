// PhysicsListService

#include "artg4tk/pluginActions/physicsList/PhysicsList_service.hh"
#include "fhiclcpp/ParameterSet.h"

#include "Geant4/G4PhysListFactoryAlt.hh"
#include "Geant4/G4PhysListRegistry.hh"
#include "Geant4/G4PhysicsConstructorRegistry.hh"
#include "Geant4/G4VModularPhysicsList.hh"

// geant 4 physics constructors:
#include "Geant4/G4NeutronTrackingCut.hh"
#include "Geant4/G4OpticalPhysics.hh"
#include "Geant4/G4SystemOfUnits.hh"

#include <fstream>
#include <memory>

artg4tk::PhysicsListService::PhysicsListService(fhicl::ParameterSet const& p)
  : PhysicsListName_(p.get<std::string>("PhysicsListName", "FTFP_BERT"))
  , DumpList_(p.get<bool>("DumpList", false))
  , enableNeutronLimit_(p.get<bool>("enableNeutronLimit", true))
  , NeutronTimeLimit_(p.get<double>("NeutronTimeLimit", 10. * microsecond))
  , NeutronKinELimit_(p.get<double>("NeutronKinELimit", 0.0))
  , enableStepLimit_(p.get<bool>("enableStepLimit", true))
  , enableOptical_(p.get<bool>("enableOptical", true))
  , enableCerenkov_(p.get<bool>("enableCerenkov", false))
  , CerenkovStackPhotons_(p.get<bool>("CerenkovStackPhotons", false))
  , CerenkovMaxNumPhotons_(p.get<int>(" CerenkovMaxNumPhotons", 100))
  , CerenkovMaxBetaChange_(p.get<double>("CerenkovMaxBetaChange", 10.0))
  , CerenkovTrackSecondariesFirst_(p.get<bool>("CerenkovTrackSecondariesFirst", false))
  , enableScintillation_(p.get<bool>("enableScintillation", true))
  , ScintillationStackPhotons_(p.get<bool>("ScintillationStackPhotons", false))
  , ScintillationByParticleType_(p.get<bool>("ScintillationByParticleType", true))
  , ScintillationTrackInfo_(p.get<bool>("ScintillationTrackInfo", false))
  , ScintillationTrackSecondariesFirst_(p.get<bool>("ScintillationTrackSecondariesFirst", false))
  , enableAbsorption_(p.get<bool>("enableAbsorption", false))
  , enableRayleigh_(p.get<bool>("enableRayleigh", false))
  , enableMieHG_(p.get<bool>("enableMieHG", false))
  , enableBoundary_(p.get<bool>("enableBoundary", false))
  , enableWLS_(p.get<bool>("enableWLS", false))
  , BoundaryInvokeSD_(p.get<bool>("BoundaryInvokeSD", false))
  , verbositylevel_(p.get<int>("Verbosity", 0))
  , WLSProfile_(p.get<std::string>("WLSProfile", "delta"))
{}

G4VUserPhysicsList*
artg4tk::PhysicsListService::makePhysicsList()
{

  g4alt::G4PhysListFactory factory;
  // Access to registries and factories
  //
  G4PhysicsConstructorRegistry* g4pcr = G4PhysicsConstructorRegistry::Instance();
  G4PhysListRegistry* g4plr = G4PhysListRegistry::Instance();
  //
  // the following should be unneccessary at some point:
  //
  g4plr->AddPhysicsExtension("OPTICAL", "G4OpticalPhysics");
  g4plr->AddPhysicsExtension("STEPLIMIT", "G4StepLimiterPhysics");
  g4plr->AddPhysicsExtension("NEUTRONLIMIT", "G4NeutronTrackingCut");
  g4pcr->PrintAvailablePhysicsConstructors();
  g4plr->PrintAvailablePhysLists();
  G4VModularPhysicsList* phys = NULL;
  G4String physName = PhysicsListName_;
  if (enableOptical_)
    physName = physName + "+OPTICAL";
  if (enableStepLimit_)
    physName = physName + "+STEPLIMIT";
  if (enableNeutronLimit_)
    physName = physName + "+NEUTRONLIMIT";
  std::cout << " Name of Physics list: " << physName << std::endl;
  if (factory.IsReferencePhysList(physName)) {
    phys = factory.GetReferencePhysList(physName);
    phys->SetVerboseLevel(verbositylevel_);
  }

  if (enableOptical_) {
    G4OpticalPhysics* opticalPhysics = (G4OpticalPhysics*)phys->GetPhysics("Optical");
    opticalPhysics->Configure(kCerenkov, enableCerenkov_);
    opticalPhysics->SetCerenkovStackPhotons(CerenkovStackPhotons_);
    opticalPhysics->Configure(kScintillation, enableScintillation_);
    opticalPhysics->SetScintillationStackPhotons(ScintillationStackPhotons_);
    opticalPhysics->SetScintillationByParticleType(ScintillationByParticleType_);
    opticalPhysics->SetScintillationTrackInfo(ScintillationTrackInfo_);
    opticalPhysics->SetTrackSecondariesFirst(
      kCerenkov, true); // only relevant if we actually stack and trace the optical photons
    opticalPhysics->SetTrackSecondariesFirst(
      kScintillation, true); // only relevant if we actually stack and trace the optical photons
    opticalPhysics->SetMaxNumPhotonsPerStep(CerenkovMaxNumPhotons_);
    opticalPhysics->SetMaxBetaChangePerStep(CerenkovMaxBetaChange_);
    opticalPhysics->Configure(kAbsorption, enableAbsorption_);
    opticalPhysics->Configure(kRayleigh, enableRayleigh_);
    opticalPhysics->Configure(kMieHG, enableMieHG_);
    opticalPhysics->Configure(kBoundary, enableBoundary_);
    opticalPhysics->SetVerboseLevel(verbositylevel_);
    opticalPhysics->Configure(kWLS, enableWLS_);
  }
  if (enableNeutronLimit_) {
    G4NeutronTrackingCut* neutrcut = (G4NeutronTrackingCut*)phys->GetPhysics("neutronTrackingCut");
    neutrcut->SetTimeLimit(NeutronTimeLimit_);
  }
  if (DumpList_) {
    std::cout << phys->GetPhysicsTableDirectory() << std::endl;
    phys->DumpList();
    phys->DumpCutValuesTable();
  }
  return phys;
}
