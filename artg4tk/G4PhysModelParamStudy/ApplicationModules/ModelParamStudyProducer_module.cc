
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

// needed when doing geom via GDML
//
#include "artg4tk/services/DetectorHolder_service.hh"

#include "artg4tk/services/PhysicsListHolder_service.hh"
// --> seems to come via some other inc #include "art/Framework/Services/Optional/RandomNumberGenerator.h"
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

//
#include "artg4tk/G4PhysModelParamStudy/G4Services/PhysModelConfig_service.hh"
//
#include "artg4tk/geantInit/ArtG4DetectorConstruction.hh"
//
#include "artg4tk/DataProducts/G4ModelConfig/ArtG4tkModelConfig.hh" // Run product;
                                                                    // NOTE: Event product will be done 
								    //       via GDMLDetectorService/SensDet
//
#include "artg4tk/DataProducts/EventGenerators/GenParticle.hh"
#include "artg4tk/DataProducts/EventGenerators/GenParticleCollection.hh"

/* testing/debugging purposes */

#include "Geant4/G4GDMLParser.hh"
#include "Geant4/G4PhysicalVolumeStore.hh"
/* */

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
       
     void clear();

    // data members
    
    // Diagnostics level (verbosity)
    //
    int         fVerbosity;
    
    // G4 (or derived) things 
    //
    G4RunManager*            fRM;
    //
    // data members specifically to study parameters modification
    //
    bool                           fDefaultPhysics;

    // This one is needed since we'll have to insert 
    // model(s) config info as a RunProduct (in beginRun(...))
    //
    ArtG4tkModelConfig*      fModelConfig;
    //
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

   fRM = 0;
/*
   // Get the run manager and check if it's been initialized in any way
   // NOTE-1: attempt to call the ctor more than once will result
   //         in a fatal throw on the G4 side (in the ctor itself)
   // NOTE-2: attempt to call G4RunManager::GetRunManager() BEFORE
   //         it's initialized (via new) will result in return of
   //         a NULL pointer !
   //
   fRM              = new G4RunManager();
*/
   
   // CRITICAL !!!
   // Model(s) (or at least Bertini) config/params should be in BEFORE
   // Physics List is instantiated/initialized !!!
   //   
   //
   // TRACKED PSet(s) - outputs with different settings will NOT mix
   //
   fhicl::ParameterSet physicscfg = p.get<fhicl::ParameterSet>("HadronicModelParameters");
//   fDefaultPhysics = physicscfg.get<bool>("DefaultPhysics",true);
   fDefaultPhysics = physicscfg.get<bool>("DefaultPhysics"); // make it TRACKED !!!
   fModelConfig = new ArtG4tkModelConfig(fDefaultPhysics);
   if ( !fDefaultPhysics )
   {
      fModelConfig->Fill( physicscfg );
      art::ServiceHandle<PhysModelConfigService> physcfgservice;
      fhicl::ParameterSet modelcfg = physicscfg.get<fhicl::ParameterSet>("Bertini");
      if ( !modelcfg.is_empty() ) physcfgservice->ConfigureModel("Bertini",modelcfg);
      // modelcfg = physicscfg.get<fhicl::ParameterSet>("INCLXX");
      // if ( !modelcfg.is_empty() ) physcfgservice->ConfigureModel("INCLXX",modelcfg);
      // Etc.
   }

   //
   // NOTE(JVY): This is needed when doing geom via GDML
   //
   art::ServiceHandle<DetectorHolderService> detholder;
   detholder->initialize();
   detholder->constructAllLVs();
   detholder->callArtProduces(this);

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
        
   // Run product
   // NOTE: Default 2nd arg is art::InEvent
   //
   produces<ArtG4tkModelConfig,art::InRun>();
   
   // NOTE(JVY): This is consideter a "hit by sens.det" so 
   //            it has moved to GDMLDetectorService  
   //
// ->   produces<ArtG4tkVtx>();
   
}

artg4tk::ModelParamStudyProducer::~ModelParamStudyProducer()
{

   // no need to clean up UI pointer - I don't "new" it, I just "get it"

   if ( fCurrentG4Event ) delete fCurrentG4Event;
   
   if ( fRM ) 
   {
      fRM->RunTermination();
      delete fRM;
   }
   delete fModelConfig;
   
}

// At begin job
//
void artg4tk::ModelParamStudyProducer::beginJob()
{
   
   return;

}

// At begin run
//
void artg4tk::ModelParamStudyProducer::beginRun( art::Run& run )
{  
   
   // Get the run manager and check if it's been initialized in any way
   // NOTE-1: attempt to call the ctor more than once will result
   //         in a fatal throw on the G4 side (in the ctor itself)
   // NOTE-2: attempt to call G4RunManager::GetRunManager() BEFORE
   //         it's initialized (via new) will result in return of
   //         a NULL pointer !
   //
   fRM              = new G4RunManager();

   // Setup phys.list to Geant and initialize the list if necessary
   //
   art::ServiceHandle<PhysicsListHolderService> physicsListHolder;
   fRM->SetUserInitialization( physicsListHolder->makePhysicsList() );
   
   // Declare the detector construction to Geant
   //
   fRM->SetUserInitialization( new ArtG4DetectorConstruction() ); // this GDML-related - will retrieve from detholder, etc.

   fRM->GeometryHasBeenModified();

   // inits
   //
   fRM->Initialize();   
   fRM->ConfirmBeamOnCondition();
   fRM->ConstructScoringWorlds();
   fRM->RunInitialization(); // this is part of BeamOn 
                             // and needs be done (at least) to set GeomClosed status 
    
   // Last but not least:
   // Insert the RunProduct (model(s) config info)
   //
   std::unique_ptr<ArtG4tkModelConfig> pcfg(new ArtG4tkModelConfig(*fModelConfig));   
   run.put(std::move(pcfg));   

/* testing/debugging stuff - will be removed shortly

   G4GDMLParser parser;
   // --> not (yet) available in 4.10.1.p02 --> parser.SetRegionExport(true);
   parser.Write( "feedback-geom.gdml", 
                 G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume()->GetLogicalVolume(),
		 false ); // 3rd arg set to "false" means NOT to concatinate each volume name 
		          // with its logical address in hexadecimal format (D=true, and the Read is set to strip this extention)
*/
/*
   G4PhysicalVolumeStore* pvstore = G4PhysicalVolumeStore::GetInstance();
   G4VPhysicalVolume* tgt = pvstore->GetVolume("Target");
   if ( tgt )
   {
      const G4ThreeVector& trans = tgt->GetTranslation();
      std::cout << " x = " << trans.x() 
                << " y = " << trans.y()
		<< " z = " << trans.z() << std::endl;
   }
   else
   {
      std::cout << " Can NOT find phys.vol. Target " << std::endl;
   }
*/   
   
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
      G4PrimaryVertex*   g4vtx = new G4PrimaryVertex( i->position()*CLHEP::cm, 0. ); // 4th arg is time(ns)
      //
      const CLHEP::HepLorentzVector& mom = i->momentum();
    
/*
      //
      // FIXME !!! --> DONE !!!
      // This needs to be configurable via cfg/fcl !!!
      // For now, start the particle just inside the "subvolume"
      //
      G4LogicalVolume*     world = G4TransportationManager::GetTransportationManager()
                                 ->GetNavigatorForTracking()->GetWorldVolume()->GetLogicalVolume();
      G4VPhysicalVolume*   pdaughter = world->GetDaughter(0); // "first daughter" which is subworld
      G4VSolid*            solid = pdaughter->GetLogicalVolume()->GetSolid();    
      double z = solid->DistanceToOut( G4ThreeVector(0.,0.,0.), G4ThreeVector(0.,0.,-1.) ); // NOTE that dist is always positive
      //
      if ( mom.z() > 0. ) // if the z-momentum is positive (left-to-right), then start at the left edge
      {
         z *= -1.;
      }
      //
      CLHEP::Hep3Vector pos( 237.5, 0., (pdaughter->GetTranslation().z()+z*0.999) ); // z is in mm !!! and shift it by 0.1% inward !!!
                  
      //
      G4PrimaryVertex*  g4vtx = new G4PrimaryVertex( pos*CLHEP::mm, 0. ); // 4th arg is time(ns)
      //
      // specifically to LArIAT study !!!
      //
      // G4PrimaryVertex* g4vtx = new G4PrimaryVertex( CLHEP::Hep3Vector(23.75,0.,-40.54646)*CLHEP::cm, 0. );
      //
*/

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
   
   // This is "the beef" (in terms of Geant4)
   //
   G4EventManager::GetEventManager()->ProcessOneEvent( fCurrentG4Event );
   
   if ( fVerbosity > 0 )
   {
      fLogInfo << "G4Event " << e.id().event() << " has been processed by EventManager "; // << std::endl;
   } 
   fRM->AnalyzeEvent( fCurrentG4Event );
   // fRM->UpdateScoring(); // can NOT use it outside G4RunManager (or derived) since this method is protected

   // This is to put "Event product" into Art::Event
   //
   art::ServiceHandle<DetectorHolderService> detholder;
   detholder->setCurrArtEvent( e );
   detholder->fillEventWithArtHits( fCurrentG4Event->GetHCofThisEvent() );
   
   clear(); 
   
   return;
    
}

// At end run
//
void artg4tk::ModelParamStudyProducer::endRun( art::Run& )
{
   
   return;

}

void artg4tk::ModelParamStudyProducer::clear()
{
   
   // NOTE(JVY): in principle, one can call G4RunManager::TerminateOneEvent() method,
   //            where a G4Event gets deleted unless it's marked "toBeKept"...
   //            ... which is not the case/goal for this app
   //
   delete fCurrentG4Event;
   fCurrentG4Event = 0;

   return;

}

DEFINE_ART_MODULE(artg4tk::ModelParamStudyProducer)
