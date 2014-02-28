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
// CalorimeterHit.hh: Class representing a simple CalorimeterHit to be used 
// by Geant4
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef CalorimeterHit_h
#define CalorimeterHit_h 1
#include "Geant4/G4VHit.hh"
#include "Geant4/G4THitsCollection.hh"
#include "Geant4/G4Allocator.hh"
#include "Geant4/G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {
class CalorimeterHit : public G4VHit
{
      private:

      G4double      edep;     // total energy deposit 
      G4double      edepem;   // energy deposit by e+/-, gamma 
      G4double      edepnonem;// energy deposit by non e+/-, gamma 
      G4ThreeVector pos;      // position of calorimeter cell
      G4double      time;     // time of first energy deposit
  public:

      CalorimeterHit();
      CalorimeterHit(G4double e, G4double eem, G4double enonem, G4ThreeVector p, G4double t);
     ~CalorimeterHit();
      CalorimeterHit(const CalorimeterHit&);
      const CalorimeterHit& operator=(const CalorimeterHit&);
      G4int operator==(const CalorimeterHit&) const;

      inline void* operator new(size_t);
      inline void  operator delete(void*);

      void Draw();
      void Print();

      void SetEdep(G4double de)      { edep = de; };
      void SetEdepEM(G4double de)      { edepem = de; };
      void SetEdepnonEM(G4double de)      { edepnonem = de; };      
      void SetPos      (G4ThreeVector xyz){ pos = xyz; };
      void SetTime(G4double de)      { time = de; };
            
      G4double GetEdep()    { return edep; }; 
      G4double GetEdepEM()    { return edepem; }; 
      G4double GetEdepnonEM()    { return edepnonem; };       
      G4ThreeVector GetPos(){ return pos; };
      G4double GetTime()    { return time; }; 
            
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<CalorimeterHit> CalorimeterHitsCollection;

extern G4Allocator<CalorimeterHit> CalorimeterHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* CalorimeterHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) CalorimeterHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void CalorimeterHit::operator delete(void *aHit)
{
  CalorimeterHitAllocator.FreeSingle((CalorimeterHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}
#endif
