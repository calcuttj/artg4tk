#ifndef RUNSETTINGS_SERVICE_HH
#define RUNSETTINGS_SERVICE_HH

// Art
#include "fhiclcpp/fwd.h"
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"

namespace artg4tk {

  class RunSettingsService {
  public: 
    RunSettingsService(fhicl::ParameterSet const&);
    ~RunSettingsService();
    
    fhicl::ParameterSet getSettings(std::string const &);
  private:
    fhicl::ParameterSet p_;
  };
}

using artg4tk::RunSettingsService;
DECLARE_ART_SERVICE(RunSettingsService,LEGACY)

#endif
