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
// CheckTrackerHits_module.cc: Analyzer module that demonstrates access to 
// Calorimeter hits 
// and makes some histograms
// 
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#include "artg4tk/Analysis/CheckTrackerHits_module.hh"
#include "artg4/pluginActions/writeGdml/gdmlText.hh"
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include <math.h>  
artg4tk::CheckTrackerHits::CheckTrackerHits(fhicl::ParameterSet const& p) :
art::EDAnalyzer(p),
_hnHits(0),
_hEdep(0),
_hrEdep(),
_hzEdep(0)
{
}

void artg4tk::CheckTrackerHits::beginRun(const art::Run& thisRun) {
  std::cout << "******************************Run: " << thisRun.id() <<std::endl;

}

void artg4tk::CheckTrackerHits::beginJob() {

    art::ServiceHandle<art::TFileService> tfs;
    _hnHits = tfs->make<TH1F>("hnHits", "Number of TrackerArtHits", 200, 0., 10000.);
    _hEdep = tfs->make<TH1F>("hEdep", "total Energy deposition in TrackerArtHits", 200, 0., 1.2);
    _hrEdep = tfs->make<TH1F>("hrEdep", "radius of total Energy deposition in TrackerArtHits", 300, 0., 300.);
    _hzEdep = tfs->make<TH1F>("hzEdep", "total Energy deposition in TrackerArtHits", 600, 0., 600.);
} // end beginJob

void artg4tk::CheckTrackerHits::analyze(const art::Event& event) {
    typedef std::vector< art::Handle<myTrackerArtHitDataCollection> > HandleVector;
    HandleVector allSims;
    event.getManyByType(allSims);

//    cout << "CheckTrackerHits Event:  " << event.event() << "  Nr of CaloHit collections: " << allSims.size() << endl;
    for (HandleVector::const_iterator i = allSims.begin(); i != allSims.end(); ++i) {
        const myTrackerArtHitDataCollection & sims(**i);
        //cout << "CaloHit collection size:  " << sims.size() << endl;
        double sumE = 0.0;
        _hnHits->Fill(sims.size());
        for (myTrackerArtHitDataCollection::const_iterator j = sims.begin(); j != sims.end(); ++j) {
            const myTrackerArtHitData& hit = *j;
	    double radius= 0.1*sqrt(hit.xpos*hit.xpos + hit.ypos*hit.ypos);
	    _hrEdep ->Fill(radius);
	    _hzEdep ->Fill(0.1*hit.zpos+300.,hit.Edep);
            sumE = sumE + hit.Edep;
        }
        _hEdep->Fill(sumE / GeV);
    }


} // end analyze

void artg4tk::CheckTrackerHits::endJob() {

}// end endJob

using artg4tk::CheckTrackerHits;

DEFINE_ART_MODULE(CheckTrackerHits)
