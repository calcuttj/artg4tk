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
// CheckPhotonHits_module.cc: Analyzer module that demonstrates access to 
// Photon hits and makes some histograms
// 
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#include "artg4tk/Analysis/CheckPhotonHits_module.hh"
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
//#include "artg4tk/pluginActions/writeGdml/gdmlText.hh"

artg4tk::CheckPhotonHits::CheckPhotonHits(fhicl::ParameterSet const& p) :
art::EDAnalyzer(p),
_DumpGDML(p.get<bool>("DumpGDML")),
_hnHits(0),
_hEdep(0),
_hxydet0(0),
_hxydet1(0) {
}

void artg4tk::CheckPhotonHits::beginRun(const art::Run& thisRun) {
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

void artg4tk::CheckPhotonHits::beginJob() {

    art::ServiceHandle<art::TFileService> tfs;
    _hnHits = tfs->make<TH1F>("hnHits", "Number of PhotonArtHits", 100, 0., 200.);
    _hEdep = tfs->make<TH1F>("hEdep", "energy of optical photon at detector", 100, 0., 10.);
    _hxydet0 = tfs->make<TH2F>("hxydet0", "photon position on det 0", 100, -25, 25, 100, -25, 25);
    _hxydet1 = tfs->make<TH2F>("hxydet1", "photon position on det 1", 100, -25, 25, 100, -25, 25);
} // end beginJob

void artg4tk::CheckPhotonHits::analyze(const art::Event& event) {
    typedef std::vector< art::Handle<PhotonHitCollection> > HandleVector;
    HandleVector allSims;
    event.getManyByType(allSims);
    cout << "CheckPhotonHits Event:  " << event.event() << "  Nr of PhotonHit collections: " << allSims.size() << endl;
    for (HandleVector::const_iterator i = allSims.begin(); i != allSims.end(); ++i) {
        const PhotonHitCollection & sims(**i);
        cout << "PhotonHit collection size:  " << sims.size() << endl;
        _hnHits->Fill(sims.size());
        for (PhotonHitCollection::const_iterator j = sims.begin(); j != sims.end(); ++j) {
            const PhotonHit& hit = *j;
            _hEdep->Fill(hit.edep*1000000000 );
	    std::cout << "x: "<<hit.xpos<<"  Y: "<< hit.ypos<< " z: "<<  hit.zpos<<std::endl;
            if (hit.zpos > 0.) {
                _hxydet0->Fill(hit.xpos , hit.ypos );
            } else {
                _hxydet1->Fill(hit.xpos , hit.ypos);
            }
        }
    }

} // end analyze

void artg4tk::CheckPhotonHits::endJob() {
    cout << " ********************************CheckPhotonHits: I am done " << endl;
}// end endJob

using artg4tk::CheckPhotonHits;

DEFINE_ART_MODULE(CheckPhotonHits)
