// Common materials used throughout the simulation

#include "artg4tk/services/RunSettings_service.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"

artg4tk::RunSettingsService::RunSettingsService(fhicl::ParameterSet const& p) : p_(p)
{
  mf::LogDebug("RunSettingService") << "RunSettingsService has been constructed!";
}

// Destructor
artg4tk::RunSettingsService::~RunSettingsService() {}

fhicl::ParameterSet
artg4tk::RunSettingsService::getSettings(std::string const& s)
{

  fhicl::ParameterSet p = p_.get<fhicl::ParameterSet>(s, fhicl::ParameterSet());
  return p;
}
