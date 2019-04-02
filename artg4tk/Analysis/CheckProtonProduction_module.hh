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
// CheckProtonProduction_module.hh: Analyzer module that demonstrates access to hits 
// and makes some histograms
// 
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef CHECKPROTONPRODUCTION_MODULE_HH
#define	CHECKPROTONPRODUCTION_MODULE_HH
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
    class CheckProtonProduction;
}

class artg4tk::CheckProtonProduction : public art::EDAnalyzer {
public:

    explicit CheckProtonProduction(fhicl::ParameterSet const& p);
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
  //
    TH1F* _fHistoSecPMom; // momentum of secondary protons   
    TH1F* _fHistoSecPkinE54; // kinetic Energy of secondary protons   
    TH1F* _fHistoSecPkinE68;  // kinetic Energy of secondary protons   
    TH1F* _fHistoSecPkinE90;  // kinetic Energy of secondary protons   
    TH1F* _fHistoSecPkinE121; // kinetic Energy of secondary protons   
    TH1F* _fHistoSecPkinE164; // kinetic Energy of secondary protons   
    TH1F* _fHistoSecPkinE;   // kinetic Energy of secondary protons 
    TH1F* _fHistoSecPTheta;  // theta of secondary protons 
  //
    TH1F* _fHistoSecDMom;    // momentum of secondary deuterons   
    TH1F* _fHistoSecDkinE54; // kinetic Energy of secondary deuterons   
    TH1F* _fHistoSecDkinE68;  // kinetic Energy of secondary deuterons   
    TH1F* _fHistoSecDkinE90;  // kinetic Energy of secondary deuterons   
    TH1F* _fHistoSecDkinE121; // kinetic Energy of secondary deuterons   
    TH1F* _fHistoSecDkinE164; // kinetic Energy of secondary deuterons   
    TH1F* _fHistoSecDkinE;   // kinetic Energy of secondary deuterons 
    TH1F* _fHistoSecDTheta;  // theta of secondary deuterons 
  //
    TH1F* _fHistoSecTMom;    // momentum of secondary tritons   
    TH1F* _fHistoSecTkinE54; // kinetic Energy of secondary tritons   
    TH1F* _fHistoSecTkinE68;  // kinetic Energy of secondary tritons   
    TH1F* _fHistoSecTkinE90;  // kinetic Energy of secondary tritons   
    TH1F* _fHistoSecTkinE121; // kinetic Energy of secondary tritons   
    TH1F* _fHistoSecTkinE164; // kinetic Energy of secondary tritons   
    TH1F* _fHistoSecTkinE;   // kinetic Energy of secondary tritons 
    TH1F* _fHistoSecTTheta;  // theta of secondary tritons
    std::vector<TH1D*> fHistoSecProtonFW;
};

#endif	/* CHECKPROTONPRODUCTION_MODULE_HH */

