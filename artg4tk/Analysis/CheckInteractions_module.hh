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
// CheckInteractions_module.hh: Analyzer module that demonstrates access to hits 
// and makes some histograms
// 
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef CHECKHITS_MODULE_HH
#define	CHECKHITS_MODULE_HH
//
// C++ includes.
#include <iostream>
#include <string>
#include <set>
#include <cmath>
#include <algorithm>
// art Framework includes.
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art_root_io/TFileService.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Run.h"
#include "art_root_io/TFileDirectory.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Principal/Provenance.h"
// artg4tk includes:
#include "artg4tk/services/DetectorHolder_service.hh"
#include "artg4tk/pluginDetectors/gdml/myInteractionArtHitData.hh"
// Root includes.
#include "TFile.h"
#include "TH1F.h"
#include "TDirectory.h"
// Other includes.
#include "CLHEP/Units/SystemOfUnits.h"
#include<vector>
using namespace std;
namespace artg4tk {
    class CheckInteractions;
}

class artg4tk::CheckInteractions : public art::EDAnalyzer {
public:

    explicit CheckInteractions(fhicl::ParameterSet const& p);
    virtual void beginJob();
    virtual void beginRun(const art::Run& Run);
    virtual void endJob();
    virtual void analyze(const art::Event& event);

private:
    int fNThetaBinsFW;
    double fThetaMinFW;
    double fDeltaThetaFW;
    int fNThetaBinsLA;
    double fThetaMinLA;
    double fDeltaThetaLA;
    //
    TH1F* _fHistoNSec; // number of secondaries
    TH1F* _hEdep; // total energy deposition in CaloHits
    TH1F* _hnDRHits; // number of DRCaloHits
    TH1F* _hDREdep; // total energy deposition in DRCaloHits
    TH1F* _hNCeren; // total number of Cerenkovphotons in DRCaloHits
    std::vector<TH1D*> fHistoSecPiMinusFW;
};

#endif	/* CHECKHITS_MODULE_HH */

