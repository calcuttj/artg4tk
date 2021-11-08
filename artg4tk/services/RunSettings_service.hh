#ifndef artg4tk_services_RunSettings_service_hh
#define artg4tk_services_RunSettings_service_hh

// Art
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
#include "fhiclcpp/fwd.h"

namespace artg4tk {

  class RunSettingsService {
  public:
    RunSettingsService(fhicl::ParameterSet const&);
    ~RunSettingsService();

    fhicl::ParameterSet getSettings(std::string const&);

  private:
    fhicl::ParameterSet p_;
  };
}

DECLARE_ART_SERVICE(artg4tk::RunSettingsService, LEGACY)

#endif /* artg4tk_services_RunSettings_service_hh */
