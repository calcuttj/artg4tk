//
//               __        __ __  __  __
//   ____ ______/ /_____ _/ // / / /_/ /__
//  / __ `/ ___/ __/ __ `/ // /_/ __/ //_/
// / /_/ / /  / /_/ /_/ /__  __/ /_/ ,<
// \__,_/_/   \__/\__, /  /_/  \__/_/|_|
//               /____/
//
// artg4tk: art based Geant 4 Toolkit
//
//=============================================================================
// TrackerSD.hh: Class representing a sensitive tracking detector
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef artg4tk_pluginDetectors_gdml_TrackerSD_hh
#define artg4tk_pluginDetectors_gdml_TrackerSD_hh

#include "Geant4/G4VSensitiveDetector.hh"
#include "artg4tk/pluginDetectors/gdml/TrackerHit.hh"
class G4Step;
class G4HCofThisEvent;

namespace artg4tk {

  class TrackerSD : public G4VSensitiveDetector {
  public:
    TrackerSD(G4String);

    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
    const TrackerHitCollection&
    GetHits() const
    {
      return trackerCollection;
    }

  private:
    TrackerHitCollection trackerCollection;
  };

}

#endif /* artg4tk_pluginDetectors_gdml_TrackerSD_hh */
