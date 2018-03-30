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
// TrackerSD.cc: Class representing a sensitive tracking detector
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#include "artg4tk/pluginDetectors/gdml/TrackerSD.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Event.hh"
#include "Geant4/G4EventManager.hh"
#include "Geant4/G4VSolid.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//namespace artg4tk {

 artg4tk::TrackerSD::TrackerSD(G4String name)
: G4VSensitiveDetector(name) {
   trackerCollection.clear();
    G4String HCname =  name + "_HC";
    collectionName.insert(HCname);
        G4cout << collectionName.size() << "   TrackerSD name:  " << name << " collection Name: " << HCname << G4endl;
    HCID = -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::TrackerSD::~TrackerSD() {
//    RootIO::GetInstance()->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  artg4tk::TrackerSD::Initialize(G4HCofThisEvent* HCE) {
   trackerCollection.clear();
    if (HCID < 0) {
        G4cout << "artg4tk::TrackerSD::Initialize:  " << SensitiveDetectorName << "   " << collectionName[0] << G4endl;
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool  artg4tk::TrackerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.) return false;
  if (aStep->GetTrack()->GetDynamicParticle()->GetCharge() == 0) return false;
  TrackerHit  newHit =  TrackerHit(
				   edep,
				   aStep->GetPostStepPoint()->GetPosition().x(),
				   aStep->GetPostStepPoint()->GetPosition().y(),
				   aStep->GetPostStepPoint()->GetPosition().z(),
				   aStep->GetPostStepPoint()->GetGlobalTime() / ns
				   );
    trackerCollection.push_back(newHit);
    return true;
}

