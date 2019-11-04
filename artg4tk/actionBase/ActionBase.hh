// This file is the header for the @ActionBase@ class.

// It is the lowest level base class for Geant user actions. Do not
// inherit from this class directly. Instead, use a next level up class, like
// @TrackingActionBase@.

// Include guard
#ifndef ACTION_BASE_HH
#define ACTION_BASE_HH

#include <string>

namespace art {
  class ProducesCollector;
  class Event;
  class Run;
}

// Declarations of types we use as input parameters

// Everything goes in the Art G4 namespace
namespace artg4tk {

  class ActionBase {
  public:
    // Constructor. The derived class must call this constructor. It takes a 
    // single string for the name of the action object.
    ActionBase(std::string myName)
      : myName_(move(myName))
    {}

    // Destructor
    virtual ~ActionBase();

    // Accessor
    std::string const& myName() const {return myName_;}

    // h3. Optional methods
    
    // Intialize - Instead of putting initialization code into your constructor, put
    // such code in this initialize method. This method will get called at the correct
    // time, after particle lists are already constructed and known to geant.
    virtual void initialize() {}

    // Call produces<T> to notify Art what you'll be adding to the Art event.
    virtual void callArtProduces(art::ProducesCollector&){}

    // Fill the Art event with whatever you said you would, if you haven't
    // already. This is called by ActionHolder at the very end of the Art
    // event, after all the GEANT stuff is finished.
    virtual void fillEventWithArtStuff(art::Event &){}
    
    // Fill the Art event with Run information at the beginning of the run
    virtual void fillRunBeginWithArtStuff(art::Run &){}

    // Fill the Art event with Run information at the end of the run
    virtual void fillRunEndWithArtStuff(art::Run &){}


  private:
    // A string containing this action object's name
    std::string myName_;
  };
}


#endif // ACTION_BASE_HH
