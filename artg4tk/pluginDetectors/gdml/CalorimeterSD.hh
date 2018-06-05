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
#ifndef CalorimeterSD_h
#define CalorimeterSD_h 1

#include "Geant4/G4VSensitiveDetector.hh"
#include "artg4tk/pluginDetectors/gdml/CalorimeterHit.hh"

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {
class CalorimeterSD : public G4VSensitiveDetector {
public:
  CalorimeterSD(G4String);
  ~CalorimeterSD();
  
  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  const CalorimeterHitCollection& GetHits() const { return calorimeterCollection; }
private:
    CalorimeterHitCollection calorimeterCollection;
    G4int HCID;

};
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

