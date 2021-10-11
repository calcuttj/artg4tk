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

// artg4tk includes
#include "artg4tk/pluginActions/myparticleGun/HepevtInputAction_service.hh"

// Geant4 includes
#include "Geant4/G4HEPEvtInterface.hh"
#include "Geant4/G4VPrimaryGenerator.hh"

using std::string;

artg4tk::HepevtInputActionService::HepevtInputActionService(fhicl::ParameterSet const& p)
  : PrimaryGeneratorActionBase(p.get<string>("name", "HepevtInput"))
  , HEPEvt_(0)
  , fileName_(p.get<std::string>("FileName"))
  ,
  // Initialize our message logger
  logInfo_("ExampleParticleGunAction")
{}

// Destructor

artg4tk::HepevtInputActionService::~HepevtInputActionService()
{
  delete HEPEvt_;
}

void
artg4tk::HepevtInputActionService::initialize()
{

  logInfo_ << "Initializing Hepevt Input\n";
  const char* filename = (const char*)fileName_.c_str();
  HEPEvt_ = new G4HEPEvtInterface(filename);
}

// Create a primary particle for an event!
// (Standard Art G4 simulation)

void
artg4tk::HepevtInputActionService::generatePrimaries(G4Event* anEvent)
{
  HEPEvt_->GeneratePrimaryVertex(anEvent);
}
