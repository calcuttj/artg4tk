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
// PhotonSD.cc: Class representing a sensitive detector registering optical
// photons
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef artg4tk_pluginDetectors_gdml_PhotonSD_hh
#define artg4tk_pluginDetectors_gdml_PhotonSD_hh

#include "Geant4/G4VSensitiveDetector.hh"
#include "artg4tk/pluginDetectors/gdml/PhotonHit.hh"

class G4Step;
class G4HCofThisEvent;

namespace artg4tk {
  class PhotonSD : public G4VSensitiveDetector {
  public:
    explicit PhotonSD(G4String);

    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
    const PhotonHitCollection&
    GetHits() const
    {
      return hitCollection;
    }

  private:
    PhotonHitCollection hitCollection;
  };
}

#endif /* artg4tk_pluginDetectors_gdml_PhotonSD_hh */
