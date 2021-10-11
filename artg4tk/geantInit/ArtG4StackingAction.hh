// ArtG4StackingAction.hh provides declarations for the built-in stacking
// action for the Art G4 simulation. In its main method, UserStackingAction,
// it gets a collection of all action objects for the current run from the
// Action Holder service, and loops over them, calling their respective
// UserStackingAction methods.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

#ifndef artg4tk_geantInit_ArtG4StackingAction_hh
#define artg4tk_geantInit_ArtG4StackingAction_hh

#include "Geant4/G4UserStackingAction.hh"

// Everything goes in the Art G4 namespace
namespace artg4tk {
  class ActionHolderService;

  class ArtG4StackingAction : public G4UserStackingAction {
  public:
    explicit ArtG4StackingAction(ActionHolderService*);

  private:
    // Called for each new track
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*) override;

    ActionHolderService* actionHolder_;
  };

}

#endif /* artg4tk_geantInit_ArtG4StackingAction_hh */
