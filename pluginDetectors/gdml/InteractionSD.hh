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
#ifndef InteractionSD_h
#define InteractionSD_h 1

#include "Geant4/G4VSensitiveDetector.hh"
#include "artg4tk/pluginDetectors/gdml/ParticleChange.hh"
#include "artg4tk/pluginDetectors/gdml/InteractionHit.hh"

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {
class InteractionSD : public G4VSensitiveDetector {
public:
    InteractionSD(G4String);
    ~InteractionSD();

    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);

private:
    InteractionHitsCollection* interactionCollection;
    G4int HCID;
    ParticleChange* fFirstInter;
    ParticleChange* fOtherInter;

};
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

