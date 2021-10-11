// This file is the header for the @SteppingActionBase@ class.

// Base class for Geant user stepping action

// When you create an action object, you have the option to overload the
// following methods:

// @UserSteppingAction@ - This method, which takes a pointer to a constant
// @G4Step@ object, is called at the end of each step (I think - the
// documentation is a little vague). Putting code in this method of an action
// object is equivalent to putting it in the @UserSteppingAction@ method of
// the simulation's stepping action class.

#ifndef artg4tk_actionBase_SteppingActionBase_hh
#define artg4tk_actionBase_SteppingActionBase_hh

#include <string>

#include "artg4tk/actionBase/ActionBase.hh"

// Declarations of types we use as input parameters
class G4Step;

// Everything goes in the Art G4 namespace
namespace artg4tk {

  class SteppingActionBase : public ActionBase {
  public:
    // Constructor. The derived class must call this constructor. It takes a
    // single string for the name of the action object.
    explicit SteppingActionBase(std::string name);

    // Destructor
    virtual ~SteppingActionBase();

    // h3. The interesting methods.
    // All of these are defined to do nothing by default. Users can override
    // them if desired, and if they're not overloaded, they do nothing.

    // Called at the end of each step
    virtual void
    userSteppingAction(const G4Step*)
    {}
  };
}

#endif /* artg4tk_actionBase_SteppingActionBase_hh */
