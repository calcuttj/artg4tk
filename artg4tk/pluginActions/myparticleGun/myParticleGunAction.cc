//
//               __        __ __  __  __
//   ____ ______/ /_____ _/ // / / /_/ /__
//  / __ `/ ___/ __/ __ `/ // /_/ __/ //_/
// / /_/ / /  / /_/ /_/ /__  __/ /_/ ,<
// \__,_/_/   \__/\__, /  /_/  \__/_/|_|
//               /____/
//
// artg4tk: art based Geant 4 Toolkit
//
//=============================================================================
// ParticleGunActionService.hh: art wrapper for G4ParticleGun
// To use this, all you need to do is put it in the services section
// of the configuration file, like this:
//
// services: {
//   ...
//   user: {
//      myParticleGunAction: {
//      name: "myParticleGun"
//      NParticle: 1
//      Name: "proton"
//      Direction: [ 0,  0,  1  ]
//      Energy: 10.                       // [GeV]]
//      Position: [ 0,  0,  -130.  ]      // [cm]
//    }
//     . ..
//   }
// }
// Author: Hans Wenzel (Fermilab)
//=============================================================================

// art includes
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// artg4tk includes
#include "artg4tk/pluginActions/myparticleGun/myParticleGunAction_service.hh"

// Geant4 includes
#include "Geant4/G4ParticleTable.hh"

artg4tk::myParticleGunActionService::myParticleGunActionService(fhicl::ParameterSet const& p)
  : PrimaryGeneratorActionBase(p.get<std::string>("name", "myParticleGunAction"))
  , nparticle_(p.get<G4int>("NParticle"))
  , particleName_(p.get<std::string>("Name"))
  , momentumDirection_(p.get<std::vector<double>>("Direction"))
  , energy_(p.get<double>("Energy"))
  , position_(p.get<std::vector<double>>("Position"))
{}

void
artg4tk::myParticleGunActionService::initialize()
{
  mf::LogInfo("myParticleGunAction") << "Initializing particle gun.";
  auto particleGun = std::make_unique<G4ParticleGun>(nparticle_);
  particleGun->SetParticleDefinition(
    G4ParticleTable::GetParticleTable()->FindParticle(particleName_));
  particleGun->SetParticleMomentumDirection(
    G4ThreeVector(momentumDirection_[0], momentumDirection_[1], momentumDirection_[2]));
  particleGun->SetParticleEnergy(energy_ * CLHEP::GeV);
  particleGun->SetParticlePosition(
    G4ThreeVector(position_[0] * CLHEP::cm, position_[1] * CLHEP::cm, position_[2] * CLHEP::cm));
  particleGun_ = move(particleGun);
}

// Create a primary particle for an event!
// (Standard Art G4 simulation)

void
artg4tk::myParticleGunActionService::generatePrimaries(G4Event* anEvent)
{
  particleGun_->GeneratePrimaryVertex(anEvent);
}
