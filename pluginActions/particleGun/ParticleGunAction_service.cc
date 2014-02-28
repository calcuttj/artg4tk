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
//      ParticleGunAction: {
//      name: "ParticleGun"	
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
// art includes:
#include "art/Framework/Services/Registry/ServiceHandle.h"
// artg4tk includes:
#include "artg4tk/pluginActions/particleGun/ParticleGunAction_service.hh"
#include "artg4tk/services/ActionHolder_service.hh"
// Geant 4  includes:
#include "Geant4/G4Event.hh"
#include "Geant4/G4GeneralParticleSource.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/globals.hh"

using std::string;

artg4tk::ParticleGunActionService::
ParticleGunActionService(fhicl::ParameterSet const & p,
        art::ActivityRegistry &)
: PrimaryGeneratorActionBase(p.get<string>("name", "exampleParticleGun")),
particleGun_(0),
nparticle(p.get<int>("NParticle")),
particleName_(p.get<std::string>("Name")),
ParticleMomentumDirection_(p.get<std::vector<double> >("Direction")),
ParticleEnergy_(p.get<double>("Energy")),
ParticlePosition_(p.get<std::vector<double> >("Position")),
// Initialize our message logger
logInfo_("ExampleParticleGunAction") {

}

// Destructor

artg4tk::ParticleGunActionService::~ParticleGunActionService() {
    delete particleGun_;
}

void artg4tk::ParticleGunActionService::initialize() {

    logInfo_ << "Initializing particle gun. \n";
    G4int n_particle = nparticle;
    G4ParticleGun* fParticleGun = new G4ParticleGun(n_particle);
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = particleName_;
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ParticleMomentumDirection_[0],
            ParticleMomentumDirection_[1],
            ParticleMomentumDirection_[2]));
    fParticleGun->SetParticleEnergy(ParticleEnergy_ * GeV);
    fParticleGun->SetParticlePosition(G4ThreeVector(ParticlePosition_[0] * cm,
            ParticlePosition_[1] * cm,
            ParticlePosition_[2] * cm));
    particleGun_ = fParticleGun;
}

// Create a primary particle for an event!
// (Standard Art G4 simulation)

void artg4tk::ParticleGunActionService::generatePrimaries(G4Event * anEvent) {
    particleGun_->GeneratePrimaryVertex(anEvent);
}

using artg4tk::ParticleGunActionService;
DEFINE_ART_SERVICE(ParticleGunActionService)
