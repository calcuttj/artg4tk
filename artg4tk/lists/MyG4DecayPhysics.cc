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
// ClassName:   G4DecayPhysics
//
// Author: 2002 J.P. Wellisch
//
// Modified:
// 10.11.2005 V.Ivanchenko edit to provide a standard
// 05.12.2005 V.Ivanchenko add controlled verbosity
// 25.04.2006 V.Ivanchenko fix problem of destructor 
// 07.03.2022 D.Rivera: imported into artg4tk based on :
//            geant4.10.06.p01/source/physics_lists/constructors/decay/src/G4DecayPhysics.cc
//            Don't register Decay process for mu+, dump some information
//
//----------------------------------------------------------------------------
//

#include "MyG4DecayPhysics.hh"

#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4ProcessManager.hh"

#include "Geant4/G4BosonConstructor.hh"
#include "Geant4/G4LeptonConstructor.hh"
#include "Geant4/G4MesonConstructor.hh"
#include "Geant4/G4BosonConstructor.hh"
#include "Geant4/G4BaryonConstructor.hh"
#include "Geant4/G4IonConstructor.hh"
#include "Geant4/G4ShortLivedConstructor.hh"

// factoryGeant4/
#include "Geant4/G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(MyG4DecayPhysics);

G4ThreadLocal G4Decay* MyG4DecayPhysics::fDecayProcess = 0;
G4ThreadLocal G4bool MyG4DecayPhysics::wasActivated = false;

MyG4DecayPhysics::MyG4DecayPhysics(G4int ver)
  :  G4VPhysicsConstructor("Decay"), verbose(ver)
{
}

MyG4DecayPhysics::MyG4DecayPhysics(const G4String& name, G4int ver)
  :  G4VPhysicsConstructor(name), verbose(ver)
{
}

MyG4DecayPhysics::~MyG4DecayPhysics()
{
}

void MyG4DecayPhysics::ConstructParticle()
{

// G4cout << "MyG4DecayPhysics::ConstructParticle" << G4endl;
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

void MyG4DecayPhysics::ConstructProcess()
{
  if(wasActivated) { return; }
  wasActivated = true;

  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  // Add Decay Process
  fDecayProcess = new G4Decay();
  auto myParticleIterator=GetParticleIterator();
  myParticleIterator->reset();
  G4ParticleDefinition* particle=0;

  while( (*myParticleIterator)() )
  {
    particle = myParticleIterator->value();

    // -- check for mu+
    bool isMuPlus = particle->GetParticleName() == "mu+";
    if( fDecayProcess->IsApplicable(*particle) && !isMuPlus ) 
    { 
      if(verbose > 1) {
        G4cout << "### Decays for " << particle->GetParticleName() << G4endl;
      }
      ph->RegisterProcess(fDecayProcess, particle);
    }
    if (isMuPlus) {
      G4cout << "MyG4DecayPhysics::ConstructProcess() mu+ decays will be disabled\n" << G4endl;
      particle->DumpTable();
      particle->GetProcessManager()->DumpInfo();
    }
  }
}
