// This file provides the class declaration for the
// @ExampleGeneralAction@ action object, which manages the turn counter
// and keeps track of whether the current muon has been stored and how many
// total muons have been stored in the run.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: August 2012

#include "artg4tk/geantInit/ArtG4RunManager.hh"
#include "artg4tk/pluginActions/general/ExampleGeneralAction_service.hh"

// G4 includes
#include "Geant4/G4EventManager.hh"
#include "Geant4/G4MuonMinus.hh"
#include "Geant4/G4MuonPlus.hh"
#include "Geant4/G4Run.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4Track.hh"
#include "Geant4/G4TrackingManager.hh"

using std::string;

artg4tk::ExampleGeneralActionService::ExampleGeneralActionService(fhicl::ParameterSet const& p)
  : artg4tk::TrackingActionBase(p.get<string>("name", "exampleGeneral"))
  , artg4tk::RunActionBase(p.get<string>("name", "exampleGeneral"))
  , artg4tk::SteppingActionBase(p.get<string>("name", "exampleGeneral"))
{}

// Overload PreUserTrackingAction method to decide whether or not to
// track a given particle
void
artg4tk::ExampleGeneralActionService::preUserTrackingAction(const G4Track* currTrack)
{
  // Get the tracking manager so we can tell it whether or not to store
  // the trajectory.
  G4TrackingManager* trackingMan = G4EventManager::GetEventManager()->GetTrackingManager();

  // Create trajectory only for primaries
  if (currTrack->GetParentID() == 0) {
    trackingMan->SetStoreTrajectory(true);
  }
  else {
    trackingMan->SetStoreTrajectory(false);
  }
}

// Use BeginOfRunAction to initialize the random number store.
void
artg4tk::ExampleGeneralActionService::beginOfRunAction(const G4Run* currRun)
{
  G4cout << "### Run " << currRun->GetRunID() << " start." << G4endl;

  artg4tk::ArtG4RunManager::GetRunManager()->SetRandomNumberStore(true);
}

// Use UserSteppingAction (called for each step) to suspend any tracks that
// enter the calorimeter.
void
artg4tk::ExampleGeneralActionService::userSteppingAction(const G4Step* currStep)
{
  // This method suspends any tracks of live, primary non-muons that enter
  // the calorimeter.

  // Get the track in question.
  G4Track* theTrack = currStep->GetTrack();

  // check if the track is dead - if so, we don't need to suspend, so just
  // return.
  if (theTrack->GetTrackStatus() != fAlive) {
    return;
  }

  // check if it is a non-primary particle - if so, we don't need to suspend.
  if (theTrack->GetParentID() != 0) {
    return;
  }

  // check if it's a muon - if so, we don't need to suspend it.
  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  if ((particleType == G4MuonPlus::MuonPlusDefinition()) ||
      (particleType == G4MuonMinus::MuonMinusDefinition())) {
    return;
  }

  // First, check whether the step started inside the calorimeter. If so,
  // we don't need to suspend it.
  G4StepPoint* thePrePoint = currStep->GetPreStepPoint();
  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();
  G4String thePrePVname = thePrePV->GetName();
  if (thePrePVname(0, 4) == "calo") {
    return;
  }

  // Check whether the step ended outside the calorimeter. If so, we don't
  // need to suspend it.
  G4StepPoint* thePostPoint = currStep->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
  G4String thePostPVname = thePostPV->GetName();
  if (thePostPVname(0, 4) != "calo") {
    return;
  }

  // Any step that has survived all those checks:
  // * is alive
  // * is a primary
  // * is a non-muon
  // * started outside the calorimeter
  // * ended inside the calorimeter

  // Now suspend the track
  theTrack->SetTrackStatus(fSuspend);
}
