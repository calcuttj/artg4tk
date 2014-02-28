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

#include "artg4tk/pluginDetectors/gdml/PhotonSD.hh"
#include "Geant4/G4VProcess.hh"
#include "Geant4/G4OpticalPhoton.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4VVisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::PhotonSD::PhotonSD(G4String name)
: G4VSensitiveDetector(name) {
    G4String HCname = name + "_HC";
    collectionName.insert(HCname);
    G4cout << collectionName.size() << "   PhotonSD name:  " << name << " collection Name: " << HCname << G4endl;
    HCID = -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void artg4tk::PhotonSD::Initialize(G4HCofThisEvent* HCE) {
    photonCollection = new PhotonHitsCollection(SensitiveDetectorName, collectionName[0]);
    if (HCID < 0) {
        G4cout << "artg4tk::PhotonSD::Initialize:  " << SensitiveDetectorName << "   " << collectionName[0] << G4endl;
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    HCE->AddHitsCollection(HCID, photonCollection);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::PhotonSD::~PhotonSD() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool artg4tk::PhotonSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {

    G4Track* theTrack = aStep->GetTrack();
                 
    // we only deal with optical Photons:
    if (theTrack->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {
        return false;
    }
    G4double theEdep = theTrack->GetTotalEnergy();
    const G4VProcess * thisProcess = theTrack->GetCreatorProcess();
    
    G4String processname;
    if(thisProcess !=NULL)
         processname = thisProcess->GetProcessName();
    else
        processname = "No Process";
    G4int theCreationProcessid;
    if (processname == "Cerenkov") {
        theCreationProcessid = 0;
    } else if (processname == "Scintillation") {
        theCreationProcessid = 1;
    } else {
        theCreationProcessid = -1;
    }
    PhotonHit* newHit = new PhotonHit();
    newHit->SetProcessID(theCreationProcessid);
    newHit->SetEdep(theEdep);
    newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    newHit->SetTime(theTrack->GetGlobalTime());
    photonCollection->insert(newHit);
    theTrack->SetTrackStatus(fStopAndKill);
    return true;
}

