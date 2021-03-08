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
//---------------------------------------------------------------------------
//
// 19.01.2021 D.Rivera: imported into artg4tk based on :
//            geant4.10.06.p01/source/physics_lists/builder/include/G4NeutronPHPBuilder.hh
//            Provide our own version of the G4ParticleCapture class for Argon.
//
//----------------------------------------------------------------------------
//
#ifndef MyG4NeutronPHPBuilder_h
#define MyG4NeutronPHPBuilder_h 1

// -- artg4tk includes
#include "artg4tk/lists/ArParticleHPCapture.hh"

#include "Geant4/globals.hh"

#include "Geant4/G4HadronElasticProcess.hh"
#include "Geant4/G4HadronFissionProcess.hh"
#include "Geant4/G4HadronCaptureProcess.hh"
#include "Geant4/G4NeutronInelasticProcess.hh"
#include "Geant4/G4VNeutronBuilder.hh"

#include "Geant4/G4ParticleHPElasticData.hh"
#include "Geant4/G4ParticleHPElastic.hh"
#include "Geant4/G4ParticleHPInelastic.hh"
#include "Geant4/G4ParticleHPInelasticData.hh"
#include "Geant4/G4ParticleHPFission.hh"
#include "Geant4/G4ParticleHPFissionData.hh"
//#include "Geant4/G4ParticleHPCapture.hh"
#include "Geant4/G4ParticleHPCaptureData.hh"

class MyG4NeutronPHPBuilder : public G4VNeutronBuilder
{
  public:
    MyG4NeutronPHPBuilder();
    virtual ~MyG4NeutronPHPBuilder() {}

  public:
    virtual void Build(G4HadronElasticProcess * aP) final override;
    virtual void Build(G4HadronFissionProcess * aP) final override;
    virtual void Build(G4HadronCaptureProcess * aP) final override;
    virtual void Build(G4NeutronInelasticProcess * aP) final override;

    virtual void SetMinEnergy(G4double aM) final override
    {
      theMin=aM;
      theIMin = theMin;
    }
    void SetMinInelasticEnergy(G4double aM)
    {
      theIMin=aM;
    }
    virtual void SetMaxEnergy(G4double aM) final override
    {
      theIMax = aM;
      theMax=aM;
    }
    void SetMaxInelasticEnergy(G4double aM)
    {
      theIMax = aM;
    }

    using G4VNeutronBuilder::Build; //Prevent compiler warning

  private:

    G4double theMin;
    G4double theIMin;
    G4double theMax;
    G4double theIMax;

    G4ParticleHPElastic * theHPElastic;
    G4ParticleHPElasticData * theHPElasticData;
    G4ParticleHPInelastic * theHPInelastic;
    G4ParticleHPInelasticData * theHPInelasticData;
    G4ParticleHPFission * theHPFission;
    G4ParticleHPFissionData * theHPFissionData;
    ArParticleHPCapture * theHPCapture;
    G4ParticleHPCaptureData * theHPCaptureData;

};

#endif

