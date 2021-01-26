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
// $Id: HadronPhysicsQGSP_BERT_HP.hh 93878 2015-11-03 08:18:00Z gcosmo $
//
//---------------------------------------------------------------------------
//
// ClassName:   MyG4HadronPhysicsQGSP_BERT_HP
//
// Author: 2002 J.P. Wellisch
//
// Modified:
// 15.12.2005 G.Folger: migration to non static particles
// 25.04.2007 G.Folger: Add quasielastic as option, use quasielastic by default
// 31.10.2012 A.Ribon: Use G4MiscBuilder
// 19.03.2013 A.Ribon: Replace LEP with FTFP
// 17.10.2019 D.Rivera: adapt in larg4. Copy based on:
//    geant4.10.03.p03/source/physics_lists/constructors/hadron_inelastic/include/G4HadronPhysicsQGSP_BERT_HP.hh
//
//----------------------------------------------------------------------------
//
#ifndef MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias_h
#define MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias_h 1

///#define G4CASCADE_DEBUG_INTERFACE 1

#include "Geant4/globals.hh"
#include "Geant4/G4ios.hh"

#include "Geant4/G4VPhysicsConstructor.hh"

// -- Pion and Kaon builders
#include "Geant4/G4PiKBuilder.hh"
#include "Geant4/G4FTFPPiKBuilder.hh"
#include "Geant4/G4QGSPPiKBuilder.hh"
#include "Geant4/G4BertiniPiKBuilder.hh"

// -- Proton builders
#include "Geant4/G4ProtonBuilder.hh"
#include "Geant4/G4FTFPProtonBuilder.hh"
#include "Geant4/G4QGSPProtonBuilder.hh"
#include "Geant4/G4BertiniProtonBuilder.hh"

// -- Neutron builders
#include "Geant4/G4NeutronBuilder.hh"
#include "Geant4/G4FTFPNeutronBuilder.hh"
#include "Geant4/G4QGSPNeutronBuilder.hh"
#include "Geant4/G4BertiniNeutronBuilder.hh"
//#include "MyG4BiasedBertiniNeutronBuilder.hh"
#include "Geant4/G4NeutronPHPBuilder.hh"

// -- Other builders
#include "Geant4/G4HyperonFTFPBuilder.hh"
#include "Geant4/G4AntiBarionBuilder.hh"
#include "Geant4/G4FTFPAntiBarionBuilder.hh"

class G4ComponentGGHadronNucleusXsc;


class MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias : public G4VPhysicsConstructor
{
  public:
    MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias(G4int verbose =2);
    MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias(const G4String& name, G4bool quasiElastic=true);
    MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias(const G4String& name, G4bool quasiElastic=true, G4double XSMultiplier=1.0);
    virtual ~MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias();

  public:
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    void CreateModels();

    struct ThreadPrivate {
      G4NeutronBuilder * theNeutrons;
      G4FTFPNeutronBuilder * theFTFPNeutron;
      G4QGSPNeutronBuilder * theQGSPNeutron;
      G4BertiniNeutronBuilder * theBertiniNeutron;
      //MyG4BiasedBertiniNeutronBuilder * theBertiniNeutron;
      G4NeutronPHPBuilder * theHPNeutron;

      G4PiKBuilder * thePiK;
      G4FTFPPiKBuilder * theFTFPPiK;
      G4QGSPPiKBuilder * theQGSPPiK;
      G4BertiniPiKBuilder * theBertiniPiK;

      G4ProtonBuilder * thePro;
      G4FTFPProtonBuilder * theFTFPPro;
      G4QGSPProtonBuilder * theQGSPPro;
      G4BertiniProtonBuilder * theBertiniPro;

      G4HyperonFTFPBuilder * theHyperon;

      G4AntiBarionBuilder * theAntiBaryon;
      G4FTFPAntiBarionBuilder * theFTFPAntiBaryon;

      G4ComponentGGHadronNucleusXsc * xsKaon;
      G4VCrossSectionDataSet * xsNeutronCaptureXS;
    };
    static G4ThreadLocal ThreadPrivate* tpdata;

    // G4bool QuasiElastic;
    G4double theBertiniNeutronXSMultiplier;
};

// 2019 by D. Rivera

#endif

