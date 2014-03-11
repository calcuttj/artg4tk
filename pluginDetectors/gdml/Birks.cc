//
//               __        __ __  __  __  
//   ____ ______/ /_____ _/ // / / /_/ /__
//  / __ `/ ___/ __/ __ `/ // /_/ __/ //_/
// / /_/ / /  / /_/ /_/ /__  __/ /_/ ,<   
// \__,_/_/   \__/\__, /  /_/  \__/_/|_|  
//               /____/                  
//
// artg4tk: art based Geant 4 Toolkit
//=============================================================================
// 
//                Birks' Law 
//                ===========
//    
//                      In the case of Scintillator as active medium, we can
//              describe the quenching effects with the Birks' law,
//              using the expression and the coefficients taken from
//              the paper NIM 80 (1970) 239-244 for the organic
//              scintillator NE-102:
//                                     S*dE/dr
//                    dL/dr = -----------------------------------
//                               1 + C1*(dE/dr)
//              with:
//                    S=1
//                    C1 = 1.29 x 10^-2  g*cm^-2*MeV^-1
//                    C2 = 9.59 x 10^-6  g^2*cm^-4*MeV^-2
//              These are the same values used by ATLAS TileCal
//              and CMS HCAL (and also the default in Geant3).
//              You can try different values for these parameters,
//              to have an idea on the uncertainties due to them,
//              by uncommenting one of the lines below.
//              To get the "dE/dr" that appears in the formula,
//              which has the dimensions
//                    [ dE/dr ] = MeV * cm^2 / g
//              we have to divide the energy deposit in MeV by the
//              product of the step length (in cm) and the density
//              of the scintillator:
//                    rho_scintillator = 1.032  g/cm3 .
//              Of course, in our case we use only the denominator
//              of the Birks' formula above, because we do not have
//              digitization, i.e. we only have energy deposit but
//              not conversion to photons.
//              Birks should not be applied in the case of gamma
//              energy depositions (which happens only for the
//              photoelectric process), because in this case the
//              step length is related to the photoelectric cross
//              section, and not to the distance in which the
//              energy is actually deposited, that is what is
//              needed in order to determine dE/dx which enters
//              in the Birks' law.
//              Similarly, for neutron energy depositions (which
//              have been introduced in Geant4 8.1 as an effective
//              way to describe the elastic nuclei recoils below
//              a certain kinetic threshold, set by default to
//              100 keV), the step length is related to the neutron
//              elastic cross-section, and not to the real ionization
//              range of the recoiled nuclei, which should instead
//              be considered for the dE/dx in the Birks' law.
//              In the case of neutron energy depositions, the
//              most correct way to apply the Birks quench would
//              be to eliminate them by setting the kinetic
//              threshold to 0.0 (variable  "edepLimit"  in the
//              file  "G4HadronElasticPhysics.cc"  in
//               "geant4/physics_lists/hadronic/Packaging/src/" ),
//              so that the recoiled nuclei tracks are always
//              generated explicitly. This, of course, costs in
//              term of CPU performance.

// File:   Birks.cc
// Class is used to calculate Birks suppression
// Author: Hans Wenzel (Fermilab)
// Created on March 11, 2014, 3:11 PM
//=============================================================================

#include "artg4tk/pluginDetectors/gdml/Birks.hh"
namespace artg4tk {

    Birks::Birks(): birksc1(1.29e-2),birksc2(9.59e-6){ 
    }


    Birks::~Birks() {
    }

    G4double Birks::GetBirkssuppressedEdep(G4Step* aStep) {
        G4double edep = aStep->GetTotalEnergyDeposit() / CLHEP::MeV;
        if (edep == 0.) return false;
        G4double rho = aStep->GetPreStepPoint()->GetMaterial()->GetDensity() / (CLHEP::g / CLHEP::cm3);
        G4double stepLength = aStep->GetStepLength() / CLHEP::cm;
        G4double birksFactor = 1.0;
        G4double dedx;
        //Do not apply Birks for gamma deposits!
        if (stepLength > 1.0e-8)//Check, cut if necessary.
        {
            dedx = edep / (rho * stepLength); //[MeV*cm^2/g]
            birksFactor = 1.0 / (1.0 + birksc1 * dedx + birksc2 * dedx * dedx);
            if (aStep->GetTrack()->GetDefinition() == G4Gamma::GammaDefinition()) {
                birksFactor = 1.0;
            }
            if (aStep->GetTrack()->GetDefinition() == G4Neutron::NeutronDefinition()) {
                birksFactor = 1.0;
            }
        }

        G4double eobsbirks = edep*birksFactor;
        return eobsbirks;
    }
}