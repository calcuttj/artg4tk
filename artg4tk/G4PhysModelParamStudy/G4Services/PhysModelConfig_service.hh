#ifndef artg4tk_G4PhysModelParamStudy_G4Services_PhysModelConfig_service_hh
#define artg4tk_G4PhysModelParamStudy_G4Services_PhysModelConfig_service_hh

// Art
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
#include "fhiclcpp/ParameterSet.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

// fwd declaration(s)
class ModelConfigMapper;

namespace artg4tk {

  class PhysModelConfigService {

  public:
    // ctor & dtor
    //
    // NOTE: the ctor MUST have these 2 inpur args;
    //       otherwise the system will NOT register it as a "service"
    //
    PhysModelConfigService(const fhicl::ParameterSet&, art::ActivityRegistry&);
    ~PhysModelConfigService();

    void PrintDefaultSettings();
    void ConfigureModel(const std::string&, const fhicl::ParameterSet&);
    void Reset();
    void PrintCurrentSettings();

  private:
    // member functions
    //
    void DoConfigure(const std::string&, const std::string&, const fhicl::ParameterSet&);

    // data members
    //
    ModelConfigMapper* fConfigMapper;
    bool fVerbosity;
  };

}

DECLARE_ART_SERVICE(artg4tk::PhysModelConfigService, LEGACY)

#endif /* artg4tk_G4PhysModelParamStudy_G4Services_PhysModelConfig_service_hh */
