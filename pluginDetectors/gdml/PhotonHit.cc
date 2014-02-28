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

#include "artg4tk/pluginDetectors/gdml/PhotonHit.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Circle.hh"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4VisAttributes.hh"

G4Allocator<artg4tk::PhotonHit> artg4tk::PhotonHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::PhotonHit::PhotonHit() {
}

artg4tk::PhotonHit::PhotonHit(G4int id, G4double e, G4ThreeVector p, G4double t) {

    processID = id;
    edep = e;
    pos = p;
    time = t;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::PhotonHit::~PhotonHit() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::PhotonHit::PhotonHit(const PhotonHit& right)
: G4VHit() {
    processID = right.processID;
    edep = right.edep;
    pos = right.pos;
    time = right.time;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const artg4tk::PhotonHit& artg4tk::PhotonHit::operator=(const PhotonHit& right) {
    processID = right.processID;
    edep = right.edep;
    pos = right.pos;
    time = right.time;
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int artg4tk::PhotonHit::operator==(const PhotonHit& right) const {
    return (this == &right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void artg4tk::PhotonHit::Draw() {
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (pVVisManager) {
        G4Circle circle(pos);
        circle.SetScreenSize(2.);
        circle.SetFillStyle(G4Circle::filled);
        G4Colour colour(1., 0., 0.);
        G4VisAttributes attribs(colour);
        circle.SetVisAttributes(attribs);
        pVVisManager->Draw(circle);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void artg4tk::PhotonHit::Print() {
    G4cout << "  processID: " << processID
            << "  energy deposit[MeV]: " << edep
            << "  position[mm]: " << pos
            << "  time[nsec]: " << time
            << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

