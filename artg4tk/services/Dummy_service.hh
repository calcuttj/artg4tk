#ifndef artg4tk_services_Dummy_service_hh
#define artg4tk_services_Dummy_service_hh

// Art
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
#include "art/Framework/Core/Frameworkfwd.h"
#include "fhiclcpp/fwd.h"

namespace artg4tk {

  class DummyService {
  public:
    DummyService(fhicl::ParameterSet const&);
    ~DummyService();

    void notifyArtOfProduction(art::ProducesCollector& prod);

  };
}

DECLARE_ART_SERVICE(artg4tk::DummyService, LEGACY)

#endif /* artg4tk_services_Dummy_service_hh */
