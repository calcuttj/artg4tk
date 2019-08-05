// Declarations for the @DetectorHolderService@ Art service. 

// @DetectorHolderService@ is a globally-accessible service that manages the 
// simulation geometry and detector components. All geometry construction 
// occurs in classes derived from @DetectorBase@, which are registered with
// the service at Art's @beginJob@. Registration of a detector requires that
// the detector's logical volumes be constructed, and allows for the placement
// of these volumes to create the detector physical volume. Furthermore, 
// detector objects can, if they choose, overload @DetectorBase@'s 
// @convertGeantToArtHits@ method, which is called at the end of each event,
// to construct artized hits for the event. 

// Any class can @#include@ and access the @DetectorHolderService@ service to get
// the collection of registered geometry components, retrieve parameter sets
// describing geometry and detector configuration, or get a single detector
// object of a given category.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// Include guard
#ifndef DETECTOR_HOLDER_SERVICE_HH
#define DETECTOR_HOLDER_SERVICE_HH

// Includes
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "fhiclcpp/ParameterSet.h"
namespace art {
  class EDProducer;
  class Event;
}

#include <map>

class G4HCofThisEvent;
class G4VPhysicalVolume;

// Everything for the Art G4 simulation goes in the @artg4tk@ namespace
namespace artg4tk {

  class DetectorBase;

  class DetectorHolderService {
  public:
    
    // Constructor for GeometryHolder
    DetectorHolderService(fhicl::ParameterSet const&);
    
    // This registers the passed detector with the service. 
    void registerDetector(DetectorBase * const db);
    
    // Initialize - run initialize on all detectors after the particle list is set up
    void initialize();

    // This returns the world physical volume, if it has been fully 
    // constructed, and throws an exception otherwise
    // This can't be const because, if the world hasn't already been set up,
    // it constructs all the physical volumes, and thus changes its member
    // data.
    G4VPhysicalVolume * worldPhysicalVolume();

    // This returns the map of detectors we have been passed so far.
    //std::map<std::string, DetectorBase *> const & getDetectorMap() const;
  
    // Returns a pointer to the DetectorBase for the given category, if it
    // exists. If the category was never registered, it throws an 
    // exception.
    DetectorBase * getDetectorForCategory(std::string category) const;

    // Returns the FHiCL parameter set for the given category, if it exists.
    // If the category was never registered, it throws an exception.
    fhicl::ParameterSet const getParametersForCategory(std::string category);

    // Tell Art what the detectors produce
    void callArtProduces(art::EDProducer * prod);
    
    // Convert GEANT4 hits to Art hits and put them in the event.
    void fillEventWithArtHits(G4HCofThisEvent* hc);

    // Set/get the current Art event
    void setCurrArtEvent(art::Event & e) { currentArtEvent_ = &e; }
    art::Event & getCurrArtEvent() { return (*currentArtEvent_); }

    // Construct all the logical volumes.
    void constructAllLVs();

  private:

    // Construct all the physical volumes and assign the world physical volume
    // to worldPV_.
    void constructAllPVs();

    // Add the passed DetectorBase to our category map (a complete list
    // of the detector services we have so far). The key is the DB's category
    // (without repeats), and the value is a pointer to the DB.
    void addDBtoCategoryMap(DetectorBase * const db);

    // Find the given detector's mother logical volume and pass it on to the
    // given detector.
    void placeDetector(DetectorBase * const db);

    // A complete map containing all of the detectors that
    // have registered with us so far. Key: DB's category (a string). 
    // Value: pointer to the DetectorBase object.
    // Repeated keys not permitted!
    std::map<std::string, DetectorBase *> categoryMap_;
    
    // Hold on to the world physical volume so we can return it later.
    G4VPhysicalVolume * worldPV_;

    // Hold on to the current Art event
    art::Event * currentArtEvent_;

  };

} // end namespace artg4tk

using artg4tk::DetectorHolderService;
DECLARE_ART_SERVICE(DetectorHolderService,LEGACY)
#endif // DETECTOR_HOLDER_HH
