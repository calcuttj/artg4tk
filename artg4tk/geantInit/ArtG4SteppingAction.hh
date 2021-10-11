// ArtG4SteppingAction.hh provides declarations for the built-in
// stepping action for the Art G4 simulation. In its main method,
// UserSteppingAction, it gets a collection of all action objects from
// the Action Holder service and loops over them, calling their
// respective UserSteppingAction methods.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

#ifndef artg4tk_geantInit_ArtG4SteppingAction_hh
#define artg4tk_geantInit_ArtG4SteppingAction_hh

#include "Geant4/G4UserSteppingAction.hh"

// Everything goes in the Art G4 namespace
namespace artg4tk {
  class ActionHolderService;

  // Declaration of the class
  class ArtG4SteppingAction : public G4UserSteppingAction {
  public:
    explicit ArtG4SteppingAction(ActionHolderService*);

  private:
    // Called at the end of each step (I think; the documentation is vague)
    void UserSteppingAction(const G4Step*) override;

    ActionHolderService* actionHolder_;
  };
}

#endif /* artg4tk_geantInit_ArtG4SteppingAction_hh */
