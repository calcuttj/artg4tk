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
#include "Geant4/G4Step.hh"
#include "Geant4/G4SystemOfUnits.hh"
#include "Geant4/G4ThreeVector.hh"

artg4tk::TrackerSD::TrackerSD(G4String name) : G4VSensitiveDetector(name)
{
  trackerCollection.clear();
}

void
artg4tk::TrackerSD::Initialize(G4HCofThisEvent* HCE)
{
  trackerCollection.clear();
}

G4bool
artg4tk::TrackerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.)
    return false;
  if (aStep->GetTrack()->GetDynamicParticle()->GetCharge() == 0)
    return false;
  TrackerHit newHit = TrackerHit(edep,
                                 aStep->GetPostStepPoint()->GetPosition().x(),
                                 aStep->GetPostStepPoint()->GetPosition().y(),
                                 aStep->GetPostStepPoint()->GetPosition().z(),
                                 aStep->GetPostStepPoint()->GetGlobalTime() / ns);
  trackerCollection.push_back(newHit);
  return true;
}
