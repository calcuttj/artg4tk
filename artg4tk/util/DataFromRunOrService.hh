// DataFromRunOrService
//
// A utility templated function to get data either from the Run record or from
// a service. The run record is tried first. If that fails and the service does not
// exist, then an exception is thrown. We assume that the service has a const method called
// getData() that returns a const refernce to the data type.
//
//

#ifndef artg4tk_util_DataFromRunOrService_hh
#define artg4tk_util_DataFromRunOrService_hh

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include <string>
#include <vector>

namespace artg4tk {

  template <typename DATATYPE, typename SERVICETYPE>
  const DATATYPE&
  dataFromRunOrService(const art::Run& r,
                       const std::string& producerLabel,
                       const std::string& instanceLabel)
  {

    // Let's try to get the data out of the Run first
    // std::vector< art::Handle< DATATYPE > > handleVec;
    // r.getManyByType(handleVec);
    auto handleVec = r.getMany<DATATYPE>();

    if (!handleVec.empty()) {
      // We found something in the run record
      for (const art::Handle<DATATYPE>& aHandle : handleVec) {

        // Get the provenance for this handle
        art::Provenance const* prov = aHandle.provenance();

        // Check the producer label
        if (producerLabel.empty() || producerLabel == prov->moduleLabel()) {

          // Check the instance label
          if (instanceLabel.empty() || instanceLabel == prov->productInstanceName()) {

            // We found it!
            mf::LogDebug("DataFromRunOrService") << "Getting data from Run";
            return *aHandle;
          }
        }
      }
    }

    // If we make it here, then we need to look at the service (throws an exception if the
    // service does not exist)
    mf::LogDebug("DataFromRunOrService") << "Getting data from Service";
    return art::ServiceHandle<SERVICETYPE>()->getData();
  }

}

#endif /* artg4tk_util_DataFromRunOrService_hh */
