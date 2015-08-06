
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
// #include "art/Framework/Services/Optional/RandomNumberGenerator.h"
#include "messagefacility/MessageLogger/MessageLogger.h" 

#include <iostream>
#include <cmath>
#include <memory>

// #include <vector>

// G4 (or derived) includes
//
#include "Geant4/G4RunManager.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4GeometryManager.hh"
#include "Geant4/G4StateManager.hh"
#include "Geant4/G4UImanager.hh"
#include "Geant4/G4CascadeParameters.hh"
#include "Geant4/FTFP_BERT.hh"
#include "Geant4/QGSP_FTFP_BERT.hh"
#include "Geant4/QGSP_BERT.hh"
//
#include "artg4tk/G4PhysModelParamStudy/G4Components/ModelParamStudyGeom.hh"
// --> old inc --> #include "artg4tk/G4PhysModelParamStudy/G4Components/HadInteractionSD.hh"
#include "artg4tk/pluginDetectors/gdml/HadInteractionSD.hh"
//
// --> old inc --> #include "artg4tk/G4PhysModelParamStudy/DataProd/ArtG4tkVtx.hh"
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkVtx.hh"
//
#include "artg4tk/DataProducts/EventGenerators/GenParticle.hh"
#include "artg4tk/DataProducts/EventGenerators/GenParticleCollection.hh"

namespace artg4tk {

  class ModelParamStudyProducer : public art::EDProducer {

  public:

    explicit ModelParamStudyProducer( const fhicl::ParameterSet& pset );
    virtual ~ModelParamStudyProducer();

    virtual void produce( art::Event& event ) override;
    virtual void beginJob()                   override;
    virtual void beginRun( art::Run&  )       override;
    virtual void endRun( art::Run& )          override;

  private:
  
     // local private methods to deal with 
     // modifications of physics model parameters
     //
     void fillBertiniDefaults();
     void printDefaults();
     void restoreDefaults();
     void modifyParameters( bool changestate=false );
     
     void clear();

    // data members
    
    // Diagnostics level (verbosity)
    //
    int         fVerbosity;
    
    // G4 (or derived) things 
    //
    // ModelParamStudyRunManager*     fRM;
    G4RunManager*            fRM;
    //
    G4UImanager*             fUI;
    // data members specifically to study parameters modification
    //
    bool                           fDefaultPhysics;
    //
    // NOTE(JVY): Ideally, here should be something like
    //            std::map< std::string, std::map<std::string, *** > >
    //            that'll tie together a model (by name) and a catalog 
    //            of available commands paired with D(efault)
    //            BUT !!! Bertini params are of different types (double, bool, etc.)
    //
    std::vector<std::string>       fCommands2ModifyBertini;
    std::vector<std::string>       fDefaultBertini;
    //
    std::string              fPhysList;
    //
    HadInteractionSD*        f1stHadIntSD;
    G4Event*                 fCurrentG4Event;
    //
    long                     fRandomSeed;
    
    // diagnostics printouts
    //
    mf::LogInfo fLogInfo;
    

  };

}

artg4tk::ModelParamStudyProducer::ModelParamStudyProducer( const fhicl::ParameterSet& p )
   : fLogInfo("ModelParamStudyProducer")
{
    
   fVerbosity       = p.get<int>("Verbosity",0);

   // Check the state
   //
   if(!G4StateManager::GetStateManager()->SetNewState(G4State_PreInit))
      fLogInfo << "G4StateManager PROBLEM! "; //  << G4endl;

   // Get the run manager and check if it's been initialized in any way
   // NOTE-1: attempt to call the ctor more than once will result
   //         in a fatal throw on the G4 side (in the ctor itself)
   // NOTE-2: attempt to call G4RunManager::GetRunManager() BEFORE
   //         it's initialized (via new) will result in return of
   //         a NULL pointer !
   //
   fRM              = new G4RunManager();

   // CRITICAL !!!
   // Model(s) (or at least Bertini) config/params should be in BEFORE
   // Physics List is instantiated/initialized !!!
   //
   fUI              = G4UImanager::GetUIpointer();
   // FIXME !!!
   // Need to put in a check: if list of mod.commands is NOT empty, it's NOT a "default" !!!   
   fDefaultPhysics  = p.get<bool>("DefaultPhysics",true);
   fCommands2ModifyBertini = p.get<std::vector<std::string> >("BertiniConfig");
   fillBertiniDefaults();
   if ( !fDefaultPhysics )
   {
      modifyParameters();
      std::cout << "RADIUS_SCALE " << G4CascadeParameters::radiusScale() << std::endl;
      std::cout << "XSEC_SCALE " << G4CascadeParameters::xsecScale() << std::endl;
   }
   //
   // Bertini allows a verbosity switch which has nothing to do with the model params/configuration
   // but it is useful for some debugging purposes, so we will pass in the app's general verbosity.
   //
   // NOTE: We will also do similar to other models when we get to it
   //
   std::ostringstream cmd;
   cmd << "/process/had/cascade/verbose " << fVerbosity;
//   std::cout << " cmd = " << cmd.str() << std::endl;
   fUI->ApplyCommand( cmd.str() );
//   std::cout << " Bertini verbosity " << G4CascadeParameters::verbose() << std::endl;
//   cmd.str( "" );
//   cmd.clear();

   // Setup phys.lists
   //
   fPhysList        = p.get<std::string>("PhysicsList","QGSP_FTFP_BERT");
   // Get the physics list and pass it to Geant and initialize the list if necessary
   //
   // FIXME !!!
   // For some reason, it works with artg4tk_Main but not here...
   // ... perhaps it's because of the (random?) order of instantiating Services ?
   //
   // art::ServiceHandle<PhysicsListHolderService> physicsListHolder;
   // fRunManager->SetUserInitialization( physicsListHolder->makePhysicsList() );
   //
   if ( fPhysList == "FTFP_BERT" )
   {
     fRM->SetUserInitialization( new FTFP_BERT() );
   }
   else if ( fPhysList == "QGSP_BERT" )
   {
     fRM->SetUserInitialization( new QGSP_BERT() );
   }
   else if ( fPhysList == "QGSP_FTFP_BERT" )
   {
     fRM->SetUserInitialization( new QGSP_FTFP_BERT() );
   }
   
   // Declare the detector construction to Geant
   // First, get the corresponding PSet for Geometry
   //
   const fhicl::ParameterSet& psgeom = p.get<fhicl::ParameterSet>("TargetGeom");
   fRM->SetUserInitialization( new ModelParamStudyGeom( psgeom) );
   fRM->GeometryHasBeenModified();

   // inits
   //
   fRM->Initialize();   
   fRM->ConfirmBeamOnCondition();
   fRM->ConstructScoringWorlds();
   fRM->RunInitialization(); // this is part of BeamOn 
                             // and needs be done (at least) to set GeomClosed status 

   f1stHadIntSD     = 0;

   fCurrentG4Event  = 0;


   fRandomSeed      = p.get<long>("RNDMSeed",-1);
   if ( fRandomSeed == -1) 
   {
	  // Construct seed from time and pid. (default behavior if 
	  // no seed is provided by the fcl file)
	  // Note: According to Kevin Lynch, the two lines below are not portable. 
	  fRandomSeed = time(0) + getpid();
	  fRandomSeed = ((fRandomSeed & 0xFFFF0000) >> 16) | ((fRandomSeed & 0x0000FFFF) << 16); //exchange upper and lower word
	  fRandomSeed = fRandomSeed % 900000000; // ensure the seed is in the correct range for engine
   }
   createEngine( fRandomSeed, "G4Engine" ); // inherited from "EngineCreator" base class
        
   produces<ArtG4tkVtx>();
   
}

artg4tk::ModelParamStudyProducer::~ModelParamStudyProducer()
{

   // no need to clean up UI pointer - I don't "new" it, I just "get it"

   if ( fCurrentG4Event ) delete fCurrentG4Event;
   
   fCommands2ModifyBertini.clear();
   fDefaultBertini.clear();

   fRM->RunTermination();
   delete fRM;
   
}

// At begin job
//
void artg4tk::ModelParamStudyProducer::beginJob()
{

   G4SDManager* sdman = G4SDManager::GetSDMpointer();
   f1stHadIntSD = dynamic_cast<HadInteractionSD*>(sdman->FindSensitiveDetector("HadInteractionSD"));
   
   return;

}

// At begin run
//
void artg4tk::ModelParamStudyProducer::beginRun( art::Run& )
{  
   
   if ( fVerbosity > 0 ) printDefaults();
      
   return;

}

void artg4tk::ModelParamStudyProducer::produce( art::Event& e )
{

   // first of all, fetch event from prinamry generator
   // (pgun from artg4tk/EventGenerators)
   //
   // do it "ByLabel" !!!
   // NOTE: in the config /artg4tk/G4PhysModelParamStudy/fcl/toy.fcl
   //       (module_type) EventGenerator is labeled "PrimaryGenerator"
   //       ...in principle, one can make it configurable what label 
   //       this particular analyzer wants...
   //
   art::Handle<GenParticleCollection> primgenparts;
   e.getByLabel( "PrimaryGenerator", primgenparts );
   if ( !primgenparts.isValid() )
   {
       fLogInfo << " primgen handle is NOT valid "; //  << std::endl;
       return;
   }

   G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
   
   fCurrentG4Event = new G4Event(e.id().event() );

   for ( GenParticleCollection::const_iterator i=primgenparts->begin(); i!=primgenparts->end(); ++i ) 
   {
      int pdgcode = i->pdgId();
      G4ParticleDefinition* g4pd = ptable->FindParticle( pdgcode );
      if ( !g4pd ) continue;
      //
      // G4PrimaryVertex*   g4vtx = new G4PrimaryVertex( i->position()*mm, 0. ); // 4th arg is time(ns)
      //
      // FIXME !!!
      // This needs to be configurable !!!
      //
      CLHEP::Hep3Vector pos(0.,0.,-1300.); // in mm !!!
      G4PrimaryVertex*   g4vtx = new G4PrimaryVertex( pos*CLHEP::mm, 0. ); // 4th arg is time(ns)
      const CLHEP::HepLorentzVector& mom = i->momentum();
      G4PrimaryParticle* g4prim = new G4PrimaryParticle( g4pd, 
                                                         mom.x()*CLHEP::GeV, mom.y()*CLHEP::GeV, mom.z()*CLHEP::GeV, 
							 mom.e()*CLHEP::GeV );        
      g4vtx->SetPrimary(g4prim);
      fCurrentG4Event->AddPrimaryVertex(g4vtx);
   }
   
   if ( fVerbosity > 0 )  
   {
      fLogInfo << "Processing art::Event " << e.id().event(); //  << "\n" << std::endl;   
      fLogInfo << "G4Event " << e.id().event() << " has been generated "; // << std::endl; 
      fCurrentG4Event->Print();
      int nprimvtx = fCurrentG4Event->GetNumberOfPrimaryVertex();
      for ( int i=0; i<nprimvtx; ++i )
      {
         fCurrentG4Event->GetPrimaryVertex(i)->Print();
      }
      // const G4String& version = fRM->GetVersionString();
      // fLogInfo << "Now Process it through " << version; // << std::endl;
   }
   
   // now either restore defaults or modify parameters if needs be
   //
   // NOTE (JVY): Unfortunately, this scenario does NOT work as modifications only make
   //             to the G4CascadeParameters "central holder" but NOT TO THE GUTS of Bertini
   //             (e.g. G4NucleiModel takes the params via its ctor, and that's it...) 
   // 
/*
   if ( fDefaultPhysics )
   {
      restoreDefaults();
   }
   else
   {
      modifyParameters();
   }
*/   
   // fRMP->ProcessOneEvent( fCurrentG4Event );
   G4EventManager::GetEventManager()->ProcessOneEvent( fCurrentG4Event );
   if ( fVerbosity > 0 )
   {
      fLogInfo << "G4Event " << e.id().event() << " has been processed by EventManager "; // << std::endl;
   } 
   fRM->AnalyzeEvent( fCurrentG4Event );
   // fRM->UpdateScoring(); // can NOT use it outside G4RunManager (or deribved) since this method is protected

//--------------------------

   // get the "hits" from SensitiveDetector (see also beginJob)
   //
   if ( !f1stHadIntSD ) 
   {
      fLogInfo << " Invalid HadInteractionSD ptr "; // << std::endl;
      return;
   }

   const ArtG4tkVtx& inter = f1stHadIntSD->Get1stInteraction();
   
   if ( inter.GetNumOutcoming() <= 0 )
   {
      clear();
      return;
   }
      
   // Create a (no longer empty!) output data product
   //
   std::unique_ptr<ArtG4tkVtx> firstint(new ArtG4tkVtx(inter));

   // Product Id of the data product to be created; needed for persistent pointers.
   art::ProductID firstintID(getProductID<ArtG4tkVtx>(e));

   // Put the output collection into the event
   //
   e.put(std::move(firstint));

   clear();
   return;
    
}

// At end run
//
void artg4tk::ModelParamStudyProducer::endRun( art::Run& )
{
   
   return;

}

void artg4tk::ModelParamStudyProducer::fillBertiniDefaults()
{

   // NOTE(JVY): this procedure is quite boring; need to figure out if there's
   //            a more "algorithmic" approach...
   //
   
   std::ostringstream cmd;
   
   // general purpose verbosity switch
   // has nothing to do with the model params/configuration
   // but is useful for some debugging purposes
   //
   cmd << "process/had/cascade/verbose " << G4CascadeParameters::verbose();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
   
   cmd << "/process/had/cascade/doCoalescence " << G4CascadeParameters::doCoalescence();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
/*
// these params/methods are NOT available in G4.9.6-series
// uncomment when switch to 10.1-ref03 or later !!!
   
   cmd << "/process/had/cascade/piNAbsorption " << G4CascadeParameters::piNAbsorption();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
   cmd << "/process/had/cascade/use3BodyMom " << G4CascadeParameters::use3BodyMom();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
   cmd << "/process/had/cascade/usePhaseSpace " << G4CascadeParameters::usePhaseSpace();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
*/
   cmd << "/process/had/cascade/useBestNuclearModel false"; // no corresponding access method; unclear D !!!
                                                            // From G4CascadeParameters.cc: BEST_PAR = (0!=G4NUCMODEL_USE_BEST);
							    // probably means that if env.var. is NOT set, this option isn't in use
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();

   cmd << "/process/had/cascade/useTwoParamNuclearRadius " << G4CascadeParameters::useTwoParam();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
   cmd << "/process/had/cascade/nuclearRadiusScale " << G4CascadeParameters::radiusScale();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
   cmd << "/process/had/cascade/smallNucleusRadius " << G4CascadeParameters::radiusSmall();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
   cmd << "/process/had/cascade/alphaRadiusScale " << G4CascadeParameters::radiusAlpha();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
   cmd << "/process/had/cascade/shadowningRadius " << G4CascadeParameters::radiusTrailing(); 
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
   cmd << "/process/had/cascade/fermiScale " << G4CascadeParameters::fermiScale();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
   cmd << "/process/had/cascade/crossSectionScale " << G4CascadeParameters::xsecScale();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
   cmd << "/process/had/cascade/gammaQuasiDeutScale " << G4CascadeParameters::gammaQDScale();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
   cmd << "/process/had/cascade/cluster2DPmax " << G4CascadeParameters::dpMaxDoublet();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
   cmd << "/process/had/cascade/cluster3DPmax " << G4CascadeParameters::dpMaxTriplet();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();
   cmd << "/process/had/cascade/cluster4DPmax " << G4CascadeParameters::dpMaxAlpha();
   fDefaultBertini.push_back( cmd.str() );
   cmd.str( "" );
   cmd.clear();

   return;

}
void artg4tk::ModelParamStudyProducer::printDefaults()
{

   fLogInfo << "BERTINI Cascade Default Parameters: \n"; // << std::endl;
   //std::cout << std::endl;
   for ( unsigned int i=0; i<fDefaultBertini.size(); ++i )
   {
      fLogInfo << fDefaultBertini[i] << "\n"; // << std::endl;
   }
   
   return;

}
void artg4tk::ModelParamStudyProducer::restoreDefaults()
{
   
   if ( fDefaultPhysics )
   {
      // Change G4State to Idle
      // G4UImanager will only apply modifications if
      // 1. G4State_Idle
      // 2. G4State_PreInit
      // 
      // Once run is initialized and prepared to start, the state is set to GeomClosed;
      // for this reason, G4UIcommend::IsAvailable will return false, and G4UImanager will NOT apply it
      //
      if(!G4StateManager::GetStateManager()->SetNewState(G4State_Idle))            
         fLogInfo << "G4StateManager PROBLEM! "; // << std::endl;
      // run over the container of default Bertini parameters
      // and restore defaults
      for ( unsigned int i=0; i<fDefaultBertini.size(); ++i )
      {
//         std::ostringstream cmd;
//         cmd << (it->first).c_str() << " " << it->second ;
//         fUI->ApplyCommand( cmd.str() );
         fUI->ApplyCommand( (fDefaultBertini[i]).c_str() );
	 if (fVerbosity > 0 ) fLogInfo << "Default restored: " << fDefaultBertini[i] << "\n"; // << std::endl;
      }
      //
      // Now return G4State back to GeomClosed
      //
      if(!G4StateManager::GetStateManager()->SetNewState(G4State_GeomClosed))            
         fLogInfo << "G4StateManager PROBLEM! "; // << std::endl;
   }
   
   return;

}

void artg4tk::ModelParamStudyProducer::modifyParameters( bool changestate )
{

   if ( fDefaultPhysics ) return; // better if spit a warning !

   // Change G4State to Idle because...
   // ...once initialized, G4UImanager will only apply modifications if
   // 1. G4State_Idle
   // 2. G4State_PreInit
   // 
   // Once run is initialized and prepared to start, the state is set to GeomClosed;
   // for this reason, G4UIcommend::IsAvailable will return false, and G4UImanager will NOT apply it
   //
   if ( changestate )
   {
      if(!G4StateManager::GetStateManager()->SetNewState(G4State_Idle))            
         fLogInfo << "G4StateManager PROBLEM! "; // << std::endl;
   }
   
   // run over params to modify
   //
   for ( unsigned int i=0; i<fCommands2ModifyBertini.size(); ++i )
   {
      fUI->ApplyCommand( (fCommands2ModifyBertini[i]).c_str() );
      if (fVerbosity > 0 ) 
      {
         fLogInfo << "Modification applied: " << fCommands2ModifyBertini[i] << "\n"; // << std::endl;
      }
      std::cout << "Modification applied: " << fCommands2ModifyBertini[i] << "\n" << std::endl;
   }
   
   // Now rerutn the State back to GeomClosed
   //
   if ( changestate )
   {
      if(!G4StateManager::GetStateManager()->SetNewState(G4State_GeomClosed))            
         fLogInfo << "G4StateManager PROBLEM! "; // << std::endl;
   }
   
   return;

}

void artg4tk::ModelParamStudyProducer::clear()
{

   f1stHadIntSD->clear();
   
   // NOTE(JVY): in principle, one can call G4RunManager::TerminateOneEvent() method,
   //            where a G4Event gets deleted unless it's marked "toBeKept"...
   //            ... which is not the case/goal for this app
   //
   delete fCurrentG4Event;
   fCurrentG4Event = 0;

   return;

}

DEFINE_ART_MODULE(artg4tk::ModelParamStudyProducer)
