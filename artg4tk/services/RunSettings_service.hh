#ifndef RUNSETTINGS_SERVICE_HH
#define RUNSETTINGS_SERVICE_HH

// Art
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Core/EDProducer.h"

namespace artg4tk {

  class RunSettingsService {
  public: 
    RunSettingsService(fhicl::ParameterSet const&, art::ActivityRegistry&);
    ~RunSettingsService();
    
    fhicl::ParameterSet getSettings(std::string const &);
  private:
    fhicl::ParameterSet p_;
  };
}

using artg4tk::RunSettingsService;
DECLARE_ART_SERVICE(RunSettingsService,LEGACY)

#endif
