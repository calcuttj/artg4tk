#ifndef RUNSETTINGS_BASE_HH
#define RUNSETTINGS_BASE_HH

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4tk/services/RunSettings_service.hh"


#include "fhiclcpp/ParameterSet.h"

#include <string>
namespace artg4tk {

  struct RunSettingsBase { 
    
    RunSettingsBase(std::string const & listName) : 
      name(listName),
      p() 
    {
      art::ServiceHandle <artg4tk::RunSettingsService> s;
      p = s->getSettings( listName );
    }
    
    virtual ~RunSettingsBase() {}
    
    virtual bool contains(std::string) {return false;}
      
    std::string name;
    fhicl::ParameterSet p;
    
  };
}


#endif // RUNSETTINGS_BASE_HH

