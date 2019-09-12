// This file is the header for the @StackingActionBase@ class.
// All action objects MUST inherit from this class.

// The usual method in @G4UserStackingAction@ is @ClassifyNewTrack@. Here, you instead 
// supply a function for @killNewTrack@, which returns @true@ if the track should be killed
// and @false@ if the track should remain in the Urgent list (there is no capability to 
// put a track on the waiting list). See @artg4tk/geantInit/ArtG4StackingAction.hh@ and @.cc@ for 
// how this class is handled. 

// Include guard
#ifndef STACKING_ACTION_BASE_HH
#define STACKING_ACTION_BASE_HH

#include <string>

#include "artg4tk/actionBase/ActionBase.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4tk/services/ActionHolder_service.hh"

class G4Track;


namespace artg4tk {

    class StackingActionBase : public ActionBase {
    public:
        // Constructor. The derived class must call this constructor. It takes a 
        // single string for the name of the action object.

        StackingActionBase(std::string myName)
        : ActionBase(myName) {
            art::ServiceHandle<artg4tk::ActionHolderService> actionHolder;
            actionHolder->registerAction(this);
        }

        // Destructor
        virtual ~StackingActionBase();

        // killNewTrack (see above)
        virtual bool killNewTrack( const G4Track* ) { return false; }

    };
}

#endif // STACKING_ACTION_BASE_HH
