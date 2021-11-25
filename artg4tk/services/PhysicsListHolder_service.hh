// Declarations for the @PhysicsListHolder@ service

// Hang on to a physics list to be given to Geant

// Note that this is a little different than the other holder services.
// Instead of "forwarding" member functons from the Geant class, we need
// to initialize Geant with the *real* physics list class (because it will
// change things internally).

#ifndef artg4tk_services_PhysicsListHolder_service_hh
#define artg4tk_services_PhysicsListHolder_service_hh

// Includes
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
#include "fhiclcpp/fwd.h"

class G4VUserPhysicsList;

// Everything for the Art G4 simulation goes in the @artg4tk@ namespace
namespace artg4tk {

  class PhysicsListServiceBase;

  class PhysicsListHolderService {
  public:
    // Constructor for Physics List holder
    PhysicsListHolderService(fhicl::ParameterSet const&) {}

    // This registers the passed detector with the service.
    void registerPhysicsListService(PhysicsListServiceBase*);

    // Get Physics list
    G4VUserPhysicsList* makePhysicsList() const;

    // Initialize the physics list if necessary
    void initializePhysicsList() const;

  private:
    PhysicsListServiceBase* physicsListService_{nullptr};

  }; // class PhysicsListHolderService

} // end namespace artg4tk

DECLARE_ART_SERVICE(artg4tk::PhysicsListHolderService, LEGACY)

#endif /* artg4tk_services_PhysicsListHolder_service_hh */
