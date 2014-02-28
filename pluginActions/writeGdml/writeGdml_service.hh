// writeGdmlRunAction - write out the Geant geometry as GDML
//
// Write out the Geometry as a GDML file and then, if desired, put it
// into the run record.

#ifndef WRITE_GDML_SERVICE_HH
#define WRITE_GDML_SERVICE_HH

#include "artg4tk/actionBase/RunActionBase.hh"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Core/EDProducer.h"

#include <string>

namespace artg4tk {
  
  class WriteGdmlService : public RunActionBase {
    public:
    
      WriteGdmlService(fhicl::ParameterSet const&, art::ActivityRegistry&);
      virtual ~WriteGdmlService();
    
      // Prepare Art for the data
      virtual void callArtProduces(art::EDProducer *);
    
      // Write out the data in the Run record
      virtual void fillRunBeginWithArtStuff(art::Run&);
    
    private:
      std::string gdmlFileName_;
  };
}

using artg4tk::WriteGdmlService;
DECLARE_ART_SERVICE(WriteGdmlService,LEGACY)
#endif
