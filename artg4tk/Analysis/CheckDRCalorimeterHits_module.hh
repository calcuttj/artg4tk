//
//               __        __ __  __  __  
//   ____ ______/ /_____ _/ // / / /_/ /__
//  / __ `/ ___/ __/ __ `/ // /_/ __/ //_/
// / /_/ / /  / /_/ /_/ /__  __/ /_/ ,<   
// \__,_/_/   \__/\__, /  /_/  \__/_/|_|  
//               /____/                  
//
// artg4tk: art based Geant 4 Toolkit
// 
//=============================================================================
// CheckDRCalorimeterHits_module.hh: Analyzer module that demonstrates access to hits 
// and makes some histograms
// 
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef CHECKDRCALORIMETERHITS_MODULE_HH
#define	CHECKDRCALORIMETERHITS_MODULE_HH
// art Framework includes.
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Services/Optional/TFileDirectory.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Principal/Provenance.h"
// artg4 includes:
#include "artg4tk/services/DetectorHolder_service.hh"
// artg4tk includes:
#include "artg4tk/pluginDetectors/gdml/CalorimeterHit.hh"
#include "artg4tk/pluginDetectors/gdml/myDRCaloArtHitData.hh"
#include "artg4tk/pluginDetectors/gdml/myParticleEContribArtData.hh"
// Root includes.
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TNtuple.h"
#include "TDirectory.h"
// Other includes.
#include "CLHEP/Units/SystemOfUnits.h"
//#include <string>
using namespace std;
namespace artg4tk {
    class CheckDRCalorimeterHits;
}

class artg4tk::CheckDRCalorimeterHits : public art::EDAnalyzer {
public:

    explicit CheckDRCalorimeterHits(fhicl::ParameterSet const& p);
    virtual void beginJob();
    virtual void beginRun(const art::Run& Run);
    virtual void endJob();
    virtual void analyze(const art::Event& event);

private:

    TH1F* _hnDRHits;// number of DRCaloHits
    TH1F* _hDREdep;// total energy deposition in DRCaloHits
    TH1F* _hNCeren;// total number of Cerenkovphotons in DRCaloHits
    TH2F* _hEdepvsproton;
    TNtuple* _ntuple;
};

#endif	/* CHECKDRCALORIMETERHITS_MODULE_HH */

