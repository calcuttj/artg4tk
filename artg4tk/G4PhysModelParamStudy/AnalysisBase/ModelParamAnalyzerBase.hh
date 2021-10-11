#ifndef artg4tk_MPAnalyzerBase_H
#define artg4tk_MPAnalyzerBase_H 1

#include "artg4tk/G4PhysModelParamStudy/AnalysisBase/AnalyzerWithExpDataBase.hh"

// Run/Event data products
//
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkParticle.hh"   // Event data product
#include "artg4tk/DataProducts/G4ModelConfig/ArtG4tkModelConfig.hh" // Run data product

// Root-specific headers
//
#include "TDirectory.h"
#include "TFile.h"
#include "TObjArray.h"
#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/BeamThinTargetConfig.hh"

namespace artg4tk {

  class ModelParamAnalyzerBase : public artg4tk::AnalyzerWithExpDataBase {

  public:
    explicit ModelParamAnalyzerBase(const fhicl::ParameterSet&);
    virtual ~ModelParamAnalyzerBase();

    virtual void beginRun(const art::Run&) override;
    virtual void endJob() override;

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

#endif
