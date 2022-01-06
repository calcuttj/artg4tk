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
// ClassName:   MyQGSP_BERT_LASER
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
// 05.01.2022 D.Rivera: imported into artg4tk based on :
//            geant4.10.06.p01/source/physics_lists/lists/src/QGSP_BERT.cc
//            Converted this to an extensible physics list and registered it.
//            Included custom version of the EmStandardPhysics to attempt
//            to simulate the PDSP ionization laser with mu+.
//
//----------------------------------------------------------------------------
//

#include <iomanip>   
#include <CLHEP/Units/SystemOfUnits.h>

// -- artg4tk includes
#include "artg4tk/lists/G4EmStandardPhysics_muplusLaser.hh"

#include "Geant4/globals.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4ProcessManager.hh"
#include "Geant4/G4ProcessVector.hh"
#include "Geant4/G4ParticleTypes.hh"
#include "Geant4/G4ParticleTable.hh"

#include "Geant4/G4Material.hh"
#include "Geant4/G4MaterialTable.hh"

#include "Geant4/G4DecayPhysics.hh"
#include "Geant4/G4EmExtraPhysics.hh"
#include "Geant4/G4IonPhysics.hh"
#include "Geant4/G4StoppingPhysics.hh"
#include "Geant4/G4HadronElasticPhysics.hh"
#include "Geant4/G4NeutronTrackingCut.hh"

#include "Geant4/G4HadronPhysicsQGSP_BERT.hh"

/////////////////////////////////////////////////////////////////////////////
// The following change is the _only_ required changed to move from
// the non-extensible factory to the exensible factory.  All other changes
// relative to the "factory" example are there to demonstrate new features.
/////////////////////////////////////////////////////////////////////////////
// non-extensible:  #include "G4PhysListFactory.hh"
#include "Geant4/G4PhysListFactoryAlt.hh"
/////////////////////////////////////////////////////////////////////////////
// headers needed to demonstrate new features
/////////////////////////////////////////////////////////////////////////////

// allow ourselves to extend the short names for physics ctor addition/replace
// along the same lines as EMX, EMY, etc
#include "Geant4/G4PhysListRegistry.hh"

// allow ourselves to give the user extra info about available physics ctors
#include "Geant4/G4PhysicsConstructorFactory.hh"

/////////////////////////////////////////////////////////////////////////////
// pull in a user defined physics list definition into the main program
// and register it with the factory (doesn't have to be the main program
// but the .o containing the declaration _must_ get linked/loaded)

#include "Geant4/G4VModularPhysicsList.hh"

#include "Geant4/G4PhysListStamper.hh" // defines macro for factory registration
#include "MyQGSP_BERT_LASER.hh"

// -- Register the physics list
G4_DECLARE_PHYSLIST_FACTORY(MyQGSP_BERT_LASER);

MyQGSP_BERT_LASER::MyQGSP_BERT_LASER(G4int ver)
{

  G4cout << "<<< Geant4 Physics List simulation engine: MyQGSP_BERT_LASER"<<G4endl;
  G4cout <<G4endl;

  defaultCutValue = 0.7*CLHEP::mm;  
  SetVerboseLevel(ver);

  // EM Physics
  //RegisterPhysics( new G4EmStandardPhysics(ver) );
  RegisterPhysics( new G4EmStandardPhysics_muplusLaser(ver) ); // -- replace with laser version

  // Synchroton Radiation & GN Physics
  RegisterPhysics( new G4EmExtraPhysics(ver) );

  // Decays
  RegisterPhysics( new G4DecayPhysics(ver) );

   // Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysics(ver) );

  // Hadron Physics
  RegisterPhysics( new G4HadronPhysicsQGSP_BERT(ver));

  // Stopping Physics
  RegisterPhysics( new G4StoppingPhysics(ver) );

  // Ion Physics
  RegisterPhysics( new G4IonPhysics(ver));
  
  // Neutron tracking cut
  RegisterPhysics( new G4NeutronTrackingCut(ver));

}
