// This file provides the implementation for an action object that records how
// long runs take and outputs its results.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: August 2012

#include "artg4tk/pluginActions/clock/ClockAction_service.hh"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4tk/services/ActionHolder_service.hh"

using std::string;

artg4tk::ClockActionService::ClockActionService(fhicl::ParameterSet const & p, 
					      art::ActivityRegistry &)
  : RunActionBase(p.get<string>("name","clock")),
    // Set the clock to use 'real time'.
    start_(), // These have different types on MAC and LINUX
    end_(),   // but they are defined in the same relative order
#ifdef __MACH__
    cclock_(),
    mts_(),
#else
    clockID_(CLOCK_REALTIME),
#endif
    // Initialize our message logger
    logInfo_("ClockAction")
{  }

// Destructor
artg4tk::ClockActionService::~ClockActionService()
{}


// Overload beginOfRunAction, called at the beginning of each run.
// Here we assign a value to the start clock.
void artg4tk::ClockActionService::beginOfRunAction(const G4Run * )//currentRun)
{
  // Find the current time and assign it to our member variable @start@
#ifdef __MACH__
mach_clock_get_time(start_);
#else
  clock_gettime(clockID_, &start_);
#endif
}

// Overload EndOfRunAction, called at the end of each run.
// Here we assign a value to the end clock and print how long the run took.
 void artg4tk::ClockActionService::endOfRunAction(const G4Run * )//currentRun)
{
  // Find the current time and assign it to our member variable @end@
#ifdef __MACH__
mach_clock_get_time(end_);
#else
  clock_gettime(clockID_, &end_);
#endif
  // Print out how long the run took
  logInfo_ << "Elapsed time this run: " 
	   << clockDiff() << " seconds\n";
}



// This private member function finds the time difference between the two
// provided timespec values, in seconds.
double artg4tk::ClockActionService::clockDiff()
{
  // mach_timespec_t and timespec both have the internal vars:
  // tv_nsec and tv_sec, so just let the compiler figure out the
  // type of temp and do the same calculation. In the end, we just
  // return a double

#ifdef __MACH__
  mach_timespec_t temp;
#else
  timespec temp;
#endif
  
  if ((end_.tv_nsec-start_.tv_nsec)<0) {
    temp.tv_sec = end_.tv_sec-start_.tv_sec-1;
    temp.tv_nsec = 1000000000+end_.tv_nsec-start_.tv_nsec;
  } else {
    temp.tv_sec = end_.tv_sec-start_.tv_sec;
    temp.tv_nsec = end_.tv_nsec-start_.tv_nsec;
  }
  return temp.tv_sec + temp.tv_nsec/1e9;
}//ClockActionService::clockDiff()

#ifdef __MACH__

// Take a reference to the start_ or end_ time.
// clock_get_time takes a pointer to a mach_timespec_t as its 2nd var, so 
// I pass the address of someTime
void artg4tk::ClockActionService::mach_clock_get_time(mach_timespec_t &someTime){
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock_);
  clock_get_time(cclock_, &someTime);
  mach_port_deallocate(mach_task_self(), cclock_); 
}

#endif


using artg4tk::ClockActionService;
DEFINE_ART_SERVICE(ClockActionService)
