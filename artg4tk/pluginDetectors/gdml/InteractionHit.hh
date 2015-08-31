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
// InteractionHit.hh: Class representing a simple InteractionHit to be used 
// by Geant4
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef InteractionHit_h
#define InteractionHit_h 1
#include "Geant4/G4VHit.hh"
#include "Geant4/G4THitsCollection.hh"
#include "Geant4/G4Allocator.hh"
#include "Geant4/G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {

    class InteractionHit : public G4VHit {
    private:
        G4String pname; // name of secondary particle
        G4double pmom; // momentum of secondary particle
        G4double Ekin; // kinetic energy of secondary particle
        G4double theta; // theta of secondary particle  

    public:

        InteractionHit();
        InteractionHit(G4String n, G4double m, G4double e,G4double t);
        ~InteractionHit();
        InteractionHit(const InteractionHit&);
        const InteractionHit& operator=(const InteractionHit&);
        G4int operator==(const InteractionHit&) const;

        inline void* operator new(size_t);
        inline void operator delete(void*);

        void Print();

        void SetPname(G4String de) {
            pname = de;
        };

        void SetPmom(G4double de) {
            pmom = de;
        };

        void SetEkin(G4double de) {
            Ekin = de;
        };

        void SetTheta(G4double de) {
            theta = de;
        };

        G4String GetPname() {
            return pname;
        };

        G4double GetPmom() {
            return pmom;
        };

        G4double GetEkin() {
            return Ekin;
        };

        G4double GetTheta() {
            return theta;
        };
    };

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    typedef G4THitsCollection<InteractionHit> InteractionHitsCollection;

    extern G4Allocator<InteractionHit> InteractionHitAllocator;

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    inline void* InteractionHit::operator new(size_t) {
        void *aHit;
        aHit = (void *) InteractionHitAllocator.MallocSingle();
        return aHit;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    inline void InteractionHit::operator delete(void *aHit) {
        InteractionHitAllocator.FreeSingle((InteractionHit*) aHit);
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}
#endif
