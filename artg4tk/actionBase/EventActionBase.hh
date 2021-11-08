// This file is the header for the @EventActionBase@ class.

// A base class for Geant user event actions

// When you create an event action object, you have the option to overload the
// following methods:

// @BeginOfEventAction@ - This method, which takes a pointer to a constant
// @G4Event@ object, is called once at the beginning of each event. Note that
// this occurs after the primaries have been constructed and passed to the
// event manager, but before anything is simulated. Putting code in this
// method of an action object is equivalent to putting it in the
// @BeginOfEventAction@ method of the simulation's event action class.

// @EndOfEventAction@ - This method, which takes a pointer to a constant
// @G4Event@ object, is called once at the end of each event. This is the
// last time the event object is accessible (right before the state changes
// from EventProc (in which the event is accessible) to GeomClosed). Putting
// code in this method of an action object is equivalent to putting it in the
// @EndOfEventAction@ method of the simulation's event action class.

#ifndef artg4tk_actionBase_EventActionBase_hh
#define artg4tk_actionBase_EventActionBase_hh

#include <string>

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4tk/services/ActionHolder_service.hh"

#include "artg4tk/actionBase/ActionBase.hh"

// Declarations of types we use as input parameters
class G4Event;

// Everything goes in the Art G4 namespace
namespace artg4tk {

  class EventActionBase : public ActionBase {
  public:
    // Constructor. The derived class must call this constructor. It takes a
    // single string for the name of the action object.
    explicit EventActionBase(std::string name);

    // Destructor
    virtual ~EventActionBase();

    // Tell Art what this detector will put into the event. You do not need to
    // call this yourself.
    void
    callArtProduces(art::ProducesCollector& collector)
    {
      doCallArtProduces(collector);
    }

    // h3. The interesting methods.
    // All of these are defined to do nothing by default. Users can override
    // them if desired, and if they're not overloaded, they do nothing.

    // Called at the beginning of each event (after creation of primaries)
    virtual void
    beginOfEventAction(const G4Event*)
    {}

    // Called at the end of each event
    virtual void
    endOfEventAction(const G4Event*)
    {}

  private:
    // Tell Art what you will put into the event.
    virtual void
    doCallArtProduces(art::ProducesCollector&)
    {}
  };
}

#endif /* artg4tk_actionBase_EventActionBase_hh */
