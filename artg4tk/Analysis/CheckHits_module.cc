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

// art Framework includes.
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/fwd.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art_root_io/TFileService.h"

// artg4tk includes:
#include "artg4tk/pluginDetectors/gdml/ByParticle.hh"
#include "artg4tk/pluginDetectors/gdml/CalorimeterHit.hh"
#include "artg4tk/pluginDetectors/gdml/DRCalorimeterHit.hh"

// Root includes.
#include "TH1F.h"

// Other includes.
#include "CLHEP/Units/SystemOfUnits.h"

namespace artg4tk {
  class CheckHits;
}

class artg4tk::CheckHits : public art::EDAnalyzer {
public:
  explicit CheckHits(fhicl::ParameterSet const& p);
  void beginJob() override;
  void analyze(const art::Event& event) override;

private:
  bool _DumpGDML;  // enable/disable dumping of GDML geometry information
  TH1F* _hnHits;   // number of CaloHits
  TH1F* _hEdep;    // total energy deposition in CaloHits
  TH1F* _hnDRHits; // number of DRCaloHits
  TH1F* _hDREdep;  // total energy deposition in DRCaloHits
  TH1F* _hNCeren;  // total number of Cerenkovphotons in DRCaloHits
};

using namespace std;

artg4tk::CheckHits::CheckHits(fhicl::ParameterSet const& p)
  : art::EDAnalyzer(p)
  , _DumpGDML(p.get<bool>("DumpGDML"))
  , _hnHits(0)
  , _hEdep(0)
  , _hnDRHits(0)
  , _hDREdep(0)
  , _hNCeren(0)
{}

void
artg4tk::CheckHits::beginJob()
{
  art::ServiceHandle<art::TFileService> tfs;
  _hnHits = tfs->make<TH1F>("hnHits", "Number of CaloArtHits", 100, 0., 20000.);
  _hEdep = tfs->make<TH1F>("hEdep", "total Energy deposition in CaloArtHits", 100, 0., 15.);
  _hnDRHits = tfs->make<TH1F>("hnDRHits", "Number of DRCaloArtHits", 100, 0., 20000.);
  _hDREdep = tfs->make<TH1F>("hDREdep", "total Energy deposition in DRCaloArtHits", 100, 0., 11.);
  _hNCeren = tfs->make<TH1F>(
    "hNCeren", "total number of Cerenkov Photons in DRCaloArtHits", 100, 0., 1000000.);
} // end beginJob

void
artg4tk::CheckHits::analyze(const art::Event& event)
{
  typedef std::vector<art::Handle<CalorimeterHitCollection>> HandleVector;
  auto allSims = event.getMany<CalorimeterHitCollection>();

  cout << "CheckHits Event:  " << event.event() << "  Nr of CaloHit collections: " << allSims.size()
       << endl;
  for (HandleVector::const_iterator i = allSims.begin(); i != allSims.end(); ++i) {
    const CalorimeterHitCollection& sims(**i);
    cout << "CaloHit collection size:  " << sims.size() << endl;
    double sumE = 0.0;
    _hnHits->Fill(sims.size());
    for (CalorimeterHitCollection::const_iterator j = sims.begin(); j != sims.end(); ++j) {
      const CalorimeterHit& hit = *j;
      sumE = sumE + hit.GetEdep();
    }
    _hEdep->Fill(sumE / CLHEP::GeV);
  }
  typedef std::vector<art::Handle<DRCalorimeterHitCollection>> DRHandleVector;
  auto allDRSims = event.getMany<DRCalorimeterHitCollection>();
  cout << "Event:  " << event.event() << "  Nr of DRCaloHit collections: " << allDRSims.size()
       << endl;
  for (DRHandleVector::const_iterator i = allDRSims.begin(); i != allDRSims.end(); ++i) {
    const DRCalorimeterHitCollection& DRsims(**i);
    cout << "DRCaloHit collection size:  " << DRsims.size() << endl;
    double sumDRE = 0.0;
    double sumNCeren = 0.0;
    _hnDRHits->Fill(DRsims.size());
    for (DRCalorimeterHitCollection::const_iterator j = DRsims.begin(); j != DRsims.end(); ++j) {
      const DRCalorimeterHit& hit = *j;
      sumDRE = sumDRE + hit.GetEdep();
      sumNCeren = sumNCeren + hit.GetNceren();
    }
    _hDREdep->Fill(sumDRE / CLHEP::GeV);
    _hNCeren->Fill(sumNCeren);
  }
  typedef std::vector<art::Handle<ByParticle>> EdepHandleVector;
  auto allEdeps = event.getMany<ByParticle>();

  for (EdepHandleVector::const_iterator i = allEdeps.begin(); i != allEdeps.end(); ++i) {
    const ByParticle& Edeps(**i);
    cout << "Edep collection size:  " << Edeps.size() << endl;
    for (std::map<std::string, double>::const_iterator it = Edeps.begin(); it != Edeps.end();
         ++it) {
      std::cout << "Particle: " << it->first << "   " << it->second << " % " << std::endl;
    }
  }

} // end analyze

DEFINE_ART_MODULE(artg4tk::CheckHits)
