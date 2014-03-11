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
// DRCalorimeterSD.cc: 
// implementation of a Dual read out sensitive Detector which registers 
// both ionization and Cerenkov contributions in a calorimeter cell
//
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#include "artg4tk/pluginDetectors/gdml/DRCalorimeterSD.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Poisson.hh"
#include<iomanip>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {

    DRCalorimeterSD::DRCalorimeterSD(G4String name)
    : G4VSensitiveDetector(name) {
        G4String HCname = name + "_HC";
        collectionName.insert(HCname);
        CerenGenerator = new(Cerenkov);
        G4cout << collectionName.size() << "   artg4tk::DRCalorimeterSD name:  " << name << " collection Name: " << HCname << G4endl;
        HCID = -1;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    DRCalorimeterSD::~DRCalorimeterSD() {

    }
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    void DRCalorimeterSD::EndOfEvent(G4HCofThisEvent*) {
        //for (std::map<std::string,double>::iterator it=EbyParticle.begin(); it!=EbyParticle.end(); ++it) {
        //    std::cout << "Particle: " << it->first << "   " << 100.0 * it->second / TotalE << " % " << std::endl;
        //}
    }
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    void DRCalorimeterSD::Initialize(G4HCofThisEvent* HCE) {
        calorimeterCollection = new DRCalorimeterHitsCollection(SensitiveDetectorName, collectionName[0]);
        if (HCID < 0) {
            G4cout << "artg4tk::DRCalorimeterSD::Initialize:  " << SensitiveDetectorName << "   " << collectionName[0] << G4endl;
            HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

        }
        HCE->AddHitsCollection(HCID, calorimeterCollection);
        // 
        TotalE = 0.0;
        EbyParticle["Fragment"] = 0.0;
        EbyParticle["He3"] = 0.0;
        EbyParticle["alpha"] = 0.0;
        EbyParticle["deuteron"] = 0.0;
        EbyParticle["triton"] = 0.0;
        EbyParticle["proton"] = 0.0;
        //   EbyParticle["p_ev"] = 0.0;
        //   EbyParticle["p_sp"] = 0.0;
        //   EbyParticle["p_he"] = 0.0;
        EbyParticle["neutron"] = 0.0;
        EbyParticle["e+"] = 0.0;
        EbyParticle["e-"] = 0.0;
        EbyParticle["pi+"] = 0.0;
        EbyParticle["pi-"] = 0.0;
        EbyParticle["gamma"] = 0.0;
        EbyParticle["mu+"] = 0.0;
        EbyParticle["mu-"] = 0.0;
        EbyParticle["sigma+"] = 0.0;
        EbyParticle["sigma-"] = 0.0;
        EbyParticle["kaon+"] = 0.0;
        EbyParticle["kaon-"] = 0.0;
        EbyParticle["kaon0L"] = 0.0;
        EbyParticle["kaon0S"] = 0.0;
        EbyParticle["lambda"] = 0.0;
        EbyParticle["xi-"] = 0.0;
        EbyParticle["anti_neutron"] = 0.0;
        EbyParticle["anti_sigma-"] = 0.0;
        EbyParticle["anti_proton"] = 0.0;
        EbyParticle["anti_xi-"] = 0.0;
        EbyParticle["anti_omega-"] = 0.0;
        EbyParticle["anti_sigma+"] = 0.0;
        EbyParticle["anti_lambda"] = 0.0;
        EbyParticle["anti_xi0"] = 0.0;
        EbyParticle["other"] = 0.0; // Just in case 
        //
        TotalNCeren=0.0;
        NCerenbyParticle["e+"] = 0.0;
        NCerenbyParticle["e-"] = 0.0;
        NCerenbyParticle["kaon+"] = 0.0;
        NCerenbyParticle["kaon-"] = 0.0;
        NCerenbyParticle["mu+"] = 0.0;
        NCerenbyParticle["mu-"] = 0.0;
        NCerenbyParticle["pi+"] = 0.0;
        NCerenbyParticle["pi-"] = 0.0;
        NCerenbyParticle["proton"] = 0.0;
        NCerenbyParticle["deuteron"] = 0.0;
        NCerenbyParticle["triton"] = 0.0;
        NCerenbyParticle["He3"] = 0.0;
        NCerenbyParticle["sigma-"] = 0.0;
        NCerenbyParticle["sigma+"] = 0.0;
        NCerenbyParticle["xi-"] = 0.0;
        NCerenbyParticle["anti_sigma-"] = 0.0;
        NCerenbyParticle["anti_proton"] = 0.0;
        NCerenbyParticle["anti_xi-"] = 0.0;
        NCerenbyParticle["anti_omega-"] = 0.0;
        NCerenbyParticle["anti_sigma+"] = 0.0;
        NCerenbyParticle["other"] = 0.0; // Just in case
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    G4bool DRCalorimeterSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
        G4double edep = aStep->GetTotalEnergyDeposit() / MeV;
        if (edep == 0.) return false;
        TotalE = TotalE + edep;
        const G4double time = aStep->GetPreStepPoint()->GetGlobalTime() / ns;
        const G4VTouchable* touch = aStep->GetPreStepPoint()->GetTouchable();
        G4String thematerial = touch->GetVolume()->GetLogicalVolume()->GetMaterial()->GetName();
        //const G4ThreeVector cellpos = touch->GetTranslation();
        G4int NCerenPhotons = 0;
        G4Track* theTrack = aStep->GetTrack();
        const G4double charge = theTrack->GetDefinition()->GetPDGCharge();
        G4String particleType = theTrack->GetDefinition()->GetParticleName();
        G4String fragment = "Fragment";
        if (theTrack->GetParticleDefinition()->GetParticleType() == "nucleus" && theTrack->GetParticleDefinition()->GetParticleSubType() == "generic") {
            particleType = fragment;
        }
        if (EbyParticle.find(particleType) == EbyParticle.end()) {
            EbyParticle["other"] = EbyParticle["other"] + edep;
        } else {
            EbyParticle[particleType] = EbyParticle[particleType] + edep;
        }

        G4StepPoint* pPreStepPoint = aStep->GetPreStepPoint();
        G4StepPoint* pPostStepPoint = aStep->GetPostStepPoint();
        G4double beta = 0.5 * (pPreStepPoint ->GetBeta() + pPostStepPoint->GetBeta());
        G4double MeanNumberOfPhotons = CerenGenerator->GetAverageNumberOfPhotons(charge, beta, thematerial);
        if (MeanNumberOfPhotons > 0.0) {
            G4double step_length = aStep->GetStepLength();
            MeanNumberOfPhotons = MeanNumberOfPhotons * step_length;
            NCerenPhotons = (G4int) G4Poisson(MeanNumberOfPhotons);
            TotalNCeren=TotalNCeren+NCerenPhotons;
            if (NCerenbyParticle.find(particleType) == NCerenbyParticle.end()) {
                NCerenbyParticle["other"] = NCerenbyParticle["other"] +NCerenPhotons;
            } else {
                NCerenbyParticle[particleType] = NCerenbyParticle[particleType] + NCerenPhotons;
            }
        } else {
            NCerenPhotons = 0;
        }
        const G4ThreeVector cellpos = touch->GetTranslation();
        for (G4int j = 0; j < calorimeterCollection->entries(); j++) {
            DRCalorimeterHit* aPreviousHit = (*calorimeterCollection)[j];
            if (cellpos == aPreviousHit->GetPos()) {
                aPreviousHit->SetEdep(aStep->GetTotalEnergyDeposit() + aPreviousHit->GetEdep());
                aPreviousHit->SetNCeren(NCerenPhotons + aPreviousHit->GetNCeren());
                if ((particleType == "e+") || (particleType == "gamma") || (particleType == "e-")) {
                    aPreviousHit->SetEdepEM(edep + aPreviousHit->GetEdepEM());
                } else {
                    aPreviousHit->SetEdepnonEM(edep + aPreviousHit->GetEdepnonEM());
                }
                return true;
            }
        }
        DRCalorimeterHit* newHit = new DRCalorimeterHit();
        newHit->SetEdep(edep);
        newHit->SetNCeren(NCerenPhotons);
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

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

