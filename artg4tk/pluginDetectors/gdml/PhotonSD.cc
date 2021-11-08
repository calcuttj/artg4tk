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
// PhotonSD.cc: Class representing a sensitive detector registering optical
// photons
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#include "artg4tk/pluginDetectors/gdml/PhotonSD.hh"
#include "Geant4/G4OpticalPhoton.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4VProcess.hh"

artg4tk::PhotonSD::PhotonSD(G4String name) : G4VSensitiveDetector(name) {}

void
artg4tk::PhotonSD::Initialize(G4HCofThisEvent* HCE)
{
  hitCollection.clear();
}

G4bool
artg4tk::PhotonSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  int ID = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();
  G4Track* theTrack = aStep->GetTrack();

  // we only deal with optical Photons:
  if (theTrack->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {
    return false;
  }
  G4double theEdep = theTrack->GetTotalEnergy();
  const G4VProcess* thisProcess = theTrack->GetCreatorProcess();

  G4String processname;
  if (thisProcess != NULL)
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
  PhotonHit newHit = PhotonHit(ID,
                               theCreationProcessid,
                               theEdep,
                               aStep->GetPostStepPoint()->GetPosition().x(),
                               aStep->GetPostStepPoint()->GetPosition().y(),
                               aStep->GetPostStepPoint()->GetPosition().z(),
                               theTrack->GetGlobalTime());
  hitCollection.push_back(newHit);
  theTrack->SetTrackStatus(fStopAndKill);
  return true;
}
