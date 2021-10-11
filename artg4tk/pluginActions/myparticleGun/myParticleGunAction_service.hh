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

#ifndef artg4tk_pluginActions_myparticleGun_myParticleGunAction_service_hh
#define artg4tk_pluginActions_myparticleGun_myParticleGunAction_service_hh

// art Includes:
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
#include "fhiclcpp/fwd.h"

// artg4tk includes
#include "artg4tk/actionBase/PrimaryGeneratorActionBase.hh"

// Geant 4 includes
#include "Geant4/G4ParticleGun.hh"
class G4Event;

#include <string>

namespace artg4tk {

  class myParticleGunActionService : public artg4tk::PrimaryGeneratorActionBase {
  public:
    myParticleGunActionService(fhicl::ParameterSet const&);

    void initialize() override;

    // To generate primaries, we need to overload the GeneratePrimaries
    // method.
    void generatePrimaries(G4Event* anEvent) override;

    G4ParticleGun const*
    GetGun() const
    {
      return particleGun_.get();
    };

  private:
    std::unique_ptr<G4ParticleGun> particleGun_{nullptr};
    G4int nparticle_;
    std::string particleName_;
    std::vector<double> momentumDirection_;
    double energy_;
    std::vector<double> position_;
  };
} // namespace artg4tk

DECLARE_ART_SERVICE(artg4tk::myParticleGunActionService, LEGACY)

#endif /* artg4tk_pluginActions_myparticleGun_myParticleGunAction_service_hh */
