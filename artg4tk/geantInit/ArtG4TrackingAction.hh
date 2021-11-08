// ArtG4TrackingAction.hh provides declarations for the built-in tracking action
// for the Art G4 simulation. In its two main methods, PreUserTrackingAction
// and PostUserTrackingAction, it gets the collection of all action objects
// registered for the simulation job and loops over them, calling their
// respective methods (conveniently called Pre/PostUserTrackingAction).

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

#ifndef artg4tk_geantInit_ArtG4TrackingAction_hh
#define artg4tk_geantInit_ArtG4TrackingAction_hh

// G4 includes
#include "Geant4/G4UserTrackingAction.hh"

namespace artg4tk {
  class ActionHolderService;

  class ArtG4TrackingAction : public G4UserTrackingAction {
  public:
    explicit ArtG4TrackingAction(ActionHolderService* actionHolder);

  private:
    // Called immediately after the creation of a track and before simulating
    // it.
    void PreUserTrackingAction(const G4Track* currentTrack) override;

    // Called after stopping a track
    void PostUserTrackingAction(const G4Track* currentTrack) override;

    ActionHolderService* actionHolder_;
  };

}

#endif /* artg4tk_geantInit_ArtG4TrackingAction_hh */
