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

#ifndef artg4tk_pluginDetectors_gdml_GDMLDetector_service_hh
#define artg4tk_pluginDetectors_gdml_GDMLDetector_service_hh

// Includes

namespace art {
  class ProducesCollector;
}

#include "messagefacility/MessageLogger/MessageLogger.h"

namespace fhicl {
  class ParameterSet;
}

#include <string>
#include <vector>

class G4HCosThisEvent;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4GDMLParser;

// Get the base class
#include "artg4tk/Core/DetectorBase.hh"

namespace artg4tk {

  class GDMLDetectorService : public artg4tk::DetectorBase {
  private:
    std::string gdmlFileName_; // name of the gdml file
    bool checkoverlaps_;       // enable/disable check of overlaps
    std::vector<std::string>
      volumeNames_;                 // list of volume names for which step limits should be set
    std::vector<float> stepLimits_; // corresponding step limits to be set for each volume in the
                                    // list of volumeNames, [mm]
    size_t inputVolumes_;           // number of stepLimits to be set
    bool dumpMP_;                   // enable/disable dump of material properties
    // A message logger for this action
    mf::LogInfo logInfo_;
    std::vector<std::pair<std::string, std::string>> DetectorList;
    std::vector<std::pair<std::string, float>>
      selectedVolumes_; // holds all <volume, steplimit> pairs to be set from the configuration file
    std::unordered_map<std::string, float>
      setGDMLVolumes_; // holds all <volume, steplimit> pairs set from the GDML file
  public:
    GDMLDetectorService(fhicl::ParameterSet const&);
    virtual ~GDMLDetectorService();

  private:
    // Private overriden methods
    std::vector<G4LogicalVolume*> doBuildLVs() override;
    std::vector<G4VPhysicalVolume*> doPlaceToPVs(std::vector<G4LogicalVolume*>) override;

    // -- D.R. Set the step limits for specific volumes from the configuration file
    virtual void setStepLimits(G4GDMLParser* parser);

    // We need to add something to the art event, so we need these two methods:

    // Tell Art what we'll produce
    void doCallArtProduces(art::ProducesCollector&) override;

    // Actually produce
    void doFillEventWithArtHits(G4HCofThisEvent* hc) override;
  };
}

DECLARE_ART_SERVICE(artg4tk::GDMLDetectorService, LEGACY)

#endif /* artg4tk_pluginDetectors_gdml_GDMLDetector_service_hh */
