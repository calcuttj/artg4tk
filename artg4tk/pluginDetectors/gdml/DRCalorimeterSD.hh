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
// DRCalorimeterSD.hh:
// implementation of a Dual read out sensitive Detector which registers
// both ionization and Cerenkov contributions in a calorimeter cell
//
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef artg4tk_pluginDetectors_gdml_DRCalorimeterSD_hh
#define artg4tk_pluginDetectors_gdml_DRCalorimeterSD_hh

#include "Geant4/G4VSensitiveDetector.hh"
#include "artg4tk/pluginDetectors/gdml/ByParticle.hh"
#include "artg4tk/pluginDetectors/gdml/DRCalorimeterHit.hh"
class G4Step;
class G4HCofThisEvent;

namespace artg4tk {

  class DRCalorimeterSD : public G4VSensitiveDetector {
  private:
    DRCalorimeterHitCollection drcalorimeterCollection;
    ByParticle EbyParticle; // Energy deposited by particle type
    double TotalE;
    ByParticle NCerenbyParticle;
    double TotalNCeren;

  public:
    explicit DRCalorimeterSD(G4String);

    void Initialize(G4HCofThisEvent*) override;
    void EndOfEvent(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;

    DRCalorimeterHitCollection const&
    GetHits() const
    {
      return drcalorimeterCollection;
    }

    ByParticle const&
    GetEbyParticle() const
    {
      return EbyParticle;
    };

    ByParticle const&
    GetNCerenbyParticle()
    {
      return NCerenbyParticle;
    };

    double
    GetTotalE() const
    {
      return TotalE;
    }

    double
    GetTotalNCeren() const
    {
      return TotalNCeren;
    }
  };
}

#endif /* artg4tk_pluginDetectors_gdml_DRCalorimeterSD_hh */
