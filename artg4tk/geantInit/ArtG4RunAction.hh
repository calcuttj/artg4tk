// ArtG4RunAction.hh provides declarations for the built-in run action for
// the Art G4 simulation. It gets the collection of all action objects for the
// run and loops over them for both BeginOfRunAction and EndOfRunAction,
// calling the respective methods in each action object.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// Include guard
#ifndef ARTG4_RUN_ACTION_HH
#define ARTG4_RUN_ACTION_HH

// Some G4 includes
#include "Geant4/G4UserRunAction.hh"
class G4Run;

// Everything goes in the Art G4 namespace
namespace artg4tk {

  class ArtG4RunAction : public G4UserRunAction {
  public:
    // Compiler-generated default constructor, destructor, copy constructor,
    // and equality operator are all okay.

    // Called at the beginning of each run
    void BeginOfRunAction(const G4Run* currentRun);

    // Called at the end of each run
    void EndOfRunAction(const G4Run* currentRun);
  };
}
#endif // ARTG4_RUN_ACTION_HH
