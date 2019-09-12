// ArtG4StackingAction.hh provides declarations for the built-in stacking
// action for the Art G4 simulation. In its main method, UserStackingAction,
// it gets a collection of all action objects for the current run from the
// Action Holder service, and loops over them, calling their respective
// UserStackingAction methods.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// Include guard
#ifndef ARTG4_STACKING_ACTION_HH
#define ARTG4_STACKING_ACTION_HH

// A couple of G4 includes
#include "Geant4/G4UserStackingAction.hh"
class G4Track;

// Everything goes in the Art G4 namespace
namespace artg4tk {

  // Declaration of the class
  class ArtG4StackingAction : public G4UserStackingAction {
  public:
    // Compiler-generated constructor, destructor, copy constructor, and 
    // equality operator are okay here.
    
    // Called for each new track
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *);
  };

}

#endif // ARTG4_STACKING_ACTION_HH
