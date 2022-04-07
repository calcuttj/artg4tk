#ifndef artg4tk_G4PhysModelParamStudy_G4Services_ProcLevelSimSetup_service_hh
#define artg4tk_G4PhysModelParamStudy_G4Services_ProcLevelSimSetup_service_hh

// Art
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include <string>

class G4Material;
class G4Region;

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
