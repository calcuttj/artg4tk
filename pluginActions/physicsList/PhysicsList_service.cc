// PhysicsListService

#include "artg4tk/pluginActions/physicsList/PhysicsList_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "Geant4/G4PhysListFactory.hh"
#include "Geant4/G4OpticalPhysics.hh"
#include "Geant4/G4VModularPhysicsList.hh"
#include <fstream>

#include <memory>

artg4tk::PhysicsListService::PhysicsListService(fhicl::ParameterSet const & p, art::ActivityRegistry &) :
  PhysicsListName_( p.get<std::string>("PhysicsListName")),
  DumpList_( p.get<bool>("DumpList")),
  enableCerenkov_( p.get<bool>("enableCerenkov")),    
  enableScintillation_( p.get<bool>("enableScintillation")),
  enableAbsorption_( p.get<bool>("enableAbsorption")),   
  enableRayleigh_( p.get<bool>("enableRayleigh")),     
  enableMieHG_( p.get<bool>("enableMieHG")),        
  enableBoundary_( p.get<bool>("enableBoundary")),     
  enableWLS_( p.get<bool>("enableWLS"))
{}

G4VUserPhysicsList* artg4tk::PhysicsListService::makePhysicsList() {

    G4PhysListFactory factory;
    G4VModularPhysicsList* phys = NULL;
    G4String physName = PhysicsListName_;
    //-----------------------------------------------------
    // Physics List name allows to select from 
    // the following 19 reference physics lists:
    //  CHIPS
    //  FTFP_BERT
    //  FTFP_BERT_TRV
    //  FTFP_BERT_HP
    //  FTF_BIC 
    //  LBE
    //  LHEP
    //  QBBC
    //  QGSC_BERT
    //  QGSP
    //  QGSP_BERT
    //  QGSP_BERT_CHIPS
    //  QGSP_BERT_HP
    //  QGSP_BIC
    //  QGSP_BIC_HP
    //  QGSP_FTFP_BERT
    //  QGS_BIC
    //  QGSP_INCLXX
    //  Shielding
    //-----------------------------------------------------

    if (factory.IsReferencePhysList(physName)) {
        phys = factory.GetReferencePhysList(physName);
    }

    //    G4bool enable_scint = true;
    // G4bool opticalphys = true;
    G4OpticalPhysics* opticalPhysics;
    opticalPhysics = new G4OpticalPhysics();
    if (enableCerenkov_||    
	enableScintillation_||
	enableAbsorption_||   
	enableRayleigh_||     
	enableMieHG_||        
	enableBoundary_||     
	enableWLS_ )          
      {
	phys->RegisterPhysics(opticalPhysics);
      }  else {

      delete opticalPhysics;
      G4cout << "no Optical physics processes selected" << G4endl;
    }

    if (enableCerenkov_)    
      {
       opticalPhysics->Configure(kCerenkov, true);
       G4cout << "Cerenkov enabled" << G4endl;
      }
    if (enableScintillation_)    
      {
       opticalPhysics->Configure(kScintillation, true);
       G4cout << "Scintillation enabled" << G4endl;
      }
    if (enableAbsorption_)    
      {
       opticalPhysics->Configure(kAbsorption, true);
       G4cout << "Absorption enabled" << G4endl;
      }
    if (enableRayleigh_)    
      {
       opticalPhysics->Configure(kRayleigh, true);
       G4cout << "Rayleigh enabled" << G4endl;
      }
    if (enableMieHG_)    
      {
       opticalPhysics->Configure(kMieHG, true);
       G4cout << "Mie enabled" << G4endl;
      }
    if (enableBoundary_)    
      {
       opticalPhysics->Configure(kBoundary, true);
       G4cout << "Boundary enabled" << G4endl;
      }
    
    if (enableWLS_)    
      {
	opticalPhysics->Configure(kWLS, true);
	G4cout << "WLS enabled" << G4endl;
      }
    


    /*
    if (opticalphys) {
        G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
        phys->RegisterPhysics(opticalPhysics);
        //
        //  opticalPhysics->Configure(kNoProcess, true);
        //  The following optical processes (indeces) are available
        //    
        //   kCerenkov,      ///< Cerenkov process index
        //   kScintillation, ///< Scintillation process index
        //   kAbsorption,    ///< Absorption process index
        //   kRayleigh,      ///< Rayleigh scattering process index
        //   kMieHG,         ///< Mie scattering process index
        //   kBoundary,      ///< Boundary process index
        //   kWLS,           ///< Wave Length Shifting process index
        //   kNoProcess      ///< Number of processes, no selected process
        //
        opticalPhysics->Configure(kCerenkov, true);
	//phys->AddContinuousProcess(theCerenkovProcess);
	//pmanager->AddProcess(theCerenkovProcess);
        //pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);

        if (enable_scint ) {
            opticalPhysics->Configure(kScintillation, true);
            G4cout << "Scintillation and Cerenkov enabled" << G4endl;
        } else {
            G4cout << "Scintillation disabled and Cerenkov enabled" << G4endl;
        }
    } else {
        G4cout << "Optical physics processes (scintillation/Cerenkov) disabled" << G4endl;
    }
*/
    if (DumpList_)
      {
	phys->DumpList();
	phys->DumpCutValuesTable();
      }
    return phys;
}

using artg4tk::PhysicsListService;
DEFINE_ART_SERVICE(PhysicsListService)
