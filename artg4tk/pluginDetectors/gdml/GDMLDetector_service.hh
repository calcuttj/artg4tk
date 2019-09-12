//
//               __        __ __  __  __  
//   ____ ______/ /_____ _/ // / / /_/ /__
//  / __ `/ ___/ __/ __ `/ // /_/ __/ //_/
// / /_/ / /  / /_/ /_/ /__  __/ /_/ ,<   
// \__,_/_/   \__/\__, /  /_/  \__/_/|_|  
//               /____/                  
//
// artg4tk: art based Geant 4 Toolkit
// 
//=============================================================================
// GDMLDetector_service.hh: 
// GDMLDetectorService is the service that constructs the Geant 4 Geometry 
// as specified in a gdml file.
// To use this service, all you need to do is put it in the services section
// of the fcl configuration file, like this:
// 
// <pre>
// services: { 
//   ...
//   user: {
//     ...  
// GDMLDetector : 
//    {
//    category: "world"
//    gdmlFileName_ : "ta_target.gdml"
//    }  
//   }
// }
// </pre>
// Author: Hans Wenzel (Fermilab)
//=============================================================================


// Include guard
#ifndef GDMLDETECTOR_SERVICE_HH
#define GDMLDETECTOR_SERVICE_HH

// Includes
#include "fhiclcpp/fwd.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
namespace art { class EDProducer; }

#include <vector>
#include <string>

class G4HCosThisEvent;
class G4LogicalVolume;
class G4VPhysicalVolume;

// Get the base class
#include "artg4tk/Core/DetectorBase.hh"


namespace artg4tk {

    class GDMLDetectorService : public artg4tk::DetectorBase {
    private:
      std::string gdmlFileName_;  // name of the gdml file 
      bool checkoverlaps_;        // enable/disable check of overlaps
      bool dumpMP_;               // enable/disable dump of material properties
      // A message logger for this action
      mf::LogInfo logInfo_;
      std::vector<std::pair<std::string,std::string> > DetectorList;
    public:
        GDMLDetectorService(fhicl::ParameterSet const&);
        virtual ~GDMLDetectorService();

    private:

        // Private overriden methods
        virtual std::vector<G4LogicalVolume*> doBuildLVs() override;
        virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs(std::vector<G4LogicalVolume*>) override;
        // We need to add something to the art event, so we need these two methods:

        // Tell Art what we'll produce
        virtual void doCallArtProduces(art::EDProducer * producer) override;

        // Actually produce
        virtual void doFillEventWithArtHits(G4HCofThisEvent * hc) override;

 

    };
}
using artg4tk::GDMLDetectorService;
DECLARE_ART_SERVICE(GDMLDetectorService,LEGACY)
#endif
