#ifndef DUMMY_SERVICE_HH
#define DUMMY_SERVICE_HH

// Art
#include "fhiclcpp/fwd.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
namespace art { class EDProducer; }

namespace artg4tk {

  class DummyService {
  public: 
    DummyService(fhicl::ParameterSet const&);
    ~DummyService();

    void notifyArtOfProduction(art::EDProducer * prod);

  private:

  };
}
using artg4tk::DummyService;
DECLARE_ART_SERVICE(DummyService,LEGACY)

#endif
