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
// ClassName:   G4HadronPhysicsQGSP_BERT_HP
//
// Author: 2002 J.P. Wellisch
//
// Modified:
// 15.12.2005 G.Folger: migration to non static particles
// 08.06.2006 V.Ivanchenko: remove stopping
// 20.06.2006 G.Folger: Bertini applies to Kaons, i.e. use SetMinEnergy instead of SetMinPionEnergy
// 25.04.2007 G.Folger: Add code for quasielastic
// 31.10.2012 A.Ribon: Use G4MiscBuilder
// 19.03.2013 A.Ribon: Replace LEP with FTFP
// 19.01.2021 D.Rivera: imported into artg4tk based on :
//            geant4.10.06.p01/source/physics_lists/constructors/hadron_inelastic/src/G4HadronPhysicsQGSP_BERT_HP.cc
//            Provide our own copy of G4NeutronPHPBuilder.
//
//----------------------------------------------------------------------------
//
#include <iomanip>

// -- artg4tk includes
#include "artg4tk/lists/MyG4HadronPhysicsQGSP_BERT_ArHP.hh"
#include "artg4tk/lists/MyG4NeutronPHPBuilder.hh"

//#include "Geant4/G4HadronPhysicsQGSP_BERT_HP.hh"

#include "Geant4/globals.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4SystemOfUnits.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4ParticleTable.hh"

#include "Geant4/G4NeutronBuilder.hh"
#include "Geant4/G4FTFPNeutronBuilder.hh"
#include "Geant4/G4QGSPNeutronBuilder.hh"
#include "Geant4/G4BertiniNeutronBuilder.hh"
//#include "Geant4/G4NeutronPHPBuilder.hh"

#include "Geant4/G4HadronCaptureProcess.hh"
#include "Geant4/G4NeutronRadCapture.hh"
#include "Geant4/G4NeutronCaptureXS.hh"
#include "Geant4/G4ParticleHPCaptureData.hh"
#include "Geant4/G4LFission.hh"
#include "Geant4/G4ProcessVector.hh"
#include "Geant4/G4ProcessManager.hh"

#include "Geant4/G4CrossSectionDataSetRegistry.hh"

#include "Geant4/G4PhysListUtil.hh"

#include "Geant4/G4HadronicParameters.hh"

// factoryGeant4/
#include "Geant4/G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(MyG4HadronPhysicsQGSP_BERT_ArHP);

MyG4HadronPhysicsQGSP_BERT_ArHP::MyG4HadronPhysicsQGSP_BERT_ArHP(G4int)
    :  MyG4HadronPhysicsQGSP_BERT_ArHP("hInelastic MyQGSP_BERT_ArHP")
{}

MyG4HadronPhysicsQGSP_BERT_ArHP::MyG4HadronPhysicsQGSP_BERT_ArHP(const G4String& name, G4bool /*quasiElastic */ )
    :  G4HadronPhysicsQGSP_BERT(name)
{
    minBERT_neutron = 19.9*MeV;
}

void MyG4HadronPhysicsQGSP_BERT_ArHP::Neutron()
{
  auto neu = new G4NeutronBuilder( true ); // Fission on
  AddBuilder(neu);
  auto qgs = new G4QGSPNeutronBuilder(QuasiElasticQGS);
  AddBuilder(qgs);
  qgs->SetMinEnergy(minQGSP_neutron);
  neu->RegisterMe(qgs);
  auto ftf = new G4FTFPNeutronBuilder(QuasiElasticFTF);
  AddBuilder(ftf);
  ftf->SetMinEnergy(minFTFP_neutron);
  ftf->SetMaxEnergy(maxFTFP_neutron);
  neu->RegisterMe(ftf);
  auto bert = new G4BertiniNeutronBuilder;
  AddBuilder(bert);
  bert->SetMinEnergy(minBERT_neutron);
  bert->SetMaxEnergy(maxBERT_neutron);
  neu->RegisterMe(bert);
  auto hp = new MyG4NeutronPHPBuilder;
  AddBuilder(hp);
  neu->RegisterMe(hp);
  neu->Build();
}

void MyG4HadronPhysicsQGSP_BERT_ArHP::ExtraConfiguration()
{
  // --- Neutrons ---
  const G4ParticleDefinition* neutron = G4Neutron::Neutron();
  G4HadronicProcess* capture = G4PhysListUtil::FindCaptureProcess(neutron);
  if (capture) {
    G4NeutronRadCapture* theNeutronRadCapture = new G4NeutronRadCapture();
    theNeutronRadCapture->SetMinEnergy( minBERT_neutron );
    capture->RegisterMe( theNeutronRadCapture );
  }
  G4HadronicProcess* fission = G4PhysListUtil::FindFissionProcess(neutron);
  if (fission) {
    G4LFission* theNeutronLEPFission = new G4LFission();
    theNeutronLEPFission->SetMinEnergy( minBERT_neutron );
    theNeutronLEPFission->SetMaxEnergy( G4HadronicParameters::Instance()->GetMaxEnergy() );
    fission->RegisterMe( theNeutronLEPFission );
  }
}
