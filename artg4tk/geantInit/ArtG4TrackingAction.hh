// ArtG4TrackingAction.hh provides declarations for the built-in tracking action
// for the Art G4 simulation. In its two main methods, PreUserTrackingAction
// and PostUserTrackingAction, it gets the collection of all action objects
// registered for the simulation job and loops over them, calling their
// respective methods (conveniently called Pre/PostUserTrackingAction).

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// Include guard
#ifndef ARTG4_TRACKING_ACTION
#define ARTG4_TRACKING_ACTION

// G4 includes
#include "Geant4/G4UserTrackingAction.hh"
class G4Track;

// Everything goes in the Art G4 namespace
namespace artg4tk {

  class ArtG4TrackingAction : public G4UserTrackingAction {
  public:
    // Compiler-generated constructor, destructor, copy constructor, equality
    // operator are all okay here - there's nothing to keep track of!
    
    // Called immediately after the creation of a track and before simulating
    // it.
    void PreUserTrackingAction(const G4Track * currentTrack);

    // Called after stopping a track
    void PostUserTrackingAction(const G4Track * currentTrack);

  };

}

#endif // ARTG4_TRACKING_ACTION
