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
// ClassName:   G4BertiniPiKBuilder
//
// Author: 2002 H.P. Wellisch
//
// Modified:
// 02.04.2009 V.Ivanchenko remove add cross section, string builderis reponsible 
// 12.04.2017 A.Dotti move to new design with base class
//
//----------------------------------------------------------------------------
//
#include "G4BertiniPiKBuilderBias.hh"
#include "Geant4/G4SystemOfUnits.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ProcessManager.hh"
#include "Geant4/G4BGGPionInelasticXS.hh"
#include "Geant4/G4ComponentGGHadronNucleusXsc.hh"
#include "Geant4/G4CrossSectionInelastic.hh"
#include "Geant4/G4HadronicParameters.hh"


G4BertiniPiKBuilderBias::
G4BertiniPiKBuilderBias(double pion_plus_bias) : fPionPlusBias(pion_plus_bias)
 {
   kaonxs = new G4CrossSectionInelastic( new G4ComponentGGHadronNucleusXsc );
   theMin = 0.0;
   theMax = G4HadronicParameters::Instance()->GetMaxEnergyTransitionFTF_Cascade();
   theModel = new G4CascadeInterface;
   theModel->SetMinEnergy(theMin);
   theModel->SetMaxEnergy(theMax); 
 }

void G4BertiniPiKBuilderBias::
Build(G4PionPlusInelasticProcess * aP)
 {
   theModel->SetMinEnergy(theMin);
   theModel->SetMaxEnergy(theMax);
   aP->AddDataSet( new G4BGGPionInelasticXS( G4PionPlus::Definition() ) );
   std::cout << "Biasing: " << fPionPlusBias << std::endl;
   aP->MultiplyCrossSectionBy(fPionPlusBias);
   aP->RegisterMe(theModel);
 }

void G4BertiniPiKBuilderBias::
Build(G4PionMinusInelasticProcess * aP)
 {
   theModel->SetMinEnergy(theMin);
   theModel->SetMaxEnergy(theMax);
   aP->AddDataSet( new G4BGGPionInelasticXS( G4PionMinus::Definition() ) );
   aP->RegisterMe(theModel);
 }

void G4BertiniPiKBuilderBias::
Build(G4KaonPlusInelasticProcess * aP)
 {
   aP->RegisterMe(theModel);
   theModel->SetMinEnergy(theMin);
   theModel->SetMaxEnergy(theMax);
   aP->AddDataSet(kaonxs);
 }

void G4BertiniPiKBuilderBias::
Build(G4KaonMinusInelasticProcess * aP)
 {
   aP->RegisterMe(theModel);
   theModel->SetMinEnergy(theMin);
   theModel->SetMaxEnergy(theMax);
   aP->AddDataSet(kaonxs);
 }

void G4BertiniPiKBuilderBias::
Build(G4KaonZeroLInelasticProcess * aP)
 {
   aP->RegisterMe(theModel);
   theModel->SetMinEnergy(theMin);
   theModel->SetMaxEnergy(theMax);
   aP->AddDataSet(kaonxs);
 }

void G4BertiniPiKBuilderBias::
Build(G4KaonZeroSInelasticProcess * aP)
 {
   aP->RegisterMe(theModel);
   theModel->SetMinEnergy(theMin);
   theModel->SetMaxEnergy(theMax);
   aP->AddDataSet(kaonxs);
 }

