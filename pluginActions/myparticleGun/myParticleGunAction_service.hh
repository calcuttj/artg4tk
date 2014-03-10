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
//      Energy: 10.
//      Position: [ 0,  0,  -130.  ]
//    }
//     . ..
//   }
// }
// Author: Hans Wenzel (Fermilab)
//=============================================================================

// Include guard
#ifndef EXAMPLE_PRIMARYGENERATORACTION_SERVICE_HH
#define EXAMPLE_PRIMARYGENERATORACTION_SERVICE_HH

// art Includes:
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Core/EDProducer.h"
// Get the artg4tk includes:
#include "artg4/actionBase/PrimaryGeneratorActionBase.hh"
// Geant 4 includes:
#include "Geant4/G4Event.hh"
#include "Geant4/G4ParticleGun.hh"


//#include <vector>
//#include <string>
namespace artg4tk {

    class myParticleGunActionService
    : public artg4::PrimaryGeneratorActionBase {
    public:
        myParticleGunActionService(fhicl::ParameterSet const&,
                art::ActivityRegistry&);
        virtual ~myParticleGunActionService();
        virtual void initialize() override;
        // To generate primaries, we need to overload the GeneratePrimaries
        // method. 
        virtual void generatePrimaries(G4Event * anEvent) override;
        const G4ParticleGun* GetGun()    const { return particleGun_; }; 
        // We don't add anything to the event, so we don't need callArtProduces
        // or FillEventWithArtStuff.

    private:
        G4ParticleGun * particleGun_;
        int nparticle;
        std::string particleName_;
        std::vector<double> ParticleMomentumDirection_;
        double ParticleEnergy_;
        std::vector<double> ParticlePosition_;
        // A message logger for this action object
        mf::LogInfo logInfo_;

    };
} // namespace artg4tk
using artg4tk::myParticleGunActionService;
DECLARE_ART_SERVICE(myParticleGunActionService, LEGACY)

#endif
