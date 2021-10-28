#ifndef DUMMY_SERVICE_HH
#define DUMMY_SERVICE_HH

// Art
#include "fhiclcpp/fwd.h"
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
namespace art { class ProducesCollector; }

namespace artg4tk {

  class DummyService {
  public: 
    DummyService(fhicl::ParameterSet const&);
    ~DummyService();

    void notifyArtOfProduction(art::ProducesCollector & prod);

  private:

  };
}
using artg4tk::DummyService;
DECLARE_ART_SERVICE(DummyService,LEGACY)

#endif
