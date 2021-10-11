// ArtG4PrimaryGeneratorAction.hh provides declarations for the built-in
// primary generator action for the Art G4 simulation. In its main method,
// GeneratePrimaries, it gets the collection of all action objects registered
// for the run and calls their GeneratePrimaries method.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

#ifndef artg4tk_geantInit_ArtG4PrimaryGeneratorAction_hh
#define artg4tk_geantInit_ArtG4PrimaryGeneratorAction_hh

#include "Geant4/G4VUserPrimaryGeneratorAction.hh"

namespace artg4tk {

  class ActionHolderService;

  class ArtG4PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public:
    explicit ArtG4PrimaryGeneratorAction(ActionHolderService* actionHolder);

  private:
    // Create the primary particles for the event. Called after a G4Event has
    // been created but not fully initialized.
    void GeneratePrimaries(G4Event* anEvent) override;

    ActionHolderService* actionHolder_;
  };

}
#endif /* artg4tk_geantInit_ArtG4PrimaryGeneratorAction_hh */
