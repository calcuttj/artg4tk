// ArtG4EventAction.hh provides declarations for the built-in event action for
// the Art G4 simulation. In both BeginOfEventAction and EndOfEventAction, it
// gets the collection of action objects registered for the current job and
// calls their Begin/EndOfEventAction methods. In EndOfEventAction, it also
// gets the collection of registered detector components and calls their
// hit conversion methods.

#ifndef artg4tk_geantInit_ArtG4EventAction_hh
#define artg4tk_geantInit_ArtG4EventAction_hh

#include "Geant4/G4UserEventAction.hh"

namespace artg4tk {

  class ActionHolderService;
  class DetectorHolderService;

  class ArtG4EventAction : public G4UserEventAction {
  public:
    ArtG4EventAction(ActionHolderService* actionHolder, DetectorHolderService* detectorHolder);

  private:
    // Called at the beginning of each event (note that this is after the
    // primaries have been generated and sent to the event manager)
    void BeginOfEventAction(const G4Event* currentEvent) override;

    // Called at the end of each event, right before GEANT's state switches
    // out of event processing and into closed geometry (last chance to access
    // the current event).
    void EndOfEventAction(const G4Event* currentEvent) override;

    ActionHolderService* actionHolder_;
    DetectorHolderService* detectorHolder_;
  };

}

#endif /* artg4tk_geantInit_ArtG4EventAction_hh */
