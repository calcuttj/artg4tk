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
// CheckDRCalorimeterHits_module.cc: Analyzer module that demonstrates access to 
// DRCalorimeter hits and makes some histograms
// 
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#include "artg4tk/Analysis/CheckDRCalorimeterHits_module.hh"
#include "artg4tk/pluginActions/writeGdml/gdmlText.hh"
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

artg4tk::CheckDRCalorimeterHits::CheckDRCalorimeterHits(fhicl::ParameterSet const& p) :
art::EDAnalyzer(p),
_hnDRHits(0),
_hDREdep(0),
_hNCeren(0),
_hEdepvsproton(0),
_ntuple(0) {
}

void artg4tk::CheckDRCalorimeterHits::beginRun(const art::Run& thisRun) {
         std::cout << "******************************Run: " << thisRun.id() << ": looking at Run Header" << std::endl;
}

void artg4tk::CheckDRCalorimeterHits::beginJob() {
    art::ServiceHandle<art::TFileService> tfs;
    _hnDRHits = tfs->make<TH1F>("hnDRHits", "Number of DRCaloArtHits", 100, 0., 20000.);
    _hDREdep = tfs->make<TH1F>("hDREdep", "total Energy deposition in DRCaloArtHits", 100, 0., 11.);
    _hNCeren = tfs->make<TH1F>("hNCeren", "total number of Cerenkov Photons in DRCaloArtHits", 100, 0., 1000000.);
    _hEdepvsproton = tfs->make<TH2F>("hEdepvsproton", "Etot vs proton percentage", 100, 0., 60.,100,0.,11.);
   _ntuple = tfs->make<TNtuple>("ntuple","proton percentage",
			  "EVIS:PPERC");
} // end beginJob

void artg4tk::CheckDRCalorimeterHits::analyze(const art::Event& event) {
    typedef std::vector< art::Handle<myDRCaloArtHitDataCollection> > DRHandleVector;
    //    string *proton="proton";
    //string *ETot="ETot";
    DRHandleVector allDRSims;
    double sumDRE=0;
    event.getManyByType(allDRSims);
    cout << "Event:  " << event.event() << "  Nr of DRCaloHit collections: " << allDRSims.size() << endl;
    for (DRHandleVector::const_iterator i = allDRSims.begin(); i != allDRSims.end(); ++i) {
        const myDRCaloArtHitDataCollection & DRsims(**i);
        sumDRE = 0.0;
        double sumNCeren = 0.0;
        _hnDRHits->Fill(DRsims.size());
        for (myDRCaloArtHitDataCollection::const_iterator j = DRsims.begin(); j != DRsims.end(); ++j) {
            const myDRCaloArtHitData& hit = *j;
            sumDRE = sumDRE + hit.Edep;
            sumNCeren = sumNCeren + hit.Nceren;
        }
        _hDREdep->Fill(sumDRE / GeV);
        _hNCeren->Fill(sumNCeren);
    }
    typedef std::vector< art::Handle<myParticleEContribArtData> > EdepHandleVector;
    EdepHandleVector allEdeps;
    event.getManyByType(allEdeps);
       cout << "number of Edep collections size:  " << allEdeps.size() << endl;
    for (EdepHandleVector::const_iterator i = allEdeps.begin(); i != allEdeps.end(); ++i) {
        const myParticleEContribArtData & Edeps(**i);
	bool Edep=false;	
	double ETot = 0.0;
        double EProton = 0.0;
        cout << "Edep collection size:  " << Edeps.size() << endl;
	//cout << "Edep total:  "<< Edeps[ETot]<<"  Edep proton:  "<< Edeps[proton]<<endl;
        for (std::map<std::string, double>::const_iterator it = Edeps.begin(); it != Edeps.end(); ++it) {
            std::cout << "Particle: " << it->first << "   " << it->second << " % " << std::endl;
	    if (it->first=="proton")  EProton=  it->second;
	    if (it->first=="ETot") {
	      ETot =   0.001*it->second;
	      Edep=true;
	    }
	}
	if (Edep)
	  {
	    cout << "proton:  "<<EProton<<" Total: " << ETot<<endl;
	    _hEdepvsproton->Fill(EProton,ETot);
	    _ntuple->Fill(ETot,EProton);
	  }
    }
} // end analyze

void artg4tk::CheckDRCalorimeterHits::endJob() {

}// end endJob

using artg4tk::CheckDRCalorimeterHits;

DEFINE_ART_MODULE(CheckDRCalorimeterHits)
