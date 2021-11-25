// This file is the header for the @DetectorBase@ class.

// h2. Introduction

// @DetectorBase@ enforces an interface for the construction and handling of Geant4 detectors.

// All _ArtG4_ detector objects MUST inherit from this @DetectorBase@ class.

// For your detector to work, you must override the following methods:

// * @doBuildLVs@ - This private method, which takes no arguments, constructs the detector logical
// volume and returns a pointer to it. The system takes over ownership of this pointer.

// * @doPlaceToPVs@ - This private method takes the mother logical volumes as an argument and
// performs the functions necessary to place this detector within it. This method then returns a
// pointer to the resulting physical volume. The system takes over ownership of this pointer.

// There are also some optional methods you can override.:

// * @doCallArtProduces@ - This private method is optional, but is necessary if your detector
// produces hits. Its argument is a producer, for which you should call produces<T>("name"), where T
// is the type you will add to the event (typically a hit collection) and name is an identifier to
// distinguish your hits from those of any other detector.

// * @doFillEventWithArtHits@ - This private method is optional, but is necessary if your detector
// produces hits that will end up in the Art event. The argument is the GEANT hit collection for the
// event in question. To add a collection of Art hits to the Art event, you must convert the GEANT
// hits into Art hits, and then put your collection into the Art event.

// See below for information about each method. Note that many of them you never
// call yourself.

// h2. Code

// Header Guard

#ifndef artg4tk_Core_DetectorBase_hh
#define artg4tk_Core_DetectorBase_hh

// STL includes
#include <vector>

// Art includes
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/ParameterSet.h"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4tk/services/DetectorHolder_service.hh"

// Forward referencing
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4HCofThisEvent;

namespace art {
  class ProducesCollector;
}

// h3. Declare the @DetectorBase@ class

// All offical Art G4 specific code goes into the @artg4tk@ namespace
namespace artg4tk {

  class DetectorBase {

    // h3. Public members

  public:
    // Constructor. Your derived class must call this
    // constructor. This base class constructor takes,
    // * The parameter set for the corresponding Art producer module
    // * The name of this detector
    // * The category of this detector ("world" if this is the world)
    // * The category of the mother volume that this detector is placed within ("" if world)
    DetectorBase(fhicl::ParameterSet const& p,
                 std::string myName,
                 std::string category,
                 std::string motherCategory)
      : _myParams(fhicl::ParameterSet(p))
      , _myName(myName)
      , _category(category)
      , _motherCategory(motherCategory)
      , _myLVs(0)
      , _myPVs(0)
    {
      // Register ourselves
      art::ServiceHandle<DetectorHolderService> detectorHolder;
      detectorHolder->registerDetector(this);
    }

    // Destructor
    virtual ~DetectorBase() = default;

    // Intialize after the particle list is set up
    virtual void initialize(){};

    // Build and store the logical volume (calls your @doBuild@ method). You
    // do not need to call this method yourself.
    void
    buildLVs()
    {
      _myLVs = doBuildLVs();
    }

    // Place the detector in the mother volume and store and return the
    // physical volume (calls your @doPlaceToPVs@ method). You do not need to
    // call this method yourself.
    std::vector<G4VPhysicalVolume*>
    placeToPVs(std::vector<G4LogicalVolume*> motherLVs)
    {
      _myPVs = doPlaceToPVs(motherLVs);
      return _myPVs;
    }

    // Tell Art what this detector will put into the event. You do not need to
    // call this yourself.
    void
    callArtProduces(art::ProducesCollector& produces_coll)
    {
      doCallArtProduces(produces_coll);
    }

    // Put anything you need to into the event. You do not need to call this
    // method yourself.
    void
    fillEventWithArtHits(G4HCofThisEvent* hc)
    {
      doFillEventWithArtHits(hc);
    }

    // h3. Accessors

    // Return this detector's Geant Physical Volume
    std::vector<G4VPhysicalVolume*>
    pvs() const
    {
      return _myPVs;
    }

    // Return this detector's Geant Logical Volume
    std::vector<G4LogicalVolume*>
    lvs() const
    {
      return _myLVs;
    }

    // Return the name of this detector
    std::string const&
    myName() const
    {
      return _myName;
    }

    // Return the category of this detector
    std::string const&
    category() const
    {
      return _category;
    }

    // Returnh the mother category for this detector
    std::string const&
    motherCategory() const
    {
      return _motherCategory;
    }

    // Return the parameter set for this detector
    fhicl::ParameterSet const&
    parameters() const
    {
      return _myParams;
    }

  private:
    // h3. Private abstract methods you must override (see list above)

    // Build the detector logical volume and return it
    virtual std::vector<G4LogicalVolume*> doBuildLVs() = 0;

    // Place the detector within the passed in mother logical volume and return
    // the resulting physical volume
    virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs(
      std::vector<G4LogicalVolume*> motherLV) = 0;

    // h3. Optional private methods you can override (see list above)

    // Tell Art what you will put into the event.
    virtual void
    doCallArtProduces(art::ProducesCollector& produces_coll)
    {}

    // Convert G4 hits into Art hits. Put them in the event (which you can get
    // from the DetectorHolder service).
    virtual void
    doFillEventWithArtHits(G4HCofThisEvent*)
    {}

    // h3. Private data

    // The parameters from the corresponding producer module
    fhicl::ParameterSet _myParams;

    // The detector name
    std::string _myName;

    // The detector category
    std::string _category;

    // The detector mother category
    std::string _motherCategory;

    // The detector's Geant logical volumes
    std::vector<G4LogicalVolume*> _myLVs;

    // The detectors's Geant physical volumes
    std::vector<G4VPhysicalVolume*> _myPVs;
  };
}

#endif /* artg4tk_Core_DetectorBase_hh */
