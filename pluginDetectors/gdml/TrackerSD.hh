/* ------------------------------------------------------------------------
            |\___/|       
            )     (    
           =\     /=
             )===(
            /     \         CaTS: Calorimeter and Tracker Simulation
            |     |         Author: Hans Wenzel (Fermilab)
           /       \
           \       /
            \__  _/
              ( (
               ) )
              (_(
-------------------------------------------------------------------------*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef TrackerSD_h
#define TrackerSD_h 1

#include "Geant4/G4VSensitiveDetector.hh"
#include "artg4tk/pluginDetectors/gdml/TrackerHit.hh"

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {

    class TrackerSD : public G4VSensitiveDetector {
    public:
        TrackerSD(G4String);
        ~TrackerSD();

        void Initialize(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        //      void EndOfEvent(G4HCofThisEvent*);

    private:
        TrackerHitsCollection* trackerCollection;
        G4int HCID;

    };

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}
#endif

