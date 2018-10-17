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
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Provenance.h"

artg4tk::CheckDRCalorimeterHits::CheckDRCalorimeterHits(fhicl::ParameterSet const& p) :
art::EDAnalyzer(p),
_hnDRHits(0),
_hDREdep(0),
_hNCeren(0),
_hEdepvsNCeren(0),
_ntuple(0) {
}

void artg4tk::CheckDRCalorimeterHits::beginRun(const art::Run& thisRun) {
  //  std::cout << "******************************Run: " << thisRun.id() << ": looking at Run Header" << std::endl;
}

void artg4tk::CheckDRCalorimeterHits::beginJob() {
    art::ServiceHandle<art::TFileService> tfs;
    _hnDRHits = tfs->make<TH1F>("hnDRHits", "Number of DRCaloArtHits", 100, 0., 20000.);
    _hDREdep = tfs->make<TH1F>("hDREdep", "total Energy deposition in DRCaloArtHits", 100, 0., 11.);
    _hNCeren = tfs->make<TH1F>("hNCeren", "total number of Cerenkov Photons in DRCaloArtHits", 100, 0., 10000.);
    _hEdepvsNCeren= tfs->make<TH2F>("hEdepvsNCeren","Edep vs. NCeren",100,0,11,100,0,10000.);
    _ntuple = tfs->make<TNtuple>("ntuple","proton percentage",			  "EVIS:PPERC");
    mapofhistos["Fragment"] = tfs->make<TH1F>("hFragment","Fragment percentage", 100, 0., 100.);
    mapofhistos["He3"] = tfs->make<TH1F>("hHe3","He3 percentage", 100, 0., 100.);
    mapofhistos["alpha"] = tfs->make<TH1F>("halpha","alpha percentage", 100, 0., 100.);
    mapofhistos["deuteron"] = tfs->make<TH1F>("hdeuteron","deuteron percentage", 100, 0., 100.);
    mapofhistos["triton"] = tfs->make<TH1F>("htriton","triton percentage", 100, 0., 100.);
    mapofhistos["proton"] = tfs->make<TH1F>("hproton","proton percentage", 100, 0., 100.);
    mapofhistos["neutron"] = tfs->make<TH1F>("hneutron","neutron percentage", 100, 0., 100.);
    mapofhistos["e+"] = tfs->make<TH1F>("he+","positron percentage", 100, 0., 100.);
    mapofhistos["e-"] = tfs->make<TH1F>("he-","electron percentage", 100, 0., 100.);
    mapofhistos["pi+"] = tfs->make<TH1F>("hpiplus","piplus percentage", 100, 0., 100.);
    mapofhistos["pi-"] = tfs->make<TH1F>("hpiminus","piminus percentage", 100, 0., 100.);
    mapofhistos["gamma"] = tfs->make<TH1F>("hgamma","gamma percentage", 100, 0., 100.);
    mapofhistos["mu+"] = tfs->make<TH1F>("hmu+","mu+ percentage", 100, 0., 100.);
    mapofhistos["mu-"] = tfs->make<TH1F>("hmu-","mu- percentage", 100, 0., 100.);
    mapofhistos["sigma+"] = tfs->make<TH1F>("hsigma+","sigma+ percentage", 100, 0., 100.);
    mapofhistos["sigma-"] = tfs->make<TH1F>("hsigma-","sigma- percentage", 100, 0., 100.);
    mapofhistos["kaon+"] = tfs->make<TH1F>("hkaon+","kaon+ percentage", 100, 0., 100.);
    mapofhistos["kaon-"] = tfs->make<TH1F>("hkaon-","kaon- percentage", 100, 0., 100.);
    mapofhistos["kaon0L"] = tfs->make<TH1F>("hkaon0L","kaon0 percentage", 100, 0., 100.);
    mapofhistos["kaon0S"] = tfs->make<TH1F>("hkaon0S","Kspercentage", 100, 0., 100.);
    mapofhistos["lambda"] = tfs->make<TH1F>("hlambda","lambda percentage", 100, 0., 100.);
    mapofhistos["xi-"] = tfs->make<TH1F>("hximinus","ximinu percentage", 100, 0., 100.);
    mapofhistos["anti_neutron"] = tfs->make<TH1F>("hneutronbar","neutronbar percentage", 100, 0., 100.);
    mapofhistos["anti_sigma-"] = tfs->make<TH1F>("hsigmaminusbar","anti_sigma- percentage", 100, 0., 100.);
    mapofhistos["anti_proton"] = tfs->make<TH1F>("hpar","pbar percentage", 100, 0., 100.);
    mapofhistos["anti_xi-"] = tfs->make<TH1F>("hantiximinus","anti_xi-percentage", 100, 0., 100.);
    mapofhistos["anti_omega-"] = tfs->make<TH1F>("hantiomegaminus","anti_omega- percentage", 100, 0., 100.);
    mapofhistos["anti_sigma+"] = tfs->make<TH1F>("hantisigmaplus","anti_sigma+percentage", 100, 0., 100.);
    mapofhistos["anti_lambda"] = tfs->make<TH1F>("hlambdabar","anti_lambda percentage", 100, 0., 100.);
    mapofhistos["anti_xi0"] = tfs->make<TH1F>("hxi0bar","anti_xi0 percentage", 100, 0., 100.);
    mapofhistos["other"] = tfs->make<TH1F>("hother","other percentage", 100, 0., 100.); // Just in case 
    const unsigned int numz=10;
    for (unsigned int jj=0;jj<numz;jj++)
      {
	std::string histonamethin= "Edep_thin_lay"+std::to_string(jj);
	std::string histonamethick= "Edep_thick_lay"+std::to_string(jj);
	cout<<histonamethin<<endl;
	cout<<histonamethin.c_str()<<endl;
	vecofhistosthin.push_back(tfs->make<TH1F>(histonamethin.c_str(),histonamethin.c_str(), 100, 0., 2.));
        vecofhistosthick.push_back(tfs->make<TH1F>(histonamethick.c_str(),histonamethick.c_str(), 100, 0., 2.));
      }
} // end beginJob

void artg4tk::CheckDRCalorimeterHits::analyze(const art::Event& event) {
    typedef std::vector< art::Handle<DRCalorimeterHitCollection> > DRHandleVector;
    DRHandleVector allDRSims;
    double sumDRE=0;
    double sumNCeren = 0.0;
    unsigned int numz=10;
    //unsigned int numy=6;

    std::vector<double> edepthin;
    std::vector<double> edepthick;
    for (unsigned int ijk=0;ijk<numz;ijk++)
      {
	edepthin.push_back(0.0);
	edepthick.push_back(0.0);
      }
    event.getManyByType(allDRSims);
    cout << "Event:  " << event.event() << "  Nr of DRCaloHit collections: " << allDRSims.size() << endl;
    for (DRHandleVector::const_iterator i = allDRSims.begin(); i != allDRSims.end(); ++i) {
      art::Handle<DRCalorimeterHitCollection> ih = *i;
       auto const* prov  = ih.provenance();
       string iname = prov->productInstanceName();
       const DRCalorimeterHitCollection & DRsims(**i);
       _hnDRHits->Fill(DRsims.size());
       for (DRCalorimeterHitCollection::const_iterator j = DRsims.begin(); j != DRsims.end(); ++j) {
	 const DRCalorimeterHit& hit = *j;
	 unsigned int lay = (int)(hit.GetID()/(numz*numz));
	 //unsigned int jj = (int)(((hit.GetID()-kk*(numz*numz)))/numy);
	 //unsigned int ii = (int)(hit.GetID()-kk*(numz*numz)-jj*numy);		    
	 //cout << "ID:  "<<hit.GetID()<<"  Layer:  "<< kk<<"  Column:  " << jj<< "  row:  "<< ii<<endl;	    
	 sumDRE = sumDRE + hit.GetEdep();
	 sumNCeren = sumNCeren + hit.GetNceren();
	 if(iname.find("thin")!= std::string::npos)
	   {
	     edepthin[lay]=edepthin[lay]+hit.GetEdep();
	   } else
	   {
	     edepthick[lay]=edepthick[lay]+hit.GetEdep();
	   }
       }
    }
    _hDREdep->Fill(sumDRE / CLHEP::GeV);
    _hNCeren->Fill(sumNCeren);
    _hEdepvsNCeren->Fill(sumDRE / CLHEP::GeV,sumNCeren);
   for (unsigned int ijk=0;ijk<numz;ijk++)
      {
	vecofhistosthin[ijk]->Fill(edepthin[ijk] / CLHEP::GeV);
	vecofhistosthick[ijk]->Fill(edepthick[ijk] / CLHEP::GeV);
      }
    typedef std::vector< art::Handle<ByParticle> > EdepHandleVector;
    EdepHandleVector allEdeps;
    event.getManyByType(allEdeps);
    bool first=true;
    ByParticle addup;
    double junkie=0.0;
    for ( EdepHandleVector::const_iterator i = allEdeps.begin(); i != allEdeps.end(); ++i) {
      art::Handle<ByParticle> ih = *i;
       auto const* prov  = ih.provenance();
       string instancename = prov->productInstanceName();
       if(instancename.find("NCeren")!= std::string::npos) cout << "Cerenkov Collection"<<endl;
       if(instancename.find("Edep")!= std::string::npos) 
	 {
	   const ByParticle & Edeps(**i);
	   if (first){
	     addup = Edeps;
	     junkie = Edeps.at("Etot");
	     first=false;
	   } 
	   else 
	     {
	        junkie = junkie+ Edeps.at("Etot");
	       for (std::map<std::string, double>::const_iterator it = Edeps.begin(); it != Edeps.end(); ++it) {
		 addup[it->first]=addup[it->first]+it->second;
	       }
	     }
	   /*
	   for (std::map<std::string, double>::const_iterator it = Edeps.begin(); it != Edeps.end(); ++it) {
	     
	     if (mapofhistos.find(it->first) != mapofhistos.end()) mapofhistos[it->first]->Fill((100.*it->second)/junkie);
	   }
	   */
	 }
    }
    for (std::map<std::string, double>::const_iterator it = addup.begin(); it != addup.end(); ++it) {
      if (mapofhistos.find(it->first) != mapofhistos.end()) mapofhistos[it->first]->Fill((100.*it->second)/junkie);
    }
} // end analyze

void artg4tk::CheckDRCalorimeterHits::endJob() {

}// end endJob
using artg4tk::CheckDRCalorimeterHits;

DEFINE_ART_MODULE(CheckDRCalorimeterHits)
