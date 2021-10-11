// artg4tkMain is the main producer module for Geant.

// artg4tkMain_module.cc replicates many GEANT programs' @main()@ driver. It
// creates and initializes the run manager, controls the beginning and end of
// events, and controls visualization.

// Authors: Tasha Arvanitis, Adam Lyon

// Date: July 2012

// Art includes
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// Local includes (like actions)
#include "artg4tk/geantInit/ArtG4DetectorConstruction.hh"
#include "artg4tk/geantInit/ArtG4RunManager.hh"

// The actions
#include "artg4tk/geantInit/ArtG4EventAction.hh"
#include "artg4tk/geantInit/ArtG4PrimaryGeneratorAction.hh"
#include "artg4tk/geantInit/ArtG4StackingAction.hh"
#include "artg4tk/geantInit/ArtG4SteppingAction.hh"
#include "artg4tk/geantInit/ArtG4TrackingAction.hh"

// Services
#include "artg4tk/services/ActionHolder_service.hh"
#include "artg4tk/services/DetectorHolder_service.hh"
#include "artg4tk/services/PhysicsListHolder_service.hh"

// Geant4 includes
#include "Geant4/G4UImanager.hh"
#include "Geant4/G4UIterminal.hh"

using namespace std;

namespace artg4tk {

  // Define the producer

  class artg4tkMain : public art::EDProducer {
  public:
    // Constructor
    explicit artg4tkMain(fhicl::ParameterSet const& p);

    // Destructor
    virtual ~artg4tkMain();

    // Overriding producer members
    void produce(art::Event& e) override;
    void beginJob() override;
    void beginRun(art::Run& r) override;
    void endRun(art::Run&) override;

  private:
    // Our custom run manager
    unique_ptr<ArtG4RunManager> runManager_;

    // G4 session and managers
    G4UIsession* session_;
    G4UImanager* UI_;

    // Pseudorandom engine seed (originally hardcoded to 12345),
    // obtained from the configuration file.
    // Note: the maximum seed value is 9e8, which is potentially larger
    // than a long can hold.
    long seed_;

    // Determine whether we should use visualization
    // False by default, can be set by config file
    //   bool enableVisualization_;

    // Directory path(s), in colon-delimited list, in which we should look for
    // macros, or the name of an environment variable containing that path.
    // Contains only the current directory ('.') by default, but can be
    // set by config file
    string macroPath_;

    // And a tool to find files along that path
    // Initialized based on macroPath_.
    cet::search_path pathFinder_;

    // Name of a macro file for visualization
    // 'vis.mac' by default, and can be customized by config file.
    string visMacro_;

    // Boolean to determine whether we pause execution after each event
    // If it's true, then we do. Otherwise, we pause only after all events
    // have been produced.
    // False by default, can be changed by afterEvent in FHICL
    bool pauseAfterEvent_;

    // Boolean to determine whether we're in "visualize only certain
    // events" mode. If so, we pause and show the visualization only after
    // the given events. Turning this on only works if visualization is
    // also enabled, and it will pause, pass, or bring up a UI at the end
    // of the given events, as specified by afterEvent.
    bool visSpecificEvents_;

    // If we're in "visualize only certain events" mode, this vector
    // contains the events for which the visualization should be displayed.
    // This is a map because determining whether an event is in there is
    // O(log(n)), rather than O(n) for a vector, and find(...) is a heck of
    // a lot more convenient than looping over the vector.
    std::map<int, bool> eventsToDisplay_;

    // Run diagnostic level (verbosity)
    int rmvlevel_;

    // When to pop up user interface
    bool uiAtBeginRun_;
    bool uiAtEndEvent_; // set by afterEvent in FHICL

    // What to do at the end of the event
    // Choices are
    //     pass -- do nothing
    //     pause -- Let user press return at the end of each event
    //     ui    -- show the UI at the end of the event
    std::string afterEvent_;

    // Message logger
    mf::LogInfo logInfo_;
  };
}

// Constructor - set parameters

artg4tk::artg4tkMain::artg4tkMain(fhicl::ParameterSet const& p)
  : art::EDProducer(p)
  , runManager_()
  , session_(0)
  , UI_(0)
  , seed_(p.get<long>("seed", -1))
  , macroPath_(p.get<std::string>("macroPath", "."))
  , pathFinder_(macroPath_)
  , visMacro_(p.get<std::string>("visMacro", "vis.mac"))
  , pauseAfterEvent_(false)
  , visSpecificEvents_(p.get<bool>("visualizeSpecificEvents", false))
  , eventsToDisplay_()
  , rmvlevel_(p.get<int>("rmvlevel", 0))
  , uiAtBeginRun_(p.get<bool>("uiAtBeginRun", false))
  , uiAtEndEvent_(false)
  , afterEvent_(p.get<std::string>("afterEvent", "pass"))
  , logInfo_("ArtG4Main")
{
  // If we're in "visualize specific events" mode (essentially only pause
  // after given events), then extract the list of events we need to
  // pause for. They are placed in a map because it is more efficient to
  // determine whether a given entry is present in the map than a vector.
  if (visSpecificEvents_) {
    std::vector<int> eventsToDisplayVec = p.get<vector<int>>("eventsToDisplay");
    for (size_t i = 0; i < eventsToDisplayVec.size(); i++) {
      eventsToDisplay_[eventsToDisplayVec[i]] = true;
    }
    // Would be nice to have error checking here, but for now, if you
    // do something silly, it'll probably just crash.
  }

  // We need all of the services to run @produces@ on the data they will store. We do this
  // by retrieving the holder services.
  art::ServiceHandle<ActionHolderService> actionHolder;
  art::ServiceHandle<DetectorHolderService> detectorHolder;
  detectorHolder->initialize();
  // hjw:
  // detectorHolder -> callArtProduces(this);
  // Build the detectors' logical volumes
  detectorHolder->constructAllLVs();
  // And running @callArtProduces@ on each
  actionHolder->callArtProduces(producesCollector());
  detectorHolder->callArtProduces(producesCollector());

  // Set up the random number engine.
  // See the documentation in RandomNumberHeader.h for
  // how this works. Note that @createEngine@ is a member function
  // of our base class (actually, a couple of base classes deep!).
  // Note that the name @G4Engine@ is special.
  if (seed_ == -1) {
    // Construct seed from time and pid. (default behavior if
    // no seed is provided by the fcl file)
    // Note: According to Kevin Lynch, the two lines below are not portable.
    seed_ = time(0) + getpid();
    seed_ =
      ((seed_ & 0xFFFF0000) >> 16) | ((seed_ & 0x0000FFFF) << 16); // exchange upper and lower word
    seed_ = seed_ % 900000000; // ensure the seed is in the correct range for createEngine
  }
  createEngine(seed_, "G4Engine");

  // Handle the afterEvent setting
  if (afterEvent_ == "ui") {
    uiAtEndEvent_ = true;
  } else if (afterEvent_ == "pause") {
    pauseAfterEvent_ = true;
  }
}

// Destructor

artg4tk::artg4tkMain::~artg4tkMain() {}

// At begin job

void
artg4tk::artg4tkMain::beginJob()
{
  // Set up run manager
  mf::LogDebug("Main_Run_Manager") << "In begin job";
  runManager_.reset(new ArtG4RunManager);
}

// At begin run

void
artg4tk::artg4tkMain::beginRun(art::Run& r)
{
  // Get the physics list and pass it to Geant4and initialize the list if necessary
  art::ServiceHandle<PhysicsListHolderService> physicsListHolder;
  runManager_->SetUserInitialization(physicsListHolder->makePhysicsList());

  // Get all of the detectors and initialize them
  art::ServiceHandle<DetectorHolderService> detectorHolder;

  // Declare the detector construction to Geant
  runManager_->SetUserInitialization(
    new ArtG4DetectorConstruction{detectorHolder->worldPhysicalVolume()});

  // Get all of the actions and initialize them
  art::ServiceHandle<ActionHolderService> actionHolder;
  actionHolder->initialize();

  // Store the run in the action holder
  actionHolder->setCurrArtRun(r);

  // Declare the primary generator action to Geant
  runManager_->SetUserAction(new ArtG4PrimaryGeneratorAction{actionHolder.get()});

  // Note that these actions (and ArtG4PrimaryGeneratorAction above) are all
  // generic actions that really don't do much on their own. Rather, to
  // use the power of actions, one must create action objects (derived from
  // @ActionBase@) and register them with the Art @ActionHolder@ service.
  // See @ActionBase@ and/or @ActionHolderService@ for more information.
  runManager_->SetUserAction(new ArtG4SteppingAction{actionHolder.get()});
  runManager_->SetUserAction(new ArtG4StackingAction{actionHolder.get()});
  runManager_->SetUserAction(new ArtG4EventAction{actionHolder.get(), detectorHolder.get()});
  runManager_->SetUserAction(new ArtG4TrackingAction{actionHolder.get()});

  runManager_->Initialize();
  physicsListHolder->initializePhysicsList();

  // get the pointer to the User Interface manager
  UI_ = G4UImanager::GetUIpointer();
  // Open a UI if asked
  if (uiAtBeginRun_) {
    session_ = new G4UIterminal;
    session_->SessionStart();
    delete session_;
  }

  // Start the Geant run!
  runManager_->BeamOnBeginRun(r.id().run());
}

// Produce the Geant event

void
artg4tk::artg4tkMain::produce(art::Event& e)
{
  // The holder services need the event
  art::ServiceHandle<ActionHolderService> actionHolder;
  art::ServiceHandle<DetectorHolderService> detectorHolder;

  actionHolder->setCurrArtEvent(e);
  detectorHolder->setCurrArtEvent(e);

  // Begin event
  runManager_->BeamOnDoOneEvent(e.id().event());

  logInfo_ << "Producing event " << e.id().event() << "\n" << endl;

  // Done with the event
  runManager_->BeamOnEndEvent();
}

// At end run

void
artg4tk::artg4tkMain::endRun(art::Run& r)
{
  art::ServiceHandle<ActionHolderService> actionHolder;
  actionHolder->setCurrArtRun(r);

  runManager_->BeamOnEndRun();
}

DEFINE_ART_MODULE(artg4tk::artg4tkMain)
