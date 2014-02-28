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

#ifndef PhotonHit_h
#define PhotonHit_h 1

#include "Geant4/G4VHit.hh"
#include "Geant4/G4THitsCollection.hh"
#include "Geant4/G4Allocator.hh"
#include "Geant4/G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {

    class PhotonHit : public G4VHit {
    private:

        G4int processID;
        G4double edep;
        G4ThreeVector pos;
        G4double time;

    public:

        PhotonHit();
        PhotonHit(G4int id, G4double e, G4ThreeVector p, G4double t);
        ~PhotonHit();
        PhotonHit(const PhotonHit&);
        const PhotonHit& operator=(const PhotonHit&);
        G4int operator==(const PhotonHit&) const;

        inline void* operator new(size_t);
        inline void operator delete(void*);

        void Draw();
        void Print();

        void SetProcessID(G4int track) {
            processID = track;
        };

        void SetEdep(G4double de) {
            edep = de;
        };

        void SetPos(G4ThreeVector xyz) {
            pos = xyz;
        };

        void SetTime(G4double de) {
            time = de;
        };

        G4int GetProcessID() {
            return processID;
        };

        G4double GetEdep() {
            return edep;
        };

        G4ThreeVector GetPos() {
            return pos;
        };

        G4double GetTime() {
            return time;
        };

    };

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    typedef G4THitsCollection<PhotonHit> PhotonHitsCollection;

    extern G4Allocator<PhotonHit> PhotonHitAllocator;

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    inline void* PhotonHit::operator new(size_t) {
        void *aHit;
        aHit = (void *) PhotonHitAllocator.MallocSingle();
        return aHit;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    inline void PhotonHit::operator delete(void *aHit) {
        PhotonHitAllocator.FreeSingle((PhotonHit*) aHit);
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}
#endif
