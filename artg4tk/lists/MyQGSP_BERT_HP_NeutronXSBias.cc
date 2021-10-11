/////////////////////////////////////////////////////////////////////////////
// The following change is the _only_ required changed to move from
// the non-extensible factory to the exensible factory.  All other changes
// relative to the "factory" example are there to demonstrate new features.
/////////////////////////////////////////////////////////////////////////////
// non-extensible:  #include "G4PhysListFactory.hh"
#include "Geant4/G4PhysListFactoryAlt.hh"
// use this for drop-in replacement:  using namespace g4alt;
// using namespace g4alt;

/////////////////////////////////////////////////////////////////////////////
// headers needed to demonstrate new features
/////////////////////////////////////////////////////////////////////////////

// allow ourselves to extend the short names for physics ctor addition/replace
// along the same lines as EMX, EMY, etc
#include "Geant4/G4PhysListRegistry.hh"

// allow ourselves to give the user extra info about available physics ctors
#include "Geant4/G4PhysicsConstructorFactory.hh"

/////////////////////////////////////////////////////////////////////////////
// pull in a user defined physics list definition into the main program
// and register it with the factory (doesn't have to be the main program
// but the .o containing the declaration _must_ get linked/loaded)

#include "Geant4/G4VModularPhysicsList.hh"

#include "Geant4/G4PhysListStamper.hh" // defines macro for factory registration
#include "MyQGSP_BERT_HP_NeutronXSBias.hh"

///#define G4CASCADE_DEBUG_INTERFACE 1

G4_DECLARE_PHYSLIST_FACTORY(MyQGSP_BERT_HP_NeutronXSBias);
