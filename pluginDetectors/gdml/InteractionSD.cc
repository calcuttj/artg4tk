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
// InteractionSD.cc: sensitive detector that analysis the first interaction
// (based in test23 of the Geant 4 test suite)
// Author: Hans Wenzel (Fermilab)
//=============================================================================
// Geant 4 includes:
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4RunManager.hh"
#include "Geant4/G4VUserPrimaryGeneratorAction.hh"
#include "Geant4/G4ParticleGun.hh"
//artg4tk includes:
#include "artg4tk/pluginDetectors/gdml/InteractionSD.hh"
#include "pluginActions/myparticleGun/myParticleGunAction_service.hh"
// c++ includes:
#include <typeinfo>

artg4tk::InteractionSD::InteractionSD(G4String name)
: G4VSensitiveDetector(name) {
    G4String HCname = name + "_HC";
    collectionName.insert(HCname);
    G4cout << collectionName.size() << "   artg4tk::InteractionSD name:  " << name << " collection Name: " << HCname << G4endl;
    HCID = -1;
    fFirstInter = new ParticleChange(true);
    fOtherInter = new ParticleChange();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::InteractionSD::~InteractionSD() {
    if (fFirstInter) delete fFirstInter;
    if (fOtherInter) delete fOtherInter;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void artg4tk::InteractionSD::Initialize(G4HCofThisEvent* HCE) {
    G4cout << "Hits Collection capacity:  " << HCE->GetCapacity() << G4endl;
    interactionCollection = new InteractionHitsCollection
            (SensitiveDetectorName, collectionName[0]);
    if (HCID < 0) {
        G4cout << "artg4tk::InteractionSD::Initialize:  " << SensitiveDetectorName << "   " << collectionName[0] << G4endl;
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    }
    HCE->AddHitsCollection(HCID, interactionCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool artg4tk::InteractionSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {

    int nsc = fFirstInter->GetNumberOfSecondaries();
    if (nsc > 0) {
        for (int i = 0; i < nsc; i++) {
            delete fFirstInter->GetSecondary(i);
        }
        fFirstInter->Clear();
    }
    nsc = fOtherInter->GetNumberOfSecondaries();
    if (nsc > 0) {
        for (int i = 0; i < nsc; i++) {
            delete fOtherInter->GetSecondary(i);
        }
        fOtherInter->Clear();
    }
    const std::vector<const G4Track*>* secs = aStep->GetSecondaryInCurrentStep();
    int nsec = secs->size();
    for (int i = 0; i < nsec; i++) {

        G4Track* tr = new G4Track(*((*secs)[i]));

        if (aStep->GetTrack()->GetTrackStatus() != fAlive) // track looses identity
        {
            if (aStep->GetTrack()->GetParentID() == 0) // primary track
            {
                fFirstInter->AddSecondary(tr);
            } else // secondary track, and it's also looses identity (re-interaction)
            {
                fOtherInter->AddSecondary(tr);
            }
        }

    } //end loop over secondaries
    G4int NSec = fFirstInter->GetNumberOfSecondaries();
    if (NSec > 0) {
        const G4DynamicParticle* sec = 0;
        for (G4int i = 0; i < NSec; i++) {
            sec = fFirstInter->GetSecondary(i)->GetDynamicParticle();
            const G4String& pname = sec->GetDefinition()->GetParticleName();
            G4double pmom = (sec->GetTotalMomentum()) / MeV;
            G4double Ekin = (sec->GetKineticEnergy()) / MeV;
            G4double theta = (sec->GetMomentum()).theta();
            InteractionHit* newHit = new InteractionHit(pname, pmom, Ekin, theta);
            interactionCollection->insert(newHit);
        }
    }
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

