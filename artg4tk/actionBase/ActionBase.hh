// This file is the header for the @ActionBase@ class.

// It is the lowest level base class for Geant user actions. Do not
// inherit from this class directly. Instead, use a next level up class, like
// @TrackingActionBase@.

#ifndef artg4tk_actionBase_ActionBase_hh
#define artg4tk_actionBase_ActionBase_hh

#include <string>

namespace artg4tk {

  class ActionBase {
  public:
    // Constructor. The derived class must call this constructor. It takes a
    // single string for the name of the action object.
    ActionBase(std::string myName) : myName_(move(myName)) {}

    // Destructor
    virtual ~ActionBase();

    // Accessor
    std::string const&
    myName() const
    {
      return myName_;
    }

    // h3. Optional methods

    // Intialize - Instead of putting initialization code into your constructor, put
    // such code in this initialize method. This method will get called at the correct
    // time, after particle lists are already constructed and known to geant.
    virtual void
    initialize()
    {}

  private:
    // A string containing this action object's name
    std::string myName_;
  };
}

#endif /* artg4tk_actionBase_ActionBase_hh */
