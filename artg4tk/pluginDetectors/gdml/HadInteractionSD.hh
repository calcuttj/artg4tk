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
// InteractionSD.cc: sensitive detector that analysis the first interaction
// (based in test23 of the Geant 4 test suite)
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef artg4tk_pluginDetectors_gdml_HadInteractionSD_hh
#define artg4tk_pluginDetectors_gdml_HadInteractionSD_hh

#include "Geant4/G4VSensitiveDetector.hh"
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkVtx.hh"

class G4Step;
class G4HCofThisEvent;

namespace artg4tk {

  class HadInteractionSD : public G4VSensitiveDetector {
  public:
    // ctor & dtor
    HadInteractionSD(G4String);

    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
    void
    clear() override
    {
      fFirstInter.Clear();
    }

    // specific to this SD
    //
    void
    SetStopAfter1stInt(bool flag = true)
    {
      fStopAfter1stInt = flag;
    }
    ArtG4tkVtx const&
    Get1stInteraction() const
    {
      return fFirstInter;
    }

  private:
    ArtG4tkVtx fFirstInter;
    bool fStopAfter1stInt;
  };
}

#endif /* artg4tk_pluginDetectors_gdml_HadInteractionSD_hh */
