// This file is the header for the @TrackingActionBase@ class.

// A base class for Geant user tracking actions

// There is no implementation file, as it is intended only as a base class.

// When you create an action object, you have the option to overload the
// following methods:

// @PreUserTrackingAction@ - This method, which takes a pointer to a constant
// @G4Track@ object, is called once after a track has been created but before
// it is simulated. Putting code in this method of an action object is
// equivalent to putting it in the @PreUserTrackingAction@ method of the
// simulation's tracking action class.

// @PostUserTrackingAction@ - This method, which takes a pointer to a constant
// @G4Track@ object, is called once after a track has been stopped. Putting
// code in this method of an action object is equivalent to putting it in the
// @PostUserTrackingAction@ method of the simulation's tracking action class.

#ifndef artg4tk_actionBase_TrackingActionBase_hh
#define artg4tk_actionBase_TrackingActionBase_hh

#include <string>

#include "artg4tk/actionBase/ActionBase.hh"

// Declarations of types we use as input parameters
class G4Track;

// Everything goes in the Art G4 namespace
namespace artg4tk {

  class TrackingActionBase : public ActionBase {
  public:
    // Constructor. The derived class must call this constructor. It takes a
    // single string for the name of the action object.
    explicit TrackingActionBase(std::string name);

    // Destructor
    virtual ~TrackingActionBase();

    // h3. The interesting methods.
    // All of these are defined to do nothing by default. Users can override
    // them if desired, and if they're not overloaded, they do nothing.

    // Called before a track is simulated
    virtual void
    preUserTrackingAction(const G4Track*)
    {}

    // Called when a track is stopped
    virtual void
    postUserTrackingAction(const G4Track*)
    {}
  };
}

#endif /* artg4tk_actionBase_TrackingActionBase_hh */
