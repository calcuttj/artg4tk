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
// Cerenkov.hh: 
// Author: Hans Wenzel (Fermilab)
// Created on March 11, 2014, 2:27 PM
//=============================================================================
#ifndef CERENKOV_HH
#define	CERENKOV_HH
#include <vector>
#include "Geant4/G4VSensitiveDetector.hh"
#include "Geant4/G4PhysicsOrderedFreeVector.hh"
#include "Geant4/G4Material.hh" 
#include "Geant4/G4MaterialPropertyVector.hh"
#include "artg4tk/pluginDetectors/gdml/Cerenkov.hh"
class G4Step;
class G4HCofThisEvent;
//class Cerenkov;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {
    class Cerenkov {
    public:
        Cerenkov();
        ~Cerenkov();
        void Initialize();
        G4double GetAverageNumberOfPhotons(const G4double, const G4double, const G4String) const;
    private:
        std::vector<G4String> CAI;
        std::vector<G4PhysicsOrderedFreeVector*> CerenkovAngleIntegrals;
        std::vector<G4MaterialPropertyVector*> RefractionIndeces;
    };
}

#endif	/* CERENKOV_HH */

