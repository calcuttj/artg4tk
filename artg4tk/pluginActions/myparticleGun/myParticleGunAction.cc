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

// artg4tk includes
#include "artg4tk/pluginActions/myparticleGun/myParticleGunAction_service.hh"

// Geant4 includes
#include "Geant4/G4ParticleGun.hh"
#include "Geant4/G4ParticleTable.hh"

using std::string;

artg4tk::myParticleGunActionService::myParticleGunActionService(fhicl::ParameterSet const& p)
  : PrimaryGeneratorActionBase(p.get<string>("name", "examplemyParticleGun"))
  , particleGun_(0)
  , nparticle(p.get<int>("NParticle"))
  , particleName_(p.get<std::string>("Name"))
  , ParticleMomentumDirection_(p.get<std::vector<double>>("Direction"))
  , ParticleEnergy_(p.get<double>("Energy"))
  , ParticlePosition_(p.get<std::vector<double>>("Position"))
  ,
  // Initialize our message logger
  logInfo_("ExamplemyParticleGunAction")
{}

// Destructor

artg4tk::myParticleGunActionService::~myParticleGunActionService()
{
  delete particleGun_;
}

void
artg4tk::myParticleGunActionService::initialize()
{

  logInfo_ << "Initializing particle gun. \n";
  G4int n_particle = nparticle;
  G4ParticleGun* fParticleGun = new G4ParticleGun(n_particle);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName = particleName_;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(
    ParticleMomentumDirection_[0], ParticleMomentumDirection_[1], ParticleMomentumDirection_[2]));
  fParticleGun->SetParticleEnergy(ParticleEnergy_ * CLHEP::GeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(ParticlePosition_[0] * CLHEP::cm,
                                                  ParticlePosition_[1] * CLHEP::cm,
                                                  ParticlePosition_[2] * CLHEP::cm));
  particleGun_ = fParticleGun;
}

// Create a primary particle for an event!
// (Standard Art G4 simulation)

void
artg4tk::myParticleGunActionService::generatePrimaries(G4Event* anEvent)
{
  particleGun_->GeneratePrimaryVertex(anEvent);
}
