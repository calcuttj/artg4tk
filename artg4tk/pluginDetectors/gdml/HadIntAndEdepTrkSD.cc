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
#include "Geant4/G4SystemOfUnits.hh"
#include "Geant4/G4ThreeVector.hh"

// artg4tk includes:
#include "artg4tk/pluginDetectors/gdml/HadIntAndEdepTrkSD.hh"

// c++ includes:
#include <vector>

artg4tk::HadIntAndEdepTrkSD::HadIntAndEdepTrkSD(G4String name) : G4VSensitiveDetector(name)
{

  fFirstInter.Clear();
  fEdepTrkHits.clear();

  G4SDManager* sdman = G4SDManager::GetSDMpointer();
  if (!sdman->FindSensitiveDetector(name))
    sdman->AddNewDetector(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::HadIntAndEdepTrkSD::~HadIntAndEdepTrkSD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void
artg4tk::HadIntAndEdepTrkSD::Initialize(G4HCofThisEvent*)
{

  fFirstInter.Clear();
  fEdepTrkHits.clear();

  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool
artg4tk::HadIntAndEdepTrkSD::ProcessHits(G4Step* theStep, G4TouchableHistory*)
{

  //    const std::vector<const G4Track*>* secs = theStep->GetSecondaryInCurrentStep();
  //   int nsec = secs->size();

  // check if anything generated in this step; if not, just return (nothing to do)
  //
  if (theStep->GetSecondaryInCurrentStep()->size() > 0)
    ProcessHadInt(theStep);
  if (theStep->GetTotalEnergyDeposit() > 0.)
    ProcessEdep(theStep);

  return true;
}

void
artg4tk::HadIntAndEdepTrkSD::ProcessHadInt(G4Step* theStep)
{

  if (!theStep)
    return;

  G4Track* trk = theStep->GetTrack();
  if (!trk)
    return;

  G4VPhysicalVolume* vol = trk->GetVolume();
  G4Material* mat = trk->GetMaterial();

  const std::vector<const G4Track*>* secs = theStep->GetSecondaryInCurrentStep();

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

      // --->	 if ( fStopAfter1stInt ) theStep->GetTrack()->SetTrackStatus( fStopAndKill );

      if (!secs)
        return;

      int nsec = secs->size();
      for (int i = 0; i < nsec; i++) {
        G4Track* tr = (G4Track*)((*secs)[i]);
        fFirstInter.AddOutcoming(
          ArtG4tkParticle(tr->GetParticleDefinition()->GetPDGEncoding(), tr->GetMomentum()));
        // --->            if ( fStopAfter1stInt ) tr->SetTrackStatus( fStopAndKill );
      }
    }
  }

  return;
}

void
artg4tk::HadIntAndEdepTrkSD::ProcessEdep(G4Step* theStep)
{

  if (theStep->GetTotalEnergyDeposit() <= 0.)
    return; // extra protection

  double edep = theStep->GetTotalEnergyDeposit();
  const G4ThreeVector& pos = theStep->GetTrack()->GetPosition();
  double time = theStep->GetPostStepPoint()->GetGlobalTime() / ns;
  fEdepTrkHits.push_back(TrackerHit(edep, pos.x(), pos.y(), pos.z(), time));

  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
