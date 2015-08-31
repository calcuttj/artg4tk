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
// File:   Birks.hh
// Class is used to calculate Birks suppression
// Author: Hans Wenzel (Fermilab)
// Created on March 11, 2014, 3:01 PM
//=============================================================================

#ifndef BIRKS_HH
#define	BIRKS_HH
#include "Geant4/G4Step.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4ParticleTypes.hh"
namespace artg4tk {

    class Birks {
    public:
                Birks();
        ~Birks();
    private:
        G4double birksc1;
        G4double birksc2;
    public:

        G4double Getbirksc1() {
            return birksc1;
        };
        G4double Getbirksc2() {
            return birksc1;
        };
        void Setbirksc1(G4double de) {
            birksc1 = de;
        };
        void Setbirksc2(G4double de) {
            birksc2 = de;
        };
        G4double GetBirkssuppressedEdep(G4Step*);
    };
}
#endif	/* BIRKS_HH */

