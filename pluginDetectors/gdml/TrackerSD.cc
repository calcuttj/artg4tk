/* ------------------------------------------------------------------------
            |\___/|       
            )     (    
           =\     /=
             )===(
            /     \         CaTS: Calorimeter and Tracker Simulation
            |     |         Author: Hans Wenzel (Fermilab)
           /       \
           \       /
            \__  _/
              ( (
               ) )
              (_(
-------------------------------------------------------------------------*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "artg4tk/pluginDetectors/gdml/TrackerSD.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4VVisManager.hh"

//#include "RootIO.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//namespace artg4tk {

 artg4tk::TrackerSD::TrackerSD(G4String name)
: G4VSensitiveDetector(name) {
    G4String HCname =  name + "_HC";
    collectionName.insert(HCname);
        G4cout << collectionName.size() << "   PhotonSD name:  " << name << " collection Name: " << HCname << G4endl;
    HCID = -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::TrackerSD::~TrackerSD() {
//    RootIO::GetInstance()->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  artg4tk::TrackerSD::Initialize(G4HCofThisEvent* HCE) {
    trackerCollection = new TrackerHitsCollection
            (SensitiveDetectorName, collectionName[0]);
    //static G4int HCID = -1;
    if (HCID < 0) {
        G4cout << "artg4tk::TrackerSD::Initialize:  " << SensitiveDetectorName << "   " << collectionName[0] << G4endl;
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    HCE->AddHitsCollection(HCID, trackerCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool  artg4tk::TrackerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    G4double edep = aStep->GetTotalEnergyDeposit();
    if (edep == 0.) return false;

    TrackerHit* newHit = new TrackerHit();
    newHit->SetEdep(edep);
    newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    trackerCollection->insert(newHit);
    return true;
}
//}
