// Common materials used throughout the simulation

#include "art/Framework/Core/ProducesCollector.h"
#include "artg4tk/services/Dummy_service.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include <iostream>

artg4tk::DummyService::DummyService(fhicl::ParameterSet const&)
{
  std::cout << "DummyService has been constructed!" << std::endl;
}

// Destructor
artg4tk::DummyService::~DummyService() {}

void
artg4tk::DummyService::notifyArtOfProduction(art::ProducesCollector& prod)
{
  mf::LogDebug("DummyService") << "running produces<int>()...";
  prod.produces<int>();
  mf::LogDebug("DummyService") << "done!";
}
