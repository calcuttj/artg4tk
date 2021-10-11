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
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4Step.hh"

// artg4tk includes:
#include "artg4tk/pluginDetectors/gdml/HadInteractionSD.hh"

// c++ includes:
#include <vector>

artg4tk::HadInteractionSD::HadInteractionSD(G4String name) : G4VSensitiveDetector(name)
{
  fFirstInter.Clear();
  fStopAfter1stInt = false;

  G4SDManager* sdman = G4SDManager::GetSDMpointer();
  if (!sdman->FindSensitiveDetector(name))
    sdman->AddNewDetector(this);
}

void
artg4tk::HadInteractionSD::Initialize(G4HCofThisEvent*)
{
  fFirstInter.Clear();
}

G4bool
artg4tk::HadInteractionSD::ProcessHits(G4Step* theStep, G4TouchableHistory*)
{

  G4Track* trk = theStep->GetTrack();
  G4VPhysicalVolume* vol = trk->GetVolume();
  G4Material* mat = trk->GetMaterial();

  const std::vector<const G4Track*>* secs = theStep->GetSecondaryInCurrentStep();
  int nsec = secs->size();

  // check if anything generated in this step; if not, just return (nothing to do)
  //
  if (nsec <= 0)
    return true;

  // check if primary track looses idenity
  //
  if (trk->GetTrackStatus() != fAlive) {
    if (trk->GetParentID() == 0) {

      // OK, it's 1st (hadron inelastic) interaction

      fFirstInter.SetPosition(CLHEP::HepLorentzVector(trk->GetPosition(), trk->GetGlobalTime()));
      fFirstInter.SetPhysVolumeName(vol->GetName().c_str());
      fFirstInter.SetMaterialName(mat->GetName().c_str());
      fFirstInter.SetIncoming(ArtG4tkParticle(trk->GetParticleDefinition()->GetPDGEncoding(),
                                              theStep->GetPreStepPoint()->GetMomentum()));

      if (fStopAfter1stInt)
        theStep->GetTrack()->SetTrackStatus(fStopAndKill);

      for (int i = 0; i < nsec; i++) {
        G4Track* tr = (G4Track*)((*secs)[i]);
        fFirstInter.AddOutcoming(
          ArtG4tkParticle(tr->GetParticleDefinition()->GetPDGEncoding(), tr->GetMomentum()));
        if (fStopAfter1stInt)
          tr->SetTrackStatus(fStopAndKill);
      }
    }
  }

  return true;
}
