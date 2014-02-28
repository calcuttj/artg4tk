/* ------------------------------------------------------------------------
            |\___/|       
            )     (    
           =\     /=
             )===(
            /     \         CaTS: DRCalorimeter and Tracker Simulation
            |     |         Author: Hans Wenzel (Fermilab)
           /       \
           \       /
            \__  _/
              ( (
               ) )
              (_(
-------------------------------------------------------------------------*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "artg4tk/pluginDetectors/gdml/DRCalorimeterHit.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Circle.hh"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4VisAttributes.hh"

G4Allocator<artg4tk::DRCalorimeterHit> artg4tk::DRCalorimeterHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::DRCalorimeterHit::DRCalorimeterHit() {
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
artg4tk::DRCalorimeterHit::DRCalorimeterHit(G4double ed, G4double eem, G4double enonem, G4int nc,G4ThreeVector p, G4double t) {
    edep = ed;
    edepem = eem;
    nceren = nc;
    edepnonem = enonem;
    pos = p;
    time = t;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::DRCalorimeterHit::~DRCalorimeterHit() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::DRCalorimeterHit::DRCalorimeterHit(const DRCalorimeterHit& right)
: G4VHit() {

    edep = right.edep;
    edepem = right.edepem;
    edepnonem = right.edepnonem;
    pos = right.pos;
    time = right.time;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const artg4tk::DRCalorimeterHit& artg4tk::DRCalorimeterHit::operator=(const artg4tk::DRCalorimeterHit& right) {

    edep = right.edep;
    edepem = right.edepem;
    edepnonem = right.edepnonem;
    pos = right.pos;
    time = right.time;
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int artg4tk::DRCalorimeterHit::operator==(const artg4tk::DRCalorimeterHit& right) const {
    return (this == &right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void artg4tk::DRCalorimeterHit::Draw() {
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

void artg4tk::DRCalorimeterHit::Print() {

    G4cout << "  energy deposit[MeV]: " << edep
            << "  em energy deposit[MeV]: " << edepem
            << "  non em energy deposit[MeV]: " << edepnonem
            << "  position[mm]: " << pos 
            << " Time of first energy deposit: "<< time<<G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

