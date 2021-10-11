// Here we define the physics list

#ifndef artg4tk_pluginActions_physicsList_PhysicsList_service_hh
#define artg4tk_pluginActions_physicsList_PhysicsList_service_hh

#include "fhiclcpp/fwd.h"

#include "artg4tk/services/PhysicsListServiceBase.hh"

class G4VUserPhysicsList;

namespace artg4tk {

  class PhysicsListService : public artg4tk::PhysicsListServiceBase {

  public:
    PhysicsListService(fhicl::ParameterSet const&);
    virtual ~PhysicsListService(){};
    G4VUserPhysicsList* makePhysicsList() override;

  private:
    std::string PhysicsListName_;
    bool DumpList_;           // enable/disable dumping of physics list
    bool enableNeutronLimit_; // enable/register neutron time limit physics constructor
    double NeutronTimeLimit_;
    double NeutronKinELimit_;
    bool enableStepLimit_; // enable/register step limit physics construct
                           // Note the that the limit is a material property
                           // the values is set in the gdml file

    //-----------------------------------------------------------------------------------
    bool enableOptical_; // enable/register optical physics construct
    // the following variables are ignored in case optical is false:
    bool enableCerenkov_;       // enable/disable Cerenkov process
    bool CerenkovStackPhotons_; // enable/disable stacking of Cerenkov  photons
    int CerenkovMaxNumPhotons_;
    double CerenkovMaxBetaChange_;
    bool CerenkovTrackSecondariesFirst_;
    bool enableScintillation_;         // enable/disable Scintillation process
    bool ScintillationStackPhotons_;   // enable/disable stacking of scintillations photons
    bool ScintillationByParticleType_; // enable/disable calculation of scintillation photons by
                                       // particle type
    bool ScintillationTrackInfo_;      // Activate/Inactivate scintillation track info
    bool ScintillationTrackSecondariesFirst_; // the option to track secondaries before finishing
                                              // their parent track
    bool enableAbsorption_;                   // enable/disable Absorption process
    bool enableRayleigh_;                     // enable/disable Rayleigh scattering process
    bool enableMieHG_;                        // enable/disable Mie scattering process
    bool enableBoundary_;                     // enable/disable Boundary process
    bool enableWLS_;                          // enable/disable Wave Length Shifting process
    bool BoundaryInvokeSD_;
    int verbositylevel_;     // set the verbosity level of optical processes
    std::string WLSProfile_; // Set the WLS time profile (delta or exponential)
  };

}

DECLARE_ART_SERVICE(artg4tk::PhysicsListService, LEGACY)

#endif /* artg4tk_pluginActions_physicsList_PhysicsList_service_hh */
