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
// ClassName:   QGSP_BERT
//
// Author: 2002 J.P. Wellisch
//
// Modified:
// 25.11.2005 G.Folger: migration to non static particles
// 30.11.2005 G.Folger: Register EmStandard first, split Em Standard and Extra
// 02.12.2005 V.Ivanchenko: rename components
// 08.06.2006 V.Ivanchenko: migration to CHIPS stopping
// 15.06.2006 G.Folger: Migrate to HadronElasticPhysics using improved elastic
// 20.11.2006 G.Folger: add Tracking Cut for neutrons
// 26.04.2007 G.Folger: Enable quasielastic for QGS string model
// 16.05.2007 V.Ivanchenko: rename EM builders
// 04.06.2010 G.Folger: Use new ctor for builders
// 16.08.2010 H.Kurashige: Remove inclusion of G4ParticleWithCuts 
// 16.10.2012 A.Ribon: Renamed the physics classes used
//
//----------------------------------------------------------------------------
//

#include <iomanip>   
#include <CLHEP/Units/SystemOfUnits.h>

#include "Geant4/globals.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4ProcessManager.hh"
#include "Geant4/G4ProcessVector.hh"
#include "Geant4/G4ParticleTypes.hh"
#include "Geant4/G4ParticleTable.hh"

#include "Geant4/G4Material.hh"
#include "Geant4/G4MaterialTable.hh"

#include "Geant4/G4DecayPhysics.hh"
#include "Geant4/G4EmStandardPhysics.hh"
#include "Geant4/G4EmExtraPhysics.hh"
#include "Geant4/G4IonPhysics.hh"
#include "Geant4/G4StoppingPhysics.hh"
#include "G4HadronElasticPhysicsBias.hh"
#include "Geant4/G4NeutronTrackingCut.hh"

#include "QGSP_BERT_Bias.hh"
#include "G4HadronPhysicsQGSP_BERT_Bias.hh"

#include "Geant4/G4PhysListStamper.hh"
G4_DECLARE_PHYSLIST_FACTORY(QGSP_BERT_Bias);

QGSP_BERT_Bias::QGSP_BERT_Bias(G4int ver)
{

  G4cout << "<<< Geant4 Physics List simulation engine: QGSP_BERT_Bias"<<G4endl;
  G4cout <<G4endl;

  defaultCutValue = 0.7*CLHEP::mm;  
  SetVerboseLevel(ver);

  // EM Physics
  RegisterPhysics( new G4EmStandardPhysics(ver) );

  // Synchroton Radiation & GN Physics
  RegisterPhysics( new G4EmExtraPhysics(ver) );

  // Decays
  RegisterPhysics( new G4DecayPhysics(ver) );

   // Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysicsBias(ver) );

  // Hadron Physics
  RegisterPhysics( new G4HadronPhysicsQGSP_BERT_Bias(ver));

  // Stopping Physics
  RegisterPhysics( new G4StoppingPhysics(ver) );

  // Ion Physics
  RegisterPhysics( new G4IonPhysics(ver));
  
  // Neutron tracking cut
  RegisterPhysics( new G4NeutronTrackingCut(ver));

}
