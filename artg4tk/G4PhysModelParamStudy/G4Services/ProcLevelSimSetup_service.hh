#ifndef artg4tk_G4PhysModelParamStudy_G4Services_ProcLevelSimSetup_service_hh
#define artg4tk_G4PhysModelParamStudy_G4Services_ProcLevelSimSetup_service_hh

// Art
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include <iostream>
#include <string>

// Geant4
#include "Geant4/G4ElementVector.hh"
#include "Geant4/G4ForceCondition.hh"
#include "Geant4/G4IsotopeVector.hh"
#include "Geant4/G4Material.hh"
#include "Geant4/G4NistManager.hh"
//
#include "Geant4/G4Box.hh"
#include "Geant4/G4GRSVolume.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4Region.hh"
//
#include "Geant4/G4StateManager.hh"

namespace artg4tk {

  class ProcLevelSimSetupService {

  public:
    // ctor & dtor
    //
    // NOTE: the ctor MUST have these 2 inpur args;
    //       otherwise the system will NOT register it as a "service"
    //
    ProcLevelSimSetupService(const fhicl::ParameterSet&, art::ActivityRegistry&);
    ~ProcLevelSimSetupService();

    G4Material*
    GetTgtMaterial() const
    {
      return fTgtMaterial;
    }
    const std::string&
    GetModelName() const
    {
      return fModelName;
    }

  private:
    void Init();

    // data members
    //
    std::string fModelName;
    //
    G4Region* fRegion;
    std::string fTgtMaterialName; // in fact, it's target nucleus...
    G4Material* fTgtMaterial;     // in fact, it's target nucleus...
    //
    long fRandomSeed;

    mf::LogInfo fLogInfo;
  };

}

DECLARE_ART_SERVICE(artg4tk::ProcLevelSimSetupService, LEGACY)

#endif /* artg4tk_G4PhysModelParamStudy_G4Services_ProcLevelSimSetup_service_hh */
