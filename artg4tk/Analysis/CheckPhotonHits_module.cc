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

// C++ includes.
#include <iostream>
#include <map>

// art Framework includes.
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art_root_io/TFileService.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/fwd.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

// artg4tk includes:
#include "artg4tk/pluginDetectors/gdml/PhotonHit.hh"

// Root includes.
#include "TNtuple.h"

using namespace std;
namespace artg4tk {
    class CheckPhotonHits;
}

class artg4tk::CheckPhotonHits : public art::EDAnalyzer {
public:

    explicit CheckPhotonHits(fhicl::ParameterSet const& p);
    virtual void beginJob();
    virtual void beginRun(const art::Run& Run);
    virtual void endJob();
    virtual void analyze(const art::Event& event);

private:

    TNtuple* _ntuple;
};

artg4tk::CheckPhotonHits::CheckPhotonHits(fhicl::ParameterSet const& p) :
art::EDAnalyzer(p),
_ntuple(0) {
}

void artg4tk::CheckPhotonHits::beginRun(const art::Run & thisRun) {

}

void artg4tk::CheckPhotonHits::beginJob() {

    art::ServiceHandle<art::TFileService> tfs;
    _ntuple = tfs->make<TNtuple>("ntuple", " ntuple",
            "Event:Section:ID:NPhotons");
} // end beginJob

void artg4tk::CheckPhotonHits::analyze(const art::Event & event) {
    typedef std::vector< art::Handle<PhotonHitCollection> > HandleVector;
    std::map<int, int> photonsperdet;
    HandleVector allSims;
    event.getManyByType(allSims);
    //    cout << "CheckPhotonHits Event:  " << event.event() << "  Nr of PhotonHit collections: " << allSims.size() << endl;
    int section = 0;
    for (HandleVector::const_iterator i = allSims.begin(); i != allSims.end(); ++i) {
        photonsperdet.clear();
        const PhotonHitCollection & sims(**i);
	//        cout << "PhotonHit collection size:  " << sims.size() << endl;
        for (PhotonHitCollection::const_iterator j = sims.begin(); j != sims.end(); ++j) {
            const PhotonHit& hit = *j;
            /*std::cout << "Section: "<< section<<
                    "  ID: " << hit.GetID() << 
                    "  x: " << hit.GetXpos() << 
                    "  Y: " << hit.GetYpos() << 
                    " z: " << hit.GetZpos() <<
                    std::endl;
	    */
            if (photonsperdet.find(hit.GetID()) == photonsperdet.end()) // new Detector
            {
                photonsperdet.insert(std::make_pair(hit.GetID(), 1));
            } else // new hit on Detector
            {
                photonsperdet[hit.GetID()]++;
            }

            /*_ntuple->Fill(event.event(), section,
                     hit.GetID(),
                     hit.GetEdep(),
                     hit.GetEdepEM(),
                     hit.GetEdepnonEM(),
                     hit.GetXpos(),
                     hit.GetYpos(),
                     hit.GetZpos(),
                     hit.GetTime());
             */
        }
        std::map<int, int>::iterator it = photonsperdet.begin();
        while (it != photonsperdet.end()) {
            std::cout << it->first << " :: " << it->second << std::endl;
            it++;
        }
    }
    section++;

} // end analyze

void artg4tk::CheckPhotonHits::endJob() {
    cout << " ******************************** CheckPhotonHits: I am done " << endl;
}// end endJob

using artg4tk::CheckPhotonHits;

DEFINE_ART_MODULE(CheckPhotonHits)
