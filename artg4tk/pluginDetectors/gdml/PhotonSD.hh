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
#ifndef PhotonSD_h
#define PhotonSD_h 1

#include "Geant4/G4VSensitiveDetector.hh"
#include "artg4tk/pluginDetectors/gdml/PhotonHit.hh"

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {
  class PhotonSD : public G4VSensitiveDetector {
  public:
    PhotonSD(G4String);
    ~PhotonSD();

    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    const PhotonHitCollection&
    GetHits() const
    {
      return hitCollection;
    }

  private:
    PhotonHitCollection hitCollection;
  };
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
