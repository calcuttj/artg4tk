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
#ifndef HadInteractionSD_h
#define HadInteractionSD_h 1

#include "Geant4/G4VSensitiveDetector.hh"
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkVtx.hh"

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {

class HadInteractionSD : public G4VSensitiveDetector 
{

public:

   // ctor & dtor
   HadInteractionSD(G4String);
   ~HadInteractionSD();

   virtual void   Initialize( G4HCofThisEvent* );
   virtual G4bool ProcessHits( G4Step*, G4TouchableHistory* );
   virtual void   clear() { fFirstInter.Clear(); }
   
   // specific to this SD
   //
   void SetStopAfter1stInt( bool flag=true ) { fStopAfter1stInt=flag; return; }
   const ArtG4tkVtx& Get1stInteraction() const { return fFirstInter; }

private:
    ArtG4tkVtx     fFirstInter;
    bool           fStopAfter1stInt;
};
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

