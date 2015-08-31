// ArtG4EventAction.hh provides declarations for the built-in event action for
// the Art G4 simulation. In both BeginOfEventAction and EndOfEventAction, it
// gets the collection of action objects registered for the current job and
// calls their Begin/EndOfEventAction methods. In EndOfEventAction, it also
// gets the collection of registered detector components and calls their
// hit conversion methods. 

// Include guard
#ifndef ARTG4_EVENT_ACTION_HH
#define ARTG4_EVENT_ACTION_HH

// Some G4 includes
#include "Geant4/G4UserEventAction.hh"
#include "Geant4/G4Event.hh"

// Everything goes in the Art G4 namespace
namespace artg4tk {

  // Declare the class
  class ArtG4EventAction : public G4UserEventAction {
  public:
    // Compiler-generated constructor, destructor, copy constructor, and 
    // equality operator are all okay - there's not much to this class!

    // Called at the beginning of each event (note that this is after the
    // primaries have been generated and sent to the event manager)
    void BeginOfEventAction(const G4Event * currentEvent);

    // Called at the end of each event, right before GEANT's state switches
    // out of event processing and into closed geometry (last chance to access
    // the current event).
    void EndOfEventAction(const G4Event * currentEvent);

  };

}

#endif // ARTG4_EVENT_ACTION_HH
