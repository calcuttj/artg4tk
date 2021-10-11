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
#include "Geant4/G4Cerenkov.hh"
#include "Geant4/G4EventManager.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4SteppingManager.hh"
#include "Geant4/G4SystemOfUnits.hh"
#include "Geant4/G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {

  DRCalorimeterSD::DRCalorimeterSD(G4String name) : G4VSensitiveDetector(name) {}

  void
  DRCalorimeterSD::EndOfEvent(G4HCofThisEvent*)
  {}

  void
  DRCalorimeterSD::Initialize(G4HCofThisEvent* HCE)
  {
    drcalorimeterCollection.clear();
    //
    TotalE = 0.0;
    EbyParticle["Fragment"] = 0.0;
    EbyParticle["He3"] = 0.0;
    EbyParticle["alpha"] = 0.0;
    EbyParticle["deuteron"] = 0.0;
    EbyParticle["triton"] = 0.0;
    EbyParticle["proton"] = 0.0;
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
    EbyParticle["Etot"] = 0.0;  // for redundance
    //
    TotalNCeren = 0.0;
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
    NCerenbyParticle["other"] = 0.0;     // Just in case
    NCerenbyParticle["NCerentot"] = 0.0; // for redundance
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  G4bool
  DRCalorimeterSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
  {
    G4double edep = aStep->GetTotalEnergyDeposit() / CLHEP::MeV;
    if (edep == 0.)
      return false;
    if (aStep->GetTrack()->GetDynamicParticle()->GetCharge() == 0)
      return false;
    TotalE = TotalE + edep;
    const G4double time = aStep->GetPreStepPoint()->GetGlobalTime() / CLHEP::ns;
    const G4VTouchable* touch = aStep->GetPreStepPoint()->GetTouchable();
    G4int NCerenPhotons = 0;
    G4SteppingManager* fpSteppingManager =
      G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager();
    G4StepStatus stepStatus = fpSteppingManager->GetfStepStatus();
    if (stepStatus != fAtRestDoItProc) {
      G4ProcessVector* procPost = fpSteppingManager->GetfPostStepDoItVector();
      size_t MAXofPostStepLoops = fpSteppingManager->GetMAXofPostStepLoops();
      for (size_t i3 = 0; i3 < MAXofPostStepLoops; i3++) {

        if ((*procPost)[i3]->GetProcessName() == "Cerenkov") {
          G4Cerenkov* proc = (G4Cerenkov*)(*procPost)[i3];
          NCerenPhotons += proc->GetNumPhotons();
        }
      }
    }
    G4Track* theTrack = aStep->GetTrack();
    G4String particleType = theTrack->GetDefinition()->GetParticleName();
    G4String fragment = "Fragment";
    EbyParticle["Etot"] = EbyParticle["Etot"] + edep;
    if (theTrack->GetParticleDefinition()->GetParticleType() == "nucleus" &&
        theTrack->GetParticleDefinition()->GetParticleSubType() == "generic") {
      particleType = fragment;
    }
    if (EbyParticle.find(particleType) == EbyParticle.end()) {
      EbyParticle["other"] = EbyParticle["other"] + edep;
    } else {
      EbyParticle[particleType] = EbyParticle[particleType] + edep;
    }
    NCerenbyParticle["NCerentot"] = NCerenbyParticle["NCerentot"] + NCerenPhotons;
    if (NCerenbyParticle.find(particleType) == NCerenbyParticle.end()) {
      NCerenbyParticle["other"] = NCerenbyParticle["other"] + NCerenPhotons;
    } else {
      NCerenbyParticle[particleType] = NCerenbyParticle[particleType] + NCerenPhotons;
    }
    const G4ThreeVector cellpos = touch->GetTranslation();
    unsigned int ID = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();
    //
    // check if this cell has been previously Hit if so jst add the new energy deposition
    //
    for (unsigned int j = 0; j < drcalorimeterCollection.size(); j++) {
      DRCalorimeterHit* aPreviousHit = &drcalorimeterCollection[j];
      if (ID == aPreviousHit->GetID()) {
        aPreviousHit->SetEdep(aStep->GetTotalEnergyDeposit() + aPreviousHit->GetEdep());
        if ((particleType == "e+") || (particleType == "gamma") || (particleType == "e-")) {
          aPreviousHit->Setem_Edep(edep + aPreviousHit->GetEdepEM());
        } else {
          aPreviousHit->Setnonem_Edep(edep + aPreviousHit->GetEdepnonEM());
        }
        return true;
      }
    }
    DRCalorimeterHit newHit;
    newHit.SetID(ID);
    newHit.SetEdep(edep);
    newHit.SetNceren(NCerenPhotons);
    newHit.SetXpos(cellpos.x());
    newHit.SetYpos(cellpos.y());
    newHit.SetZpos(cellpos.z());
    newHit.SetTime(time);
    if ((particleType == "e+") || (particleType == "gamma") || (particleType == "e-")) {
      newHit.Setem_Edep(edep);
      newHit.Setnonem_Edep(0.0);
    } else {
      newHit.Setnonem_Edep(edep);
      newHit.Setem_Edep(0.0);
    }
    drcalorimeterCollection.push_back(newHit);
    return true;
  } // end of process Hits
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
