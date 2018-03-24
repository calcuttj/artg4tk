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
// CheckCalorimeterHits_module.cc: Analyzer module that demonstrates access to 
// Calorimeter hits 
// and makes some histograms
// 
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#include "artg4tk/Analysis/CheckCalorimeterHits_module.hh"
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

artg4tk::CheckCalorimeterHits::CheckCalorimeterHits(fhicl::ParameterSet const& p) :
art::EDAnalyzer(p),
_DumpGDML(p.get<bool>("DumpGDML")),
_hnHits(0),
_hEdep(0),
_haEdep(0),
_ntuple(0) {
}

void artg4tk::CheckCalorimeterHits::beginRun(const art::Run& thisRun) {

}

void artg4tk::CheckCalorimeterHits::beginJob() {
    art::ServiceHandle<art::TFileService> tfs;
    _hnHits = tfs->make<TH1F>("hnHits", "Number of CaloArtHits", 300, 0, 0);
    _hEdep = tfs->make<TH1F>("hEdep", "total Energy deposition in CaloArtHits", 2000, 0, 0);
    _haEdep = tfs->make<TH1F>("haEdep", "z of  Energy deposition in CaloArtHits", 200,-500.,500.);
    _ntuple = tfs->make<TNtuple>("ntuple","Demo ntuple",
			  "Event:Edep:em_Edep:nonem_Edep:xpos:ypos:zpos:time");
} // end beginJob

void artg4tk::CheckCalorimeterHits::analyze(const art::Event& event) {
    typedef std::vector< art::Handle<CalorimeterHitCollection> > HandleVector;
    HandleVector allSims;
    event.getManyByType(allSims);
    double sumE = 0.0;
    for (HandleVector::const_iterator i = allSims.begin(); i != allSims.end(); ++i) {
        const CalorimeterHitCollection & sims(**i);
        //cout << "CaloHit collection size:  " << sims.size() << endl;
        sumE = 0.0;
        _hnHits->Fill(sims.size());
        for (CalorimeterHitCollection::const_iterator j = sims.begin(); j != sims.end(); ++j) {
            const CalorimeterHit& hit = *j;
            sumE = sumE + hit.GetEdep();
	    _haEdep->Fill( hit.GetZpos(),hit.GetEdep());
	    _ntuple->Fill(event.event(),
			  hit.GetEdep(),
			  hit.GetEdepEM(),
			  hit.GetEdepnonEM(),
			  hit.GetXpos(),
			  hit.GetYpos(),
			  hit.GetZpos(),
			  hit.GetTime());
        }
        _hEdep->Fill(sumE / CLHEP::GeV);
    }
} // end analyze

void artg4tk::CheckCalorimeterHits::endJob() {

}// end endJob

using artg4tk::CheckCalorimeterHits;

DEFINE_ART_MODULE(CheckCalorimeterHits)
