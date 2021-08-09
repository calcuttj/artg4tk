// Declarations for the @PhysicsListHolder@ service

// Hang on to a physics list to be given to Geant

// Note that this is a little different than the other holder services.
// Instead of "forwarding" member functons from the Geant class, we need
// to initialize Geant with the *real* physics list class (because it will
// change things internally). 

// Include guard
#ifndef PHYSICSLIST_HOLDER_SERVICE_HH
#define PHYSICSLIST_HOLDER_SERVICE_HH

// Includes
#include "fhiclcpp/fwd.h"
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"

class G4VUserPhysicsList;

// Everything for the Art G4 simulation goes in the @artg4tk@ namespace
namespace artg4tk {
  
  class PhysicsListServiceBase;
  
  class PhysicsListHolderService {
  public:
  
    
    // Constructor for Physics List holder
    PhysicsListHolderService(fhicl::ParameterSet const&) :
      physicsListService_()
    {}
    
    // Destructor - don't do anything
    virtual ~PhysicsListHolderService() {}
    
    // This registers the passed detector with the service.
    void registerPhysicsListService( PhysicsListServiceBase* );
    
    // Get Physics list
    G4VUserPhysicsList* makePhysicsList() const;
    
    // Initialize the physics list if necessary
    void initializePhysicsList() const;

  private:
    
    PhysicsListServiceBase* physicsListService_;
    
  }; // class PhysicsListHolderService
  
}// end namespace artg4tk

//using art::LEGACY;
using artg4tk::PhysicsListHolderService;
DECLARE_ART_SERVICE(PhysicsListHolderService, LEGACY)
  
#endif // PHYSICSLIST_HOLDER_SERVICE
