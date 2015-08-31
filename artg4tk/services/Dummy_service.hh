#ifndef DUMMY_SERVICE_HH
#define DUMMY_SERVICE_HH

// Art
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Core/EDProducer.h"

namespace artg4tk {

  class DummyService {
  public: 
    DummyService(fhicl::ParameterSet const&, art::ActivityRegistry&);
    ~DummyService();

    void notifyArtOfProduction(art::EDProducer * prod);

  private:

  };
}
using artg4tk::DummyService;
DECLARE_ART_SERVICE(DummyService,LEGACY)

#endif
