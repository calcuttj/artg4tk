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
#ifndef DRCalorimeterSD_h
#define DRCalorimeterSD_h

#include "Geant4/G4VSensitiveDetector.hh"
#include "artg4tk/pluginDetectors/gdml/DRCalorimeterHit.hh"
#include "artg4tk/pluginDetectors/gdml/ByParticle.hh"
class G4Step;
class G4HCofThisEvent;

//class Cerenkov;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {

    class DRCalorimeterSD : public G4VSensitiveDetector {
    private:
        DRCalorimeterHitCollection drcalorimeterCollection;
        ByParticle EbyParticle; // Energy deposited by particle type
        double TotalE;
        ByParticle NCerenbyParticle;
        double TotalNCeren;
    public:
        DRCalorimeterSD(G4String);
        ~DRCalorimeterSD();
        void Initialize(G4HCofThisEvent*);
        void EndOfEvent(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step*, G4TouchableHistory*);

        const DRCalorimeterHitCollection& GetHits() const {
            return drcalorimeterCollection;
        }

        const ByParticle GetEbyParticle() {
            return EbyParticle;
        };

        const ByParticle GetNCerenbyParticle() {
            return NCerenbyParticle;
        };

        double GetTotalE() {
            return TotalE;
        }

        double GetTotalNCeren() {
            return TotalNCeren;
        }
    };
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
