// ArtG4PrimaryGeneratorAction.hh provides declarations for the built-in
// primary generator action for the Art G4 simulation. In its main method, 
// GeneratePrimaries, it gets the collection of all action objects registered
// for the run and calls their GeneratePrimaries method.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// Include guard
#ifndef ARTG4_PGA_HH
#define ARTG4_PGA_HH

// G4 includes
#include "Geant4/G4VUserPrimaryGeneratorAction.hh"
#include "Geant4/G4Event.hh"

// Everything goes in the Art G4 namespace
namespace artg4tk {

  class ArtG4PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public: 
    // Compiler-generated c'tor, d'tor, copy c'tor and equality operator are
    // fine - not much to this class!

    // Create the primary particles for the event. Called after a G4Event has
    // been created but not fully initialized.
    void GeneratePrimaries(G4Event *anEvent);

  };

}
#endif // ARTG4_PGA_HH
