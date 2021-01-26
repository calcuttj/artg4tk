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
// $Id: G4BertiniNeutronBuilder.cc 66892 2013-01-17 10:57:59Z gunter $
//
//---------------------------------------------------------------------------
//
// ClassName:   MyG4BertiniNeutronBuilder
//
// Author: 2002 H.P. Wellisch
//
// Modified:
// 02.04.2009 V.Ivanchenko remove add cross section, string builderis reponsible
// 30.03.2020 D. Rivera copy based on:
//    geant4.10.03.p03/source/physics_lists/builders/src/G4BertiniNeutronBuilder.cc
//
//----------------------------------------------------------------------------
//

#include "MyG4BiasedBertiniNeutronBuilder.hh"
//#include "Geant4/G4BertiniNeutronBuilder.hh"
#include "Geant4/G4SystemOfUnits.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ProcessManager.hh"

MyG4BiasedBertiniNeutronBuilder::
MyG4BiasedBertiniNeutronBuilder()
{
  theMin = 0;
  theMax = 9.9*GeV;
  theXSMultiplier = 1.0; // -- no bias
  theModel = new G4CascadeInterface;
}

MyG4BiasedBertiniNeutronBuilder::
MyG4BiasedBertiniNeutronBuilder( G4double XSMultiplier )
{
  theMin = 0;
  theMax = 9.9*GeV;
  G4cout << "(MyG4BiasedBertiniNeutronBuilder) theBertiniNeutronBuilder was passed XSMultiplier =  " << XSMultiplier << G4endl;
  theXSMultiplier = XSMultiplier;
  theModel = new G4CascadeInterface;
}

void MyG4BiasedBertiniNeutronBuilder::
Build(G4NeutronInelasticProcess * aP )
{
  theModel->SetMinEnergy(theMin);
  theModel->SetMaxEnergy(theMax);
  G4cout << "(MyG4BiasedBertiniNeutronBuilder) Multiplying the G4NeutronInelastic XS by : " << theXSMultiplier << G4endl;
  aP->MultiplyCrossSectionBy(theXSMultiplier);
  aP->RegisterMe(theModel);
}

MyG4BiasedBertiniNeutronBuilder::
~MyG4BiasedBertiniNeutronBuilder()
{
}

void MyG4BiasedBertiniNeutronBuilder::
Build(G4HadronElasticProcess * )
{
}

void MyG4BiasedBertiniNeutronBuilder::
Build(G4HadronFissionProcess * )
{
}

void MyG4BiasedBertiniNeutronBuilder::
Build(G4HadronCaptureProcess * )
{
}

// 2002 by J.P. Wellisch
