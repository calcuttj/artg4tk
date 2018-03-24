//
//               __        __ __  __  __  
//   ____ ______/ /_____ _/ // / / /_/ /__
//  / __ `/ ___/ __/ __ `/ // /_/ __/ //_/
// / /_/ / /  / /_/ /_/ /__  __/ /_/ ,<   
// \__,_/_/   \__/\__, /  /_/  \__/_/|_|  
//               /____/                  
//
// artg4tk: art based Geant 4 Toolkit
// 
//=============================================================================
// nobleGasTPCSD.cc: Class representing a sensitive tracking detector
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#include "artg4tk/pluginDetectors/gdml/nobleGasTPCSD.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Event.hh"
#include "Geant4/G4EventManager.hh"
#include "Geant4/G4VSolid.hh"
#include "Geant4/G4Cerenkov.hh"
#include "Geant4/G4Scintillation.hh"
#include "Geant4/G4SteppingManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//namespace artg4tk {

 artg4tk::nobleGasTPCSD::nobleGasTPCSD(G4String name)
: G4VSensitiveDetector(name) {
   hitCollection.clear();
    G4String HCname =  name + "_HC";
    collectionName.insert(HCname);
        G4cout << collectionName.size() << "   nobleGasTPCSD name:  " << name << " collection Name: " << HCname << G4endl;
    HCID = -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::nobleGasTPCSD::~nobleGasTPCSD() {
//    RootIO::GetInstance()->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  artg4tk::nobleGasTPCSD::Initialize(G4HCofThisEvent* HCE) {
   hitCollection.clear();
  // trackerCollection = new nobleGasTPCHitsCollection
  //          (SensitiveDetectorName, collectionName[0]);
  // trackerCollection = new nobleGasTPCHitCollection();
  //trackerCollection->reserve(100);
    //static G4int HCID = -1;
    if (HCID < 0) {
        G4cout << "artg4tk::nobleGasTPCSD::Initialize:  " << SensitiveDetectorName << "   " << collectionName[0] << G4endl;
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    //    HCE->AddHitsCollection(HCID, trackerCollection);
}
//artg4tk::nobleGasTPCHitCollection* artg4tk::nobleGasTPCSD::getTrackerCollection()
//      {return trackerCollection;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool  artg4tk::nobleGasTPCSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  // std::cout << "nobleGasTPCSD: position = " << aStep->GetTrack()->GetPosition().x() << " "
  //	                                       << aStep->GetTrack()->GetPosition().y() << " "
  //					       << aStep->GetTrack()->GetPosition().z() << std::endl;

/*
   G4Track*           trk = aStep->GetTrack();
   std::cout << "nobleGasTPCSD: particle = " << trk->GetParticleDefinition()->GetPDGEncoding() << std::endl;
   std::cout << "nobleGasTPCSD: parent of the particle (ID) = " << trk->GetParentID() << std::endl;
	 std::cout << "nobleGasTPCSD: position = " << aStep->GetTrack()->GetPosition().x() << " "
	                                       << aStep->GetTrack()->GetPosition().y() << " "
					       << aStep->GetTrack()->GetPosition().z() << std::endl;

    std::cout << "nobleGasTPCSD: momentum = " << trk->GetMomentum().x() << " "
                                          << trk->GetMomentum().y() << " "
					  << trk->GetMomentum().z() << std::endl;
    
    std::cout << "nobleGasTPCSD: edep = " << aStep->GetTotalEnergyDeposit() << std::endl;
*/
    
  //hjw G4double edep = aStep->GetTotalEnergyDeposit();
  //hjw if (edep == 0.) return false;
       G4double edep = aStep->GetTotalEnergyDeposit();
    //std::cout<<"TrackerDS:  "<<aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()<<" Coy: "<<aStep->GetPostStepPoint()->GetPhysicalVolume()->GetCopyNo()<<std::endl;
    //std::cout<<"TrackerDS:  "<<aStep->GetPostStepPoint()->GetPhysicalVolume()->GetLogicalVolume()<<std::endl;
    if (edep == 0.) return false;
    if (aStep->GetTrack()->GetDynamicParticle()->GetCharge() == 0) return false;
    G4int photons = 0;
    G4SteppingManager* fpSteppingManager = G4EventManager::GetEventManager()
            ->GetTrackingManager()->GetSteppingManager();
    G4StepStatus stepStatus = fpSteppingManager->GetfStepStatus();
    if (stepStatus != fAtRestDoItProc) {
        G4ProcessVector* procPost = fpSteppingManager->GetfPostStepDoItVector();
        size_t MAXofPostStepLoops = fpSteppingManager->GetMAXofPostStepLoops();
        for (size_t i3 = 0; i3 < MAXofPostStepLoops; i3++) {
            /*
            if ((*procPost)[i3]->GetProcessName() == "Cerenkov") {
                G4Cerenkov* proc =(G4Cerenkov*) (*procPost)[i3];
                photons+=proc->GetNumPhotons();
            }
             */
            if ((*procPost)[i3]->GetProcessName() == "Scintillation") {
                G4Scintillation* proc1 = (G4Scintillation*) (*procPost)[i3];
		std::cout<<"nr of photons:  "<<proc1->GetNumPhotons()<<std::endl;
                photons += proc1->GetNumPhotons();
            }
        }
    }
    nobleGasTPCHit  newHit =  nobleGasTPCHit(
							   edep,
							   aStep->GetPostStepPoint()->GetPosition().x(),
							   aStep->GetPostStepPoint()->GetPosition().y(),
							   aStep->GetPostStepPoint()->GetPosition().z()
							   );
    //    newHit->SetEdep(edep);
    //    newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    //trackerCollection->insert(newHit);
    hitCollection.push_back(newHit);
    return true;
}
//}
