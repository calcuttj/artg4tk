// ArtG4SteppingAction.hh provides declarations for the built-in stepping
// action for the Art G4 simulation. In its main method, UserSteppingAction,
// it gets a collection of all action objects for the current run from the
// Action Holder service, and loops over them, calling their respective
// UserSteppingAction methods.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// Include guard
#ifndef ARTG4_STEPPING_ACTION_HH
#define ARTG4_STEPPING_ACTION_HH

// A couple of G4 includes
#include "Geant4/G4UserSteppingAction.hh"
class G4Step;

// Everything goes in the Art G4 namespace
namespace artg4tk {

  // Declaration of the class
  class ArtG4SteppingAction : public G4UserSteppingAction {
  public:
    // Compiler-generated constructor, destructor, copy constructor, and 
    // equality operator are okay here.
    
    // Called at the end of each step (I think; the documentation is vague)
    void UserSteppingAction(const G4Step *);
  };

}

#endif // ARTG4_STEPPING_ACTION_HH
