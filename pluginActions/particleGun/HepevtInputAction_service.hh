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
#ifndef EXAMPLE_PRIMARYGENERATORACTION_SERVICE_HH
#define EXAMPLE_PRIMARYGENERATORACTION_SERVICE_HH
// framework Includes:
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Core/EDProducer.h"
// Get the artg4tk base class
#include "artg4/actionBase/PrimaryGeneratorActionBase.hh"
// Geant 4 includes:
#include "Geant4/G4Event.hh"
#include "Geant4/G4VPrimaryGenerator.hh"

namespace artg4tk {

    class HepevtInputActionService
    : public artg4::PrimaryGeneratorActionBase {
    public:
        HepevtInputActionService(fhicl::ParameterSet const&,
                art::ActivityRegistry&);
        virtual ~HepevtInputActionService();
        virtual void initialize() override;
        // To generate primaries, we need to overload the GeneratePrimaries
        // method. 
        virtual void generatePrimaries(G4Event * anEvent) override;

        // We don't add anything to the event, so we don't need callArtProduces
        // or FillEventWithArtStuff.

    private:
        G4VPrimaryGenerator * HEPEvt_;
        std::string fileName_; // name of hepevt input file
        mf::LogInfo logInfo_;

    };
} // namespace artg4tk
using artg4tk::HepevtInputActionService;
DECLARE_ART_SERVICE(HepevtInputActionService, LEGACY)

#endif
