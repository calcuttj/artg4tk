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

// Include guard
#ifndef HEPEVT_INPUTACTIONSERVICE_HH
#define HEPEVT_INPUTACTIONSERVICE_HH

// framework Includes:
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
#include "fhiclcpp/fwd.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// Get the artg4tk base class
#include "artg4tk/actionBase/PrimaryGeneratorActionBase.hh"

// Geant 4 includes:
class G4Event;
class G4VPrimaryGenerator;

namespace artg4tk {

  class HepevtInputActionService : public artg4tk::PrimaryGeneratorActionBase {
  public:
    HepevtInputActionService(fhicl::ParameterSet const&);
    virtual ~HepevtInputActionService();
    virtual void initialize() override;
    // To generate primaries, we need to overload the GeneratePrimaries
    // method.
    virtual void generatePrimaries(G4Event* anEvent) override;

    // We don't add anything to the event, so we don't need callArtProduces
    // or FillEventWithArtStuff.

  private:
    G4VPrimaryGenerator* HEPEvt_;
    std::string fileName_; // name of hepevt input file
    mf::LogInfo logInfo_;
  };
} // namespace artg4tk
using artg4tk::HepevtInputActionService;
DECLARE_ART_SERVICE(HepevtInputActionService, LEGACY)

#endif
