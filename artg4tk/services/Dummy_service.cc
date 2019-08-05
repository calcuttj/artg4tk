// Common materials used throughout the simulation

#include "artg4tk/services/Dummy_service.hh"
#include "art/Framework/Core/EDProducer.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include <iostream>

artg4tk::DummyService::DummyService(fhicl::ParameterSet const &)
{
  std::cout << "DummyService has been constructed!" << std::endl;
}

// Destructor
artg4tk::DummyService::~DummyService()
{
}

void artg4tk::DummyService::notifyArtOfProduction(art::EDProducer * prod)
{
  mf::LogDebug("DummyService") << "running produces<int>()...";
  prod -> produces<int>();
  mf::LogDebug("DummyService") << "done!";
}

using artg4tk::DummyService;

DEFINE_ART_SERVICE(DummyService)
