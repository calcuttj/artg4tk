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
//#include "artg4tk/pluginActions/writeGdml/gdmlText.hh"
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
  /*
    if (_DumpGDML) {
        std::cout << "******************************Run: " << thisRun.id() << ": looking at Run Header" << std::endl;
        art::Handle<artg4::GdmlText> gdmlTextHandle;
        thisRun.getByLabel("artg4tkMain", "writeGdml", gdmlTextHandle);
        std::cout << "gdmlTextHandle:  " << gdmlTextHandle.isValid() << std::endl;
        if (gdmlTextHandle.isValid()) {
            artg4::GdmlText txt = *gdmlTextHandle;
            const std::string& stringy = txt.getData();
            art::Provenance const& prov = *gdmlTextHandle.provenance();
            cout << "\nProvenance information for product name: " << prov.branchName() << endl;
            cout << "Creator module label: " << prov.moduleLabel() << endl;
            std::cout << stringy << std::endl;
        }
    }
  */
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
    typedef std::vector< art::Handle<myCaloArtHitDataCollection> > HandleVector;
    HandleVector allSims;
    event.getManyByType(allSims);
        double sumE = 0.0;
//    cout << "CheckCalorimeterHits Event:  " << event.event() << "  Nr of CaloHit collections: " << allSims.size() << endl;
    for (HandleVector::const_iterator i = allSims.begin(); i != allSims.end(); ++i) {
        const myCaloArtHitDataCollection & sims(**i);
        //cout << "CaloHit collection size:  " << sims.size() << endl;
        sumE = 0.0;
        _hnHits->Fill(sims.size());
        for (myCaloArtHitDataCollection::const_iterator j = sims.begin(); j != sims.end(); ++j) {
            const myCaloArtHitData& hit = *j;
            sumE = sumE + hit.Edep;
	    _haEdep->Fill( hit.zpos,hit.Edep);
	    _ntuple->Fill(event.event(),
			  hit.Edep,
			  hit.em_Edep,
			  hit.nonem_Edep,
			  hit.xpos,
			  hit.ypos,
			  hit.zpos,
			  hit.time);
        }
        _hEdep->Fill(sumE / GeV);
    }
} // end analyze

void artg4tk::CheckCalorimeterHits::endJob() {

}// end endJob

using artg4tk::CheckCalorimeterHits;

DEFINE_ART_MODULE(CheckCalorimeterHits)
