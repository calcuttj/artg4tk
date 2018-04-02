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
// CheckSimEnergyDepositHit_module.cc: Analyzer module that demonstrates access to 
// Calorimeter hits 
// and makes some histograms
// 
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#include "artg4tk/Analysis/CheckSimEnergyDepositHit_module.hh"
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

artg4tk::CheckSimEnergyDepositHit::CheckSimEnergyDepositHit(fhicl::ParameterSet const& p) :
art::EDAnalyzer(p),
_hnHits(0),
_hEdep(0),
_haEdep(0),
_hnumPhotons(0),
_ntuple(0) {
}

void artg4tk::CheckSimEnergyDepositHit::beginRun(const art::Run& thisRun) {

}

void artg4tk::CheckSimEnergyDepositHit::beginJob() {
    art::ServiceHandle<art::TFileService> tfs;
    _hnHits = tfs->make<TH1F>("hnHits", "Number of SimEnergyDepositHits", 300, 0, 0);
    _hEdep = tfs->make<TH1F>("hEdep", "Energy deposition per SimEnergyDepositHits", 2000, 0, 0.1);
    _haEdep = tfs->make<TH1F>("haEdep", "z of  Energy deposition in SimEnergyDepositHits", 200,-500.,500.);
    _hnumPhotons = tfs->make<TH1F>("hnumPhotons", "number of photons per  SimEnergyDepositHit", 200,0.,500.);
    _ntuple = tfs->make<TNtuple>("ntuple","Demo ntuple",
			  "Event:Edep:em_Edep:nonem_Edep:xpos:ypos:zpos:time");

} // end beginJob

void artg4tk::CheckSimEnergyDepositHit::analyze(const art::Event& event) {
    typedef std::vector< art::Handle<SimEnergyDepositHitCollection> > HandleVector;
    HandleVector allSims;
    event.getManyByType(allSims);
    //    double sumE = 0.0;
    for (HandleVector::const_iterator i = allSims.begin(); i != allSims.end(); ++i) {
        const SimEnergyDepositHitCollection & sims(**i);
       cout << " SimEnergyDepositHit collection size:  " << sims.size() << endl;
       // sumE = 0.0;
        _hnHits->Fill(sims.size());

	for (SimEnergyDepositHitCollection::const_iterator j = sims.begin(); j != sims.end(); ++j) {
	  const SimEnergyDepositHit& hit = *j;
	  _hEdep->Fill(hit.Energy());
	  std::cout<<"Energy:  "<< hit.Energy()<<std::endl;
	  _haEdep->Fill( hit.EndZ(),hit.Energy());
	  _hnumPhotons->Fill( hit.NumPhotons());
	  /*
	    _ntuple->Fill(event.event(),
			  hit.GetEdep(),
			  hit.GetEdepEM(),
			  hit.GetEdepnonEM(),
			  hit.GetXpos(),
			  hit.GetYpos(),
			  hit.GetZpos(),
			  hit.GetTime());
	  */
        }
	// _hEdep->Fill(sumE / CLHEP::GeV);
	
    }
} // end analyze

void artg4tk::CheckSimEnergyDepositHit::endJob() {

}// end endJob

using artg4tk::CheckSimEnergyDepositHit;

DEFINE_ART_MODULE(CheckSimEnergyDepositHit)
