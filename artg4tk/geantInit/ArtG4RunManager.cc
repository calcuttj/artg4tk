//
// Override the G4RunManager class so that the ArtG4 framework can drive
// the event loop.
//
// Original author Rob Kutschke
//
//
// Notes:
// 1) In G4RunManager the counter i_event is used as the event number.
//    In this code it is taken from the event number of the art::event.
//
// Implementation file for @ArtG4RunManager@

// ArtG4 includes.
#include "artg4tk/geantInit/ArtG4RunManager.hh"

// Includes from G4.
#include "Geant4/G4Run.hh"
#include "Geant4/G4Timer.hh"
#include "Geant4/G4UImanager.hh"

using namespace std;

namespace artg4tk {

  // If the c'tor is called a second time, the c'tor of base will
  // generate an exception.
  ArtG4RunManager::ArtG4RunManager()
    : G4RunManager()
    , macroFile_(0)
    , n_select_(-1)
    , nProcessed_(0)
    , realElapsed_(0.)
    , systemElapsed_(0.)
    , userElapsed_(0.)
    , msg_("")
  {}

  // Destructor of base is called automatically.  No need to do anything.
  ArtG4RunManager::~ArtG4RunManager() {}

  // Do the "begin run" parts of BeamOn.
  void
  ArtG4RunManager::BeamOnBeginRun(unsigned int runNumber, const char* macroFile, G4int n_select)
  {

    SetRunIDCounter(runNumber);

    bool cond = ConfirmBeamOnCondition();
    if (!cond) {
      // throw here
      return;
    }

    // Initialize member data for the new run.
    macroFile_ = macroFile;
    n_select_ = n_select;
    msg_ = "";
    nProcessed_ = 0;
    realElapsed_ = 0;
    systemElapsed_ = 0;
    userElapsed_ = 0;

    // Construct command to execute the macro.
    if (macroFile_ != 0) {
      msg_ = "/control/execute ";
      msg_ += macroFile_;
    } else {
      n_select_ = -1;
    }

    numberOfEventToBeProcessed = 1;
    ConstructScoringWorlds();
    RunInitialization();
  }

  // Do the "per event" part of DoEventLoop.
  void
  ArtG4RunManager::BeamOnDoOneEvent(int eventNumber)
  {

    timer->Start();

    // This is the body of the event loop from DoEventLoop().
    currentEvent = GenerateEvent(eventNumber);
    eventManager->ProcessOneEvent(currentEvent);
    AnalyzeEvent(currentEvent);
    UpdateScoring();

    if (nProcessed_ < n_select_)
      G4UImanager::GetUIpointer()->ApplyCommand(msg_);

    // Should pause, not stop, if I can do that.
    timer->Stop();

    // Accumulate time spent in G4 for all events in this run.
    realElapsed_ += timer->GetRealElapsed();
    systemElapsed_ += timer->GetSystemElapsed();
    userElapsed_ += timer->GetUserElapsed();

    if (verboseLevel > 0) {

      G4int oldPrecision = G4cout.precision(3);
      std::ios::fmtflags oldFlags = G4cout.flags();
      G4cout.setf(std::ios::fixed, std::ios::floatfield);

      G4cout << "TimeEvent> " << eventNumber << " "
             << G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID() << " "
             << timer->GetRealElapsed() << " "
             << timer->GetUserElapsed() + timer->GetSystemElapsed() << " "
             << userElapsed_ + systemElapsed_ << G4endl;

      G4cout.setf(oldFlags);
      G4cout.precision(oldPrecision);
    }
  }

  void
  ArtG4RunManager::BeamOnEndEvent()
  {

    StackPreviousEvent(currentEvent);
    currentEvent = 0;

    ++nProcessed_;
  }

  // Do the "end of run" parts of DoEventLoop and BeamOn.
  void
  ArtG4RunManager::BeamOnEndRun()
  {

    // From G4RunManager::DoEventLoop
    if (verboseLevel > 0) {

      G4cout << "G4Run terminated." << G4endl;
      G4cout << "G4Run Summary" << G4endl;
      if (runAborted) {
        G4cout << "  G4Run Aborted after " << nProcessed_ << " events processed." << G4endl;
      } else {
        G4cout << "  Number of events processed : " << nProcessed_ << G4endl;
      }
      G4cout << "  User=" << userElapsed_ << "s Real=" << realElapsed_ << "s Sys=" << systemElapsed_
             << G4endl;

      G4int oldPrecision = G4cout.precision(3);
      std::ios::fmtflags oldFlags = G4cout.flags();
      G4cout.setf(std::ios::fixed, std::ios::floatfield);

      G4cout << "TimeReport> Time report complete in ";
      G4cout << realElapsed_;
      G4cout << " seconds" << G4endl;

      G4cout.setf(oldFlags);
      G4cout.precision(oldPrecision);
    }

    // From G4RunManager::BeamOn.
    RunTermination();
  }

} // end namespace artg4tk
