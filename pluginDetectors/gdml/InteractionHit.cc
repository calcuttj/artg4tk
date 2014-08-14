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
// InteractionHit.cc: Class representing a simple InteractionHit to be used 
// by Geant4
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#include "artg4tk/pluginDetectors/gdml/InteractionHit.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Circle.hh"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4VisAttributes.hh"

G4Allocator<artg4tk::InteractionHit> artg4tk::InteractionHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::InteractionHit::InteractionHit() {
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::InteractionHit::InteractionHit(G4String pn, G4double p, G4double e,G4double t) {
    pname = pn; // name of secondary particle
    pmom = p;   // momentum of secondary particle
    Ekin = e;   // kinetic energy of secondary particle
    theta = t;  // theta of secondary particle 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::InteractionHit::~InteractionHit() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::InteractionHit::InteractionHit(const InteractionHit& right)
: G4VHit() {

    pname = right.pname;
    pmom = right.pmom;
    Ekin = right.Ekin;
    theta = right.theta;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const artg4tk::InteractionHit& artg4tk::InteractionHit::operator=(const artg4tk::InteractionHit& right) {
    pname = right.pname;
    pmom = right.pmom;
    Ekin = right.Ekin;
    theta = right.theta;
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int artg4tk::InteractionHit::operator==(const artg4tk::InteractionHit& right) const {
    return (this == &right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void artg4tk::InteractionHit::Print() {

    G4cout << "Particle name : " << pname
            << "  momentum [GeV]: " << pmom
            << "kinetic Energy [GeV]"<<Ekin
            << "  theta: " << theta
            << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

