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
// CheckGenParticle_module.hh: Analysis module to analyze the GenParticles 
// in the Event 
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef CHECKGENPARTICLE_MODULE_HH
#define	CHECKGENPARTICLE_MODULE_HH
//
// C++ includes.
#include <iostream>
#include <string>
#include <set>
#include <cmath>
#include <algorithm>
// Framework includes.
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Services/Optional/TFileDirectory.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Principal/Provenance.h"
// artg4tk includes:
#include "artg4/services/DetectorHolder_service.hh"
#include "artg4tk/DataProducts/GenParticle.hh"
#include "artg4tk/DataProducts/GenParticleCollection.hh"
// Root includes.
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TDirectory.h"
// Other includes.
#include "CLHEP/Units/SystemOfUnits.h"

using namespace std;
namespace artg4tk {
    class CheckGenParticle;
}

class artg4tk::CheckGenParticle : public art::EDAnalyzer {
public:

 explicit CheckGenParticle(fhicl::ParameterSet const& p);
    virtual void beginJob();
    virtual void beginRun(const art::Run& Run);
    virtual void endJob();
    virtual void analyze(const art::Event& event);

private:
    std::string _myName;
    TH1F* _hnParts;
    TDirectory const * _directory;
    TFile * _file;

};

#endif	/* CHECKGENPARTICLE_MODULE_HH */

