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
// nobleGasTPCSD.hh: Class representing a sensitive tracking detector
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef nobleGasTPCSD_h
#define nobleGasTPCSD_h 1

#include "Geant4/G4VSensitiveDetector.hh"
#include "artg4tk/pluginDetectors/gdml/nobleGasTPCHit.hh"
class G4Step;
class G4HCofThisEvent;
class nobleGasTPCHitCollection;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {

    class nobleGasTPCSD : public G4VSensitiveDetector {
    public:
        nobleGasTPCSD(G4String);
        ~nobleGasTPCSD();
        void Initialize(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      const nobleGasTPCHitCollection& GetHits() const { return hitCollection; }
    private:
        nobleGasTPCHitCollection hitCollection;
        G4int HCID;

    };

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}
#endif

