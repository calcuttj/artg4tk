// Here we define the physics list

#ifndef PHYSICSLIST_SERVICE_HH
#define PHYSICSLIST_SERVICE_HH

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "artg4tk/services/PhysicsListServiceBase.hh"

class G4VUserPhysicsList;

namespace artg4tk {
  
  //  class N02PhysicsList;

  class PhysicsListService : public artg4tk::PhysicsListServiceBase {
    
  public:
    PhysicsListService(fhicl::ParameterSet const &, art::ActivityRegistry &);
    virtual ~PhysicsListService() {};
    virtual G4VUserPhysicsList* makePhysicsList() override;  
  private:
    std::string PhysicsListName_;    // name of reference Physics list
                                     // allowed names are:  
                                     //  CHIPS
                                     //  FTFP_BERT
                                     //  FTFP_BERT_TRV
                                     //  FTFP_BERT_HP
                                     //  FTF_BIC 
                                     //  LBE
                                     //  LHEP
                                     //  QBBC
                                     //  QGSC_BERT
                                     //  QGSP
                                     //  QGSP_BERT
                                     //  QGSP_BERT_CHIPS
                                     //  QGSP_BERT_HP
                                     //  QGSP_BIC
                                     //  QGSP_BIC_HP
                                     //  QGSP_FTFP_BERT
                                     //  QGS_BIC
                                     //  QGSP_INCLXX
                                     //  Shielding
                                     //------------------
    bool DumpList_;            // enable/disable dumping of physics list
    bool enableCerenkov_;      // enable/disable Cerenkov process
    bool enableScintillation_; // enable/disable Scintillation process
    bool enableAbsorption_;    // enable/disable Absorption process 
    bool enableRayleigh_;      // enable/disable Rayleigh scattering process
    bool enableMieHG_;         // enable/disable Mie scattering process
    bool enableBoundary_;      // enable/disable Boundary process
    bool enableWLS_;           // enable/disable Wave Length Shifting process
  };
  
}

using artg4tk::PhysicsListService;
DECLARE_ART_SERVICE(PhysicsListService,LEGACY)

#endif
