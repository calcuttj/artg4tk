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
// CheckProtonProduction_module.cc: Analyzer module that demonstrates access to
// and makes some histograms
//
// Author: Hans Wenzel (Fermilab)
//=============================================================================

// C++ includes.
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// art Framework includes.
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art_root_io/TFileService.h"

// artg4tk includes:
#include "artg4tk/pluginDetectors/gdml/myInteractionArtHitData.hh"

// Root includes.
#include "TH1F.h"

using namespace std;
namespace artg4tk {
  class CheckProtonProduction;
}

class artg4tk::CheckProtonProduction : public art::EDAnalyzer {
public:
  explicit CheckProtonProduction(fhicl::ParameterSet const& p);
  void beginJob() override;
  void beginRun(const art::Run& Run) override;
  void endJob() override;
  void analyze(const art::Event& event) override;

private:
  int fNThetaBinsFW;
  double fThetaMinFW;
  double fDeltaThetaFW;
  int fNThetaBinsLA;
  double fThetaMinLA;
  double fDeltaThetaLA;
  //
  TH1F* _fHistoNSec;        // number of secondaries
                            //
  TH1F* _fHistoSecPMom;     // momentum of secondary protons
  TH1F* _fHistoSecPkinE54;  // kinetic Energy of secondary protons
  TH1F* _fHistoSecPkinE68;  // kinetic Energy of secondary protons
  TH1F* _fHistoSecPkinE90;  // kinetic Energy of secondary protons
  TH1F* _fHistoSecPkinE121; // kinetic Energy of secondary protons
  TH1F* _fHistoSecPkinE164; // kinetic Energy of secondary protons
  TH1F* _fHistoSecPkinE;    // kinetic Energy of secondary protons
  TH1F* _fHistoSecPTheta;   // theta of secondary protons
                            //
  TH1F* _fHistoSecDMom;     // momentum of secondary deuterons
  TH1F* _fHistoSecDkinE54;  // kinetic Energy of secondary deuterons
  TH1F* _fHistoSecDkinE68;  // kinetic Energy of secondary deuterons
  TH1F* _fHistoSecDkinE90;  // kinetic Energy of secondary deuterons
  TH1F* _fHistoSecDkinE121; // kinetic Energy of secondary deuterons
  TH1F* _fHistoSecDkinE164; // kinetic Energy of secondary deuterons
  TH1F* _fHistoSecDkinE;    // kinetic Energy of secondary deuterons
  TH1F* _fHistoSecDTheta;   // theta of secondary deuterons
                            //
  TH1F* _fHistoSecTMom;     // momentum of secondary tritons
  TH1F* _fHistoSecTkinE54;  // kinetic Energy of secondary tritons
  TH1F* _fHistoSecTkinE68;  // kinetic Energy of secondary tritons
  TH1F* _fHistoSecTkinE90;  // kinetic Energy of secondary tritons
  TH1F* _fHistoSecTkinE121; // kinetic Energy of secondary tritons
  TH1F* _fHistoSecTkinE164; // kinetic Energy of secondary tritons
  TH1F* _fHistoSecTkinE;    // kinetic Energy of secondary tritons
  TH1F* _fHistoSecTTheta;   // theta of secondary tritons
  std::vector<TH1D*> fHistoSecProtonFW;
};

artg4tk::CheckProtonProduction::CheckProtonProduction(fhicl::ParameterSet const& p)
  : art::EDAnalyzer(p)
  , fNThetaBinsFW(p.get<int>("ThetaBinsFW", 4))
  , fThetaMinFW(p.get<double>("ThetaMinFW", 0.05))
  , fDeltaThetaFW(p.get<double>("DeltaThetaFW", 0.05))
  , fNThetaBinsLA(p.get<int>("ThetaBinsLA", 9))
  , fThetaMinLA(p.get<double>("ThetaMinLA", 0.35))
  , fDeltaThetaLA(p.get<double>("DeltaThetaLA", 0.2))
  , _fHistoNSec(0)
  , _fHistoSecPMom(0)
  , _fHistoSecPkinE54(0)
  , _fHistoSecPkinE68(0)
  , _fHistoSecPkinE90(0)
  , _fHistoSecPkinE121(0)
  , _fHistoSecPkinE164(0)
  , _fHistoSecPkinE(0)
  , _fHistoSecPTheta(0)
  , _fHistoSecDMom(0)
  , _fHistoSecDkinE54(0)
  , _fHistoSecDkinE68(0)
  , _fHistoSecDkinE90(0)
  , _fHistoSecDkinE121(0)
  , _fHistoSecDkinE164(0)
  , _fHistoSecDkinE(0)
  , _fHistoSecDTheta(0)
  , _fHistoSecTMom(0)
  , _fHistoSecTkinE54(0)
  , _fHistoSecTkinE68(0)
  , _fHistoSecTkinE90(0)
  , _fHistoSecTkinE121(0)
  , _fHistoSecTkinE164(0)
  , _fHistoSecTkinE(0)
  , _fHistoSecTTheta(0)
{}

void
artg4tk::CheckProtonProduction::beginRun(const art::Run& thisRun)
{
  thisRun.beginTime();
}

void
artg4tk::CheckProtonProduction::beginJob()
{
  art::ServiceHandle<art::TFileService> tfs;
  _fHistoNSec = tfs->make<TH1F>("NSec", "neutron + Cu", 100, 0., 100.);
  //
  _fHistoSecPMom =
    tfs->make<TH1F>("pmomentum", "neutron + Cu momentum of secondary protons", 100, 0., 600);
  _fHistoSecPkinE54 = tfs->make<TH1F>(
    "pkinE54", "neutron + Cu kinetic Energy of secondary protons (54)", 100, 0., 400.);
  _fHistoSecPkinE68 = tfs->make<TH1F>(
    "pkinE68", "neutron + Cu kinetis Energy of secondary protons (68)", 100, 0., 400.);
  _fHistoSecPkinE90 = tfs->make<TH1F>(
    "pkinE90", "neutron + Cu kinetic Energy of secondary protons (90)", 100, 0., 400.);
  _fHistoSecPkinE121 = tfs->make<TH1F>(
    "pkinE121", "neutron + Cu kinetic Energy of secondary protons (121)", 100, 0., 400);
  _fHistoSecPkinE164 = tfs->make<TH1F>(
    "pkinE164", "neutron + Cu kinetic Energy of secondary protons (168)", 100, 0., 400.);
  _fHistoSecPkinE =
    tfs->make<TH1F>("pkinE", "neutron + Cu kinetic Energy  of secondary protons", 100, 0., 400.);
  _fHistoSecPTheta =
    tfs->make<TH1F>("ptheta", "neutron + Cu kin. theta of secondary protons", 100, 0., 3.2);
  //
  _fHistoSecDMom =
    tfs->make<TH1F>("dmomentum", "neutron + Cu momentum of secondary deuterons", 100, 0., 600);
  _fHistoSecDkinE54 = tfs->make<TH1F>(
    "dkinE54", "neutron + Cu kinetic Energy of secondary deuterons (54)", 100, 0., 400.);
  _fHistoSecDkinE68 = tfs->make<TH1F>(
    "dkinE68", "neutron + Cu kinetis Energy of secondary deuterons (68)", 100, 0., 400.);
  _fHistoSecDkinE90 = tfs->make<TH1F>(
    "dkinE90", "neutron + Cu kinetic Energy of secondary deuterons (90)", 100, 0., 400.);
  _fHistoSecDkinE121 = tfs->make<TH1F>(
    "dkinE121", "neutron + Cu kinetic Energy of secondary deuterons (121)", 100, 0., 400);
  _fHistoSecDkinE164 = tfs->make<TH1F>(
    "dkinE164", "neutron + Cu kinetic Energy of secondary deuterons (168)", 100, 0., 400.);
  _fHistoSecDkinE =
    tfs->make<TH1F>("dkinE", "neutron + Cu kinetic Energy  of secondary deuterons", 100, 0., 400.);
  _fHistoSecDTheta =
    tfs->make<TH1F>("dtheta", "neutron + Cu kin. theta of secondary deuterons", 100, 0., 3.2);
  //
  _fHistoSecTMom =
    tfs->make<TH1F>("tmomentum", "neutron + Cu momentum of secondary tritons", 100, 0., 600);
  _fHistoSecTkinE54 = tfs->make<TH1F>(
    "tkinE54", "neutron + Cu kinetic Energy of secondary tritons (54)", 100, 0., 400.);
  _fHistoSecTkinE68 = tfs->make<TH1F>(
    "tkinE68", "neutron + Cu kinetis Energy of secondary tritons (68)", 100, 0., 400.);
  _fHistoSecTkinE90 = tfs->make<TH1F>(
    "tkinE90", "neutron + Cu kinetic Energy of secondary tritons (90)", 100, 0., 400.);
  _fHistoSecTkinE121 = tfs->make<TH1F>(
    "tkinE121", "neutron + Cu kinetic Energy of secondary tritons (121)", 100, 0., 400);
  _fHistoSecTkinE164 = tfs->make<TH1F>(
    "tkinE164", "neutron + Cu kinetic Energy of secondary tritons (168)", 100, 0., 400.);
  _fHistoSecTkinE =
    tfs->make<TH1F>("tkinE", "neutron + Cu kinetic Energy  of secondary tritons", 100, 0., 400.);
  _fHistoSecTTheta =
    tfs->make<TH1F>("ttheta", "neutron + Cu kin. theta of secondary tritons", 100, 0., 3.2);
  //
  string htitle;
  string hname;
  string ht = "neutron + Cu";

  double thetaMin = 0.;
  double thetaMax = 0.;
  std::string theta_bin_fw;
  std::string theta_bin_la;

  for (int i = 0; i < fNThetaBinsFW; i++) {
    thetaMin = fThetaMinFW + fDeltaThetaFW * i;
    thetaMax = thetaMin + fDeltaThetaFW;

    std::ostringstream osTitle1;
    std::ostringstream osTitle2;
    std::ostringstream osTitle3;

    osTitle1.clear();
    osTitle1 << thetaMin;
    theta_bin_fw = osTitle1.str() + " < theta < ";
    osTitle2.clear();
    osTitle2 << thetaMax;
    theta_bin_fw += osTitle2.str();
    theta_bin_fw += "(rad)";

    osTitle3.clear();
    osTitle3 << i;

    htitle = ht + " -> X + proton, " + theta_bin_fw;
    hname = "proton_fw_" + osTitle3.str();
    TH1D* histo = tfs->make<TH1D>(hname.c_str(), htitle.c_str(), 80, 0., 8.0);
    fHistoSecProtonFW.push_back(histo);
  }

} // end beginJob

void
artg4tk::CheckProtonProduction::analyze(const art::Event& event)
{
  typedef std::vector<art::Handle<myInteractionArtHitDataCollection>> HandleVector;
  auto allSims = event.getMany<myInteractionArtHitDataCollection>();

  for (HandleVector::const_iterator i = allSims.begin(); i != allSims.end(); ++i) {
    const myInteractionArtHitDataCollection& sims(**i);
    if (sims.size() > 0) {
      _fHistoNSec->Fill(sims.size());
    }
    for (myInteractionArtHitDataCollection::const_iterator j = sims.begin(); j != sims.end(); ++j) {
      const myInteractionArtHitData& hit = *j;
      if (hit.pname == "proton") {
        _fHistoSecPMom->Fill(hit.pmom);
        double angle = hit.theta * 57.29577951;
        if (fabs(angle - 54.) < 2.)
          _fHistoSecPkinE54->Fill(hit.Ekin);
        if (fabs(angle - 68.) < 2.)
          _fHistoSecPkinE68->Fill(hit.Ekin);
        if (fabs(angle - 90.) < 2.)
          _fHistoSecPkinE90->Fill(hit.Ekin);
        if (fabs(angle - 121.) < 2.)
          _fHistoSecPkinE121->Fill(hit.Ekin);
        if (fabs(angle - 164.) < 2.)
          _fHistoSecPkinE164->Fill(hit.Ekin);
        _fHistoSecPkinE->Fill(hit.Ekin);
        _fHistoSecPTheta->Fill(hit.theta);
      } else if (hit.pname == "deuteron") {
        _fHistoSecDMom->Fill(hit.pmom);
        double angle = hit.theta * 57.29577951;
        if (fabs(angle - 54.) < 2.)
          _fHistoSecDkinE54->Fill(hit.Ekin);
        if (fabs(angle - 68.) < 2.)
          _fHistoSecDkinE68->Fill(hit.Ekin);
        if (fabs(angle - 90.) < 2.)
          _fHistoSecDkinE90->Fill(hit.Ekin);
        if (fabs(angle - 121.) < 2.)
          _fHistoSecDkinE121->Fill(hit.Ekin);
        if (fabs(angle - 164.) < 2.)
          _fHistoSecDkinE164->Fill(hit.Ekin);
        _fHistoSecDkinE->Fill(hit.Ekin);
        _fHistoSecDTheta->Fill(hit.theta);
      } else if (hit.pname == "triton") {
        _fHistoSecTMom->Fill(hit.pmom);
        double angle = hit.theta * 57.29577951;
        if (fabs(angle - 54.) < 2.)
          _fHistoSecTkinE54->Fill(hit.Ekin);
        if (fabs(angle - 68.) < 2.)
          _fHistoSecTkinE68->Fill(hit.Ekin);
        if (fabs(angle - 90.) < 2.)
          _fHistoSecTkinE90->Fill(hit.Ekin);
        if (fabs(angle - 121.) < 2.)
          _fHistoSecTkinE121->Fill(hit.Ekin);
        if (fabs(angle - 164.) < 2.)
          _fHistoSecTkinE164->Fill(hit.Ekin);
        _fHistoSecTkinE->Fill(hit.Ekin);
        _fHistoSecTTheta->Fill(hit.theta);
      }
    }
  }

} // end analyze

void
artg4tk::CheckProtonProduction::endJob()
{
  cout << " ********************************CheckProtonProduction: now normalizing the histos "
       << endl;
  double norm = _fHistoNSec->Integral();
  double xbin = _fHistoNSec->GetBinWidth(1);
  double scale = 1. / (xbin * norm);
  _fHistoNSec->Scale(scale);
} // end endJob

DEFINE_ART_MODULE(artg4tk::CheckProtonProduction)
