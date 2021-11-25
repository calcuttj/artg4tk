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
// This file provides the implementation for an action object that produces
// the primary particles using a pythia generated Hepevt event as input
// ParticleGunActionService.hh: art wrapper for G4ParticleGun
// To use this, all you need to do is put it in the services section
// of the configuration file, like this:
//
// services: {
//   ...
//   user: {
//      HepevtInputAction: {
//      name: "HepevtInput"
//      FileName: "pythia_event.data"
//    }
//     . ..
//   }
// }
// Authors: Tasha Arvanitis, Adam Lyon
// Date: August 2012
//=============================================================================

#ifndef artg4tk_pluginActions_myparticleGun_HepevtInputAction_service_hh
#define artg4tk_pluginActions_myparticleGun_HepevtInputAction_service_hh

// framework Includes:
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
#include "fhiclcpp/fwd.h"

// Get the artg4tk base class
#include "artg4tk/actionBase/PrimaryGeneratorActionBase.hh"

// Geant 4 includes:
#include "Geant4/G4VPrimaryGenerator.hh"
class G4Event;

namespace artg4tk {

  class HepevtInputActionService : public artg4tk::PrimaryGeneratorActionBase {
  public:
    HepevtInputActionService(fhicl::ParameterSet const&);
    void initialize() override;

    // To generate primaries, we need to overload the GeneratePrimaries
    // method.
    void generatePrimaries(G4Event* anEvent) override;

  private:
    std::unique_ptr<G4VPrimaryGenerator> HEPEvt_{nullptr};
    std::string fileName_; // name of hepevt input file
  };
} // namespace artg4tk

DECLARE_ART_SERVICE(artg4tk::HepevtInputActionService, LEGACY)

#endif /* artg4tk_pluginActions_myparticleGun_HepevtInputAction_service_hh */
