//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
// P. Arce, June-2014 Conversion neutron_hp to particle_hp
//
#ifndef ArParticleHPCaptureFS_h
#define ArParticleHPCaptureFS_h 1

// -- artg4tk includes
#include "artg4tk/lists/ArCaptureGammas.hh"

#include "Geant4/globals.hh"
#include "Geant4/G4HadProjectile.hh"
#include "Geant4/G4HadFinalState.hh"
#include "Geant4/G4ParticleHPFinalState.hh"
#include "Geant4/G4ReactionProductVector.hh"
#include "Geant4/G4ParticleHPNames.hh"
#include "Geant4/G4ParticleHPPhotonDist.hh"
#include "Geant4/G4ParticleHPEnAngCorrelation.hh"

class ArParticleHPCaptureFS : public G4ParticleHPFinalState
{
  public:
  bool useArCapGamma = true;

  ArParticleHPCaptureFS()
  {
    hasXsec = false;
    hasExactMF6 = false;
    targetMass = 0;
  }

  ~ArParticleHPCaptureFS()
  {
  }

  void Init (G4double A, G4double Z, G4int M, G4String & dirName, G4String & aFSType, G4ParticleDefinition* );
  G4HadFinalState * ApplyYourself(const G4HadProjectile & theTrack);
  G4ParticleHPFinalState * New()
  {
   ArParticleHPCaptureFS * theNew = new ArParticleHPCaptureFS;
   return theNew;
  }

  private:

  G4double targetMass;

  G4ParticleHPPhotonDist theFinalStatePhotons;
  ArCaptureGammas       theFinalgammas;

   G4ParticleHPEnAngCorrelation theMF6FinalState;
   G4bool hasExactMF6;

  G4ParticleHPNames theNames;

//  G4double theCurrentA;
//  G4double theCurrentZ;
};
#endif
