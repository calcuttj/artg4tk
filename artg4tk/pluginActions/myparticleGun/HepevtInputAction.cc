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

// Framework includes
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// artg4tk includes
#include "artg4tk/pluginActions/myparticleGun/HepevtInputAction_service.hh"

// Geant4 includes
#include "Geant4/G4HEPEvtInterface.hh"

artg4tk::HepevtInputActionService::HepevtInputActionService(fhicl::ParameterSet const& p)
  : PrimaryGeneratorActionBase(p.get<std::string>("name", "HepevtInput"))
  , fileName_(p.get<std::string>("FileName"))
{}

void
artg4tk::HepevtInputActionService::initialize()
{
  mf::LogInfo("HepevtInputAction") << "Initializing Hepevt Input";
  HEPEvt_ = std::make_unique<G4HEPEvtInterface>(fileName_.c_str());
}

// Create a primary particle for an event!
// (Standard Art G4 simulation)

void
artg4tk::HepevtInputActionService::generatePrimaries(G4Event* anEvent)
{
  HEPEvt_->GeneratePrimaryVertex(anEvent);
}
