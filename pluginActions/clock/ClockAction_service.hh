// ClockActionService is the service that manages the clock for the simulation.
// It records how long a run takes and prints its result.
// To use this action, all you need to do is put it in the services section
// of the configuration file, like this:
// 
// services: { 
//   ...
//   user: {
//     ClockActionService: {}
//     ...
//   }
// }

// Expected parameters:

// - name (string): A name describing the action.
//       Default is 'clock'.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: August 2012
// Authors: Brendan Kiburg
// Data: Dec 2012 --> Make portable to MAC
// Include guard
#ifndef CLOCKACTION_SERVICE_HH
#define CLOCKACTION_SERVICE_HH

// Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Core/EDProducer.h"

#include "messagefacility/MessageLogger/MessageLogger.h"


// This is necessary because the time libraries are implemented 
// differently on the MAC
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#endif

// Get the base class
#include "artg4tk/actionBase/RunActionBase.hh"

namespace artg4tk {

  class ClockActionService : public RunActionBase {
  public: 
    ClockActionService(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~ClockActionService();

    // Overload the BeginOfRunAction method to initialize the clock
    virtual void beginOfRunAction(const G4Run *) override;

    // Overload the EndOfRunAction method to finalize the clock and print out
    // how long the run took.
    virtual void endOfRunAction(const G4Run *) override;

    // We don't add anything to the event, so we don't need callArtProduces
    // or FillEventWithArtStuff.
    
  private:

#ifdef __MACH__

    mach_timespec_t start_;
    mach_timespec_t end_;
    
    clock_serv_t cclock_;
    mach_timespec_t mts_;
    
    // Take a reference to either the start_ or the end_
    void mach_clock_get_time(mach_timespec_t &someTime);
#else
    // timespec values for the start and end of the run;
    timespec start_;
    timespec end_;
    
    clockid_t clockID_;
    // A method to find the difference between two timespec values  
    
#endif
    // A message logger for this action
    mf::LogInfo logInfo_;

    // The diff will use the internal variables rather than being passed
    // variables, and I will protect against MAC/LINUX timespec types
    double clockDiff();
    
    // An identifier to say whether we care about real time (CLOCK_REALTIME)
    // or absolute time (TIMER_ABSTIME).
    // (see <time.h> documentation for more information)

  };
}

using artg4tk::ClockActionService;
DECLARE_ART_SERVICE(ClockActionService,LEGACY)

#endif
