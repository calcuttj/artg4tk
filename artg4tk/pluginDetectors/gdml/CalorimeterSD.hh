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
// CalorimeterSD.hh: Class representing a sensitive Detector for a Calorimeter
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef artg4tk_pluginDetectors_gdml_CalorimeterSD_hh
#define artg4tk_pluginDetectors_gdml_CalorimeterSD_hh

#include "Geant4/G4VSensitiveDetector.hh"
#include "artg4tk/pluginDetectors/gdml/CalorimeterHit.hh"

class G4Step;
class G4HCofThisEvent;

namespace artg4tk {
  class CalorimeterSD : public G4VSensitiveDetector {
  public:
    CalorimeterSD(G4String);
    ~CalorimeterSD();

    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;

    CalorimeterHitCollection const&
    GetHits() const
    {
      return calorimeterCollection;
    }

  private:
    CalorimeterHitCollection calorimeterCollection;
  };
}

#endif /* artg4tk_pluginDetectors_gdml_CalorimeterSD_hh */
