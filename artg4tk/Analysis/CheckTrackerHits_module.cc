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

// art Framework includes.
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art_root_io/TFileService.h"

// artg4tk includes:
#include "artg4tk/pluginDetectors/gdml/TrackerHit.hh"

// Root includes.
#include "TH1F.h"

// Other includes.
#include "CLHEP/Units/SystemOfUnits.h"

// C++ includes
#include <math.h>

using namespace std;
namespace artg4tk {
  class CheckTrackerHits;
}

class artg4tk::CheckTrackerHits : public art::EDAnalyzer {
public:
  explicit CheckTrackerHits(fhicl::ParameterSet const& p);
  void beginJob() override;
  void beginRun(const art::Run& Run) override;
  void analyze(const art::Event& event) override;

private:
  TH1F* _hnHits; // number of TrackerHits
  TH1F* _hEdep;  // total energy deposition
  TH1F* _hrEdep; // radius of Energy deposition
  TH1F* _hzEdep; //   z  position of Energy deposition
};

artg4tk::CheckTrackerHits::CheckTrackerHits(fhicl::ParameterSet const& p)
  : art::EDAnalyzer(p), _hnHits(0), _hEdep(0), _hrEdep(), _hzEdep(0)
{}

void
artg4tk::CheckTrackerHits::beginRun(const art::Run& thisRun)
{
  std::cout << "******************************Run: " << thisRun.id() << std::endl;
}

void
artg4tk::CheckTrackerHits::beginJob()
{
  art::ServiceHandle<art::TFileService> tfs;
  _hnHits = tfs->make<TH1F>("hnHits", "Number of TrackerArtHits", 200, 0., 10000.);
  _hEdep = tfs->make<TH1F>("hEdep", "total Energy deposition in TrackerArtHits", 200, 0., 1.2);
  _hrEdep =
    tfs->make<TH1F>("hrEdep", "radius of total Energy deposition in TrackerArtHits", 300, 0., 300.);
  _hzEdep = tfs->make<TH1F>("hzEdep", "total Energy deposition in TrackerArtHits", 600, 0., 600.);
} // end beginJob

void
artg4tk::CheckTrackerHits::analyze(const art::Event& event)
{
  typedef std::vector<art::Handle<TrackerHitCollection>> HandleVector;
  auto allSims = event.getMany<TrackerHitCollection>();

  for (HandleVector::const_iterator i = allSims.begin(); i != allSims.end(); ++i) {
    const TrackerHitCollection& sims(**i);
    double sumE = 0.0;
    _hnHits->Fill(sims.size());
    for (TrackerHitCollection::const_iterator j = sims.begin(); j != sims.end(); ++j) {
      const TrackerHit& hit = *j;
      double radius = 0.1 * sqrt(hit.xpos * hit.xpos + hit.ypos * hit.ypos);
      _hrEdep->Fill(radius);
      _hzEdep->Fill(0.1 * hit.zpos + 300., hit.Edep);
      sumE = sumE + hit.Edep;
    }
    _hEdep->Fill(sumE / CLHEP::GeV);
  }

} // end analyze

DEFINE_ART_MODULE(artg4tk::CheckTrackerHits)
