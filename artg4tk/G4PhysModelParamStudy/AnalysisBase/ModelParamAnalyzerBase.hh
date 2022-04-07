#ifndef artg4tk_G4PhysModelParamStudy_AnalysisBase_ModelParamAnalyzerBase_hh
#define artg4tk_G4PhysModelParamStudy_AnalysisBase_ModelParamAnalyzerBase_hh

#include "artg4tk/G4PhysModelParamStudy/AnalysisBase/AnalyzerWithExpDataBase.hh"
#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/BeamThinTargetConfig.hh"

#include <string>

class TObjArray;

namespace artg4tk {

  class ArtG4tkParticle;
  
  class ModelParamAnalyzerBase : public artg4tk::AnalyzerWithExpDataBase {

  public:
    explicit ModelParamAnalyzerBase(const fhicl::ParameterSet&);
    virtual ~ModelParamAnalyzerBase();

    void beginRun(const art::Run&) override;
    void endJob() override;

  protected:
    void prepareG4PTable();
    void initXSecOnTarget(const std::string&, const ArtG4tkParticle&);
    bool ensureBeamTgtConfig(const art::Event&);

    // These two data members below can go into a base class for
    // this group of analysis modules
    //
    std::string fProdLabel;
    TObjArray* fModelConfig;

    // NOTE: Will also need XSec on TARGET !!!!!
    //
    double fXSecOnTarget;
    bool fXSecInit;

    // For cross-checks & bookkeeping on beamid-mom-target
    //
    BeamThinTargetConfig fBTConf;
    bool fConsistentRunConditions;
  };

}

#endif /* artg4tk_G4PhysModelParamStudy_AnalysisBase_ModelParamAnalyzerBase_hh */
