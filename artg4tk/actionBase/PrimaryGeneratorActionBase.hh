// This file is the header for the @PrimaryGeneratorActionBase@ class.

// A base class for Geant user primary generator action

// @GeneratePrimaries@ - This method, which takes a pointer to a @G4Event@
// object, is called to create the primary particle(s) for an event. It is
// called once at the beginning of each event. Putting code in this method of
// and action object is equivalent to putting it in the @GeneratePrimaries@
// method of the simulation's primary generator action class.

// Include guard
#ifndef PRIMARY_GENERATOR_ACTION_BASE_HH
#define PRIMARY_GENERATOR_ACTION_BASE_HH

#include <string>

#include "artg4tk/actionBase/ActionBase.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4tk/services/ActionHolder_service.hh"

// Declarations of types we use as input parameters
class G4Event;

// Everything goes in the Art G4 namespace
namespace artg4tk {

  class PrimaryGeneratorActionBase : public ActionBase {
  public:
    // Constructor. The derived class must call this constructor. It takes a
    // single string for the name of the action object.
    PrimaryGeneratorActionBase(std::string myName) : ActionBase(myName)
    {
      art::ServiceHandle<artg4tk::ActionHolderService> actionHolder;
      actionHolder->registerAction(this);
    }

    // Destructor
    virtual ~PrimaryGeneratorActionBase();

    // h3. The interesting methods.
    // All of these are defined to do nothing by default. Users can override
    // them if desired, and if they're not overloaded, they do nothing.

    // Called for the generation of primaries
    virtual void
    generatePrimaries(G4Event*)
    {}
  };
}

#endif // PRIMARY_GENERATOR_ACTION_BASE_HH
