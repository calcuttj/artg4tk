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
// CalorimeterSD.cc: Class representing a sensitive Detector for a Calorimeter
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#include "artg4tk/pluginDetectors/gdml/CalorimeterSD.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4VVisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::CalorimeterSD::CalorimeterSD(G4String name)
: G4VSensitiveDetector(name) {
    G4String HCname = name + "_HC";
    collectionName.insert(HCname);
    G4cout << collectionName.size() << "   artg4tk::CalorimeterSD name:  " << name << " collection Name: " << HCname << G4endl;
    HCID = -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::CalorimeterSD::~CalorimeterSD() {

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void artg4tk::CalorimeterSD::Initialize(G4HCofThisEvent* HCE) {

    calorimeterCollection = new CalorimeterHitsCollection
            (SensitiveDetectorName, collectionName[0]);
    if (HCID < 0) {
        G4cout << "artg4tk::CalorimeterSD::Initialize:  " << SensitiveDetectorName << "   " << collectionName[0] << G4endl;
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    }
    HCE->AddHitsCollection(HCID, calorimeterCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool artg4tk::CalorimeterSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    G4double edep = aStep->GetTotalEnergyDeposit() / CLHEP::MeV;
    if (edep == 0.) return false;
    const G4double time = aStep->GetPreStepPoint()->GetGlobalTime() / CLHEP::ns;
    const G4VTouchable* touch = aStep->GetPreStepPoint()->GetTouchable();
    const G4ThreeVector cellpos = touch->GetTranslation();
    G4Track* theTrack = aStep->GetTrack();
    G4String particleType = theTrack->GetDefinition()->GetParticleName();
    for (G4int j = 0; j < calorimeterCollection->entries(); j++) {
        CalorimeterHit* aPreviousHit = (*calorimeterCollection)[j];
        if (cellpos == aPreviousHit->GetPos()) {
            aPreviousHit->SetEdep(aStep->GetTotalEnergyDeposit() + aPreviousHit->GetEdep());
            if ((particleType == "e+") || (particleType == "gamma") || (particleType == "e-")) {
                aPreviousHit->SetEdepEM(edep + aPreviousHit->GetEdepEM());
            } else {
                aPreviousHit->SetEdepnonEM(edep + aPreviousHit->GetEdepnonEM());
            }
            return true;
        }
    }
    CalorimeterHit* newHit = new CalorimeterHit();
    newHit->SetEdep(edep);
    newHit->SetPos(cellpos);
    newHit->SetTime(time);
    if ((particleType == "e+") || (particleType == "gamma") || (particleType == "e-")) {
        newHit->SetEdepEM(edep);
        newHit->SetEdepnonEM(0.0);
    } else {
        newHit->SetEdepnonEM(edep);
        newHit->SetEdepEM(0.0);
    }

    calorimeterCollection->insert(newHit);
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

