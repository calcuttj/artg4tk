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
// CheckHits_module.cc: Analyzer module that demonstrates access to 
// Calorimeter hits 
// and makes some histograms
// 
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#include "artg4tk/Analysis/CheckHits_module.hh"
#include "artg4/pluginActions/writeGdml/gdmlText.hh"
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

artg4tk::CheckHits::CheckHits(fhicl::ParameterSet const& p) :
art::EDAnalyzer(p),
_DumpGDML(p.get<bool>("DumpGDML")),
_hnHits(0),
_hEdep(0),
_hnDRHits(0),
_hDREdep(0),
_hNCeren(0) {
}

void artg4tk::CheckHits::beginRun(const art::Run& thisRun) {
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
}

void artg4tk::CheckHits::beginJob() {

    art::ServiceHandle<art::TFileService> tfs;
    _hnHits = tfs->make<TH1F>("hnHits", "Number of CaloArtHits", 100, 0., 20000.);
    _hEdep = tfs->make<TH1F>("hEdep", "total Energy deposition in CaloArtHits", 100, 0., 15.);
    _hnDRHits = tfs->make<TH1F>("hnDRHits", "Number of DRCaloArtHits", 100, 0., 20000.);
    _hDREdep = tfs->make<TH1F>("hDREdep", "total Energy deposition in DRCaloArtHits", 100, 0., 11.);
    _hNCeren = tfs->make<TH1F>("hNCeren", "total number of Cerenkov Photons in DRCaloArtHits", 100, 0., 1000000.);
} // end beginJob

void artg4tk::CheckHits::analyze(const art::Event& event) {
    typedef std::vector< art::Handle<myCaloArtHitDataCollection> > HandleVector;
    HandleVector allSims;
    event.getManyByType(allSims);

    cout << "CheckHits Event:  " << event.event() << "  Nr of CaloHit collections: " << allSims.size() << endl;
    for (HandleVector::const_iterator i = allSims.begin(); i != allSims.end(); ++i) {
        const myCaloArtHitDataCollection & sims(**i);
        cout << "CaloHit collection size:  " << sims.size() << endl;
        double sumE = 0.0;
        _hnHits->Fill(sims.size());
        for (myCaloArtHitDataCollection::const_iterator j = sims.begin(); j != sims.end(); ++j) {
            const myCaloArtHitData& hit = *j;
            sumE = sumE + hit.Edep;
        }
        _hEdep->Fill(sumE/GeV);
    }
    typedef std::vector< art::Handle<myDRCaloArtHitDataCollection> > DRHandleVector;
    DRHandleVector allDRSims;
    event.getManyByType(allDRSims);
    cout << "Event:  " << event.event() << "  Nr of DRCaloHit collections: " << allDRSims.size() << endl;
    for (DRHandleVector::const_iterator i = allDRSims.begin(); i != allDRSims.end(); ++i) {
        const myDRCaloArtHitDataCollection & DRsims(**i);
        cout << "DRCaloHit collection size:  " << DRsims.size() << endl;
        double sumDRE = 0.0;
        double sumNCeren = 0.0;
        _hnDRHits->Fill(DRsims.size());
        for (myDRCaloArtHitDataCollection::const_iterator j = DRsims.begin(); j != DRsims.end(); ++j) {
            const myDRCaloArtHitData& hit = *j;
            sumDRE = sumDRE + hit.Edep;
            sumNCeren = sumNCeren + hit.Nceren;
        }
        _hDREdep->Fill(sumDRE/GeV);
        _hNCeren->Fill(sumNCeren);
    }
} // end analyze

void artg4tk::CheckHits::endJob() {
    cout << " ********************************CheckHits: I am done " << endl;
}// end endJob

using artg4tk::CheckHits;

DEFINE_ART_MODULE(CheckHits)
