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
//===============================================================================
// CheckDRCalorimeterHits_module.cc: Analyzer module that demonstrates access to
// DRCalorimeter hits and makes some histograms
//
// Author: Hans Wenzel (Fermilab)
//===============================================================================

#include "fhiclcpp/ParameterSet.h"

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Provenance.h"
#include "art/Framework/Principal/fwd.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art_root_io/TFileService.h"
// artg4tk includes:
#include "artg4tk/pluginDetectors/gdml/ByParticle.hh"
#include "artg4tk/pluginDetectors/gdml/DRCalorimeterHit.hh"
#include "artg4tk/services/DetectorHolder_service.hh"

// Root includes.
#include "TH1F.h"
#include "TH2F.h"
#include "TNtuple.h"

// Other includes.
#include "CLHEP/Units/SystemOfUnits.h"
using namespace std;
namespace artg4tk {
  class CheckDRCalorimeterHits;
}

class artg4tk::CheckDRCalorimeterHits : public art::EDAnalyzer {
public:
  explicit CheckDRCalorimeterHits(fhicl::ParameterSet const& p);
  void beginJob() override;
  void analyze(const art::Event& event) override;

private:
  fhicl::ParameterSet pstl; // parameterset from PhysicsListService
  std::map<std::string, TH1F*> mapofhistos;
  std::map<std::string, TH1F*> ncmapofhistos;
  std::vector<TH1F*> vecofhistosthin;
  std::vector<TH1F*> vecofhistosthick;
  std::vector<double> edepthin;
  std::vector<double> edepthick;
  TH1F* _hnDRHits; // number of DRCaloHits
  TH1F* _hDREdep;  // total energy deposition in DRCaloHits
  TH1F* _hNCeren;  // total number of Cerenkovphotons in DRCaloHits
  TH2F* _hEdepvsNCeren;
  TNtuple* _ntuple;
  TNtuple* _ntuple2;
};

artg4tk::CheckDRCalorimeterHits::CheckDRCalorimeterHits(fhicl::ParameterSet const& p)
  : art::EDAnalyzer(p)
  , pstl(p.get<fhicl::ParameterSet>("pstl"))
  , _hnDRHits(0)
  , _hDREdep(0)
  , _hNCeren(0)
  , _hEdepvsNCeren(0)
  , _ntuple(0)
  , _ntuple2(0)
{}

void
artg4tk::CheckDRCalorimeterHits::beginJob()
{
  art::ServiceHandle<art::TFileService> tfs;
  _hnDRHits = tfs->make<TH1F>("hnDRHits", "Number of DRCaloArtHits", 100, 0., 20000.);
  _hDREdep = tfs->make<TH1F>("hDREdep", "total Energy deposition in DRCaloArtHits", 100, 0., 11.);
  _hNCeren = tfs->make<TH1F>(
    "hNCeren", "total number of Cerenkov Photons in DRCaloArtHits", 100, 0., 10000.);
  _hEdepvsNCeren = tfs->make<TH2F>("hEdepvsNCeren", "Edep vs. NCeren", 100, 0, 11, 100, 0, 10000.);
  _ntuple = tfs->make<TNtuple>(
    "ntuple", "Demo ntuple", "Event:Edep0:Edep1:Edep2:Edep3:Edep4:Edep5:Edep6:Edep7:Edep8:Edep9");
  _ntuple2 = tfs->make<TNtuple>(
    "ntuple2",
    "Demo ntuple",
    "Event:Nceren0:Nceren1:Nceren2:Nceren3:Nceren4:Nceren5:Nceren6:Nceren7:Nceren8:Nceren9");
  mapofhistos["Fragment"] = tfs->make<TH1F>("hFragment", "Fragment percentage", 100, 0., 100.);
  mapofhistos["He3"] = tfs->make<TH1F>("hHe3", "He3 percentage", 100, 0., 100.);
  mapofhistos["alpha"] = tfs->make<TH1F>("halpha", "alpha percentage", 100, 0., 100.);
  mapofhistos["deuteron"] = tfs->make<TH1F>("hdeuteron", "deuteron percentage", 100, 0., 100.);
  mapofhistos["triton"] = tfs->make<TH1F>("htriton", "triton percentage", 100, 0., 100.);
  mapofhistos["proton"] = tfs->make<TH1F>("hproton", "proton percentage", 100, 0., 100.);
  mapofhistos["neutron"] = tfs->make<TH1F>("hneutron", "neutron percentage", 100, 0., 100.);
  mapofhistos["e+"] = tfs->make<TH1F>("he+", "positron percentage", 100, 0., 100.);
  mapofhistos["e-"] = tfs->make<TH1F>("he-", "electron percentage", 100, 0., 100.);
  mapofhistos["pi+"] = tfs->make<TH1F>("hpiplus", "piplus percentage", 100, 0., 100.);
  mapofhistos["pi-"] = tfs->make<TH1F>("hpiminus", "piminus percentage", 100, 0., 100.);
  mapofhistos["gamma"] = tfs->make<TH1F>("hgamma", "gamma percentage", 100, 0., 100.);
  mapofhistos["mu+"] = tfs->make<TH1F>("hmu+", "mu+ percentage", 100, 0., 100.);
  mapofhistos["mu-"] = tfs->make<TH1F>("hmu-", "mu- percentage", 100, 0., 100.);
  mapofhistos["sigma+"] = tfs->make<TH1F>("hsigma+", "sigma+ percentage", 100, 0., 100.);
  mapofhistos["sigma-"] = tfs->make<TH1F>("hsigma-", "sigma- percentage", 100, 0., 100.);
  mapofhistos["kaon+"] = tfs->make<TH1F>("hkaon+", "kaon+ percentage", 100, 0., 100.);
  mapofhistos["kaon-"] = tfs->make<TH1F>("hkaon-", "kaon- percentage", 100, 0., 100.);
  mapofhistos["kaon0L"] = tfs->make<TH1F>("hkaon0L", "kaon0 percentage", 100, 0., 100.);
  mapofhistos["kaon0S"] = tfs->make<TH1F>("hkaon0S", "Kspercentage", 100, 0., 100.);
  mapofhistos["lambda"] = tfs->make<TH1F>("hlambda", "lambda percentage", 100, 0., 100.);
  mapofhistos["xi-"] = tfs->make<TH1F>("hximinus", "ximinu percentage", 100, 0., 100.);
  mapofhistos["anti_neutron"] =
    tfs->make<TH1F>("hneutronbar", "neutronbar percentage", 100, 0., 100.);
  mapofhistos["anti_sigma-"] =
    tfs->make<TH1F>("hsigmaminusbar", "anti_sigma- percentage", 100, 0., 100.);
  mapofhistos["anti_proton"] = tfs->make<TH1F>("hpar", "pbar percentage", 100, 0., 100.);
  mapofhistos["anti_xi-"] = tfs->make<TH1F>("hantiximinus", "anti_xi-percentage", 100, 0., 100.);
  mapofhistos["anti_omega-"] =
    tfs->make<TH1F>("hantiomegaminus", "anti_omega- percentage", 100, 0., 100.);
  mapofhistos["anti_sigma+"] =
    tfs->make<TH1F>("hantisigmaplus", "anti_sigma+percentage", 100, 0., 100.);
  mapofhistos["anti_lambda"] =
    tfs->make<TH1F>("hlambdabar", "anti_lambda percentage", 100, 0., 100.);
  mapofhistos["anti_xi0"] = tfs->make<TH1F>("hxi0bar", "anti_xi0 percentage", 100, 0., 100.);
  mapofhistos["other"] =
    tfs->make<TH1F>("hother", "other percentage", 100, 0., 100.); // Just in case
  //
  ncmapofhistos["deuteron"] =
    tfs->make<TH1F>("NCeren_deuteron", "deuteronp ercentage", 100, 0., 100.);
  ncmapofhistos["triton"] = tfs->make<TH1F>("NCeren_triton", "triton percentage", 100, 0., 100.);
  ncmapofhistos["He3"] = tfs->make<TH1F>("NCeren_He3", "He3 percentage", 100, 0., 100.);
  ncmapofhistos["proton"] = tfs->make<TH1F>("NCeren_proton", "proton percentage", 100, 0., 100.);
  ncmapofhistos["e+"] = tfs->make<TH1F>("NCeren_e+", "e+ percentage", 100, 0., 100.);
  ncmapofhistos["e-"] = tfs->make<TH1F>("NCeren_e-", "e- percentage", 100, 0., 100.);
  ncmapofhistos["mu+"] = tfs->make<TH1F>("NCeren_mu+", "mu+ percentage", 100, 0., 100.);
  ncmapofhistos["mu-"] = tfs->make<TH1F>("NCeren_mu-", "mu- percentage", 100, 0., 100.);
  ncmapofhistos["pi+"] = tfs->make<TH1F>("NCeren_pi+", "pi+ percentage", 100, 0., 100.);
  ncmapofhistos["pi-"] = tfs->make<TH1F>("NCeren_pi-", "pi- percentage", 100, 0., 100.);
  ncmapofhistos["kaon+"] = tfs->make<TH1F>("NCeren_kaon+", "kaon+ percentage", 100, 0., 100.);
  ncmapofhistos["kaon-"] = tfs->make<TH1F>("NCeren_kaon-", "kaon- percentage", 100, 0., 100.);
  ncmapofhistos["sigma+"] = tfs->make<TH1F>("NCeren_sigma+", "sigma+ percentage", 100, 0., 100.);
  ncmapofhistos["sigma-"] = tfs->make<TH1F>("NCeren_sigma-", "sigma- percentage", 100, 0., 100.);
  ncmapofhistos["xi-"] = tfs->make<TH1F>("NCeren_xi-", "xi- percentage", 100, 0., 100.);
  ncmapofhistos["anti_sigma+"] =
    tfs->make<TH1F>("NCeren_anti_sigma+", "anti_sigma+ percentage", 100, 0., 100.);
  ncmapofhistos["anti_sigma-"] =
    tfs->make<TH1F>("NCeren_anti_sigma-", "anti_sigma- percentage", 100, 0., 100.);
  ncmapofhistos["anti_proton"] =
    tfs->make<TH1F>("NCeren_anti_proton", "anti_proton percentage", 100, 0., 100.);
  ncmapofhistos["anti_xi-"] =
    tfs->make<TH1F>("NCeren_anti_xi-", "anti_xi- percentage", 100, 0., 100.);
  ncmapofhistos["anti_omega-"] =
    tfs->make<TH1F>("NCeren_anti_omega-", "anti_omega- percentage", 100, 0., 100.);
  ncmapofhistos["other"] =
    tfs->make<TH1F>("NCeren_other", "other percentage", 100, 0., 100.); // Just in case
  const unsigned int numz = 10;
  for (unsigned int jj = 0; jj < numz; jj++) {
    std::string histonamethin = "Edep_thin_lay" + std::to_string(jj);
    std::string histonamethick = "Edep_thick_lay" + std::to_string(jj);
    cout << histonamethin << endl;
    cout << histonamethin.c_str() << endl;
    vecofhistosthin.push_back(
      tfs->make<TH1F>(histonamethin.c_str(), histonamethin.c_str(), 100, 0., 2.));
    vecofhistosthick.push_back(
      tfs->make<TH1F>(histonamethick.c_str(), histonamethick.c_str(), 100, 0., 2.));
  }
  std::cout << " artg4tk::CheckDRCalorimeterHits: The name of the used reference physics list is: "
            << pstl.get<std::string>("PhysicsListName") << std::endl;

} // end beginJob

void
artg4tk::CheckDRCalorimeterHits::analyze(const art::Event& event)
{
  typedef std::vector<art::Handle<DRCalorimeterHitCollection>> DRHandleVector;
  // DRHandleVector allDRSims;
  double sumDRE = 0;
  double sumNCeren = 0.0;
  unsigned int numz = 10;
  std::vector<double> edepthin;
  std::vector<double> edepthick;
  std::vector<double> ncerenthin;
  std::vector<double> ncerenthick;
  for (unsigned int ijk = 0; ijk < numz; ijk++) {
    edepthin.push_back(0.0);
    edepthick.push_back(0.0);
    ncerenthin.push_back(0.0);
    ncerenthick.push_back(0.0);
  }
  // event.getManyByType(allDRSims);
  auto allDRSims = event.getMany<DRCalorimeterHitCollection>();
  for (DRHandleVector::const_iterator i = allDRSims.begin(); i != allDRSims.end(); ++i) {
    art::Handle<DRCalorimeterHitCollection> ih = *i;
    auto const* prov = ih.provenance();
    string iname = prov->productInstanceName();
    string pname = prov->processName();
    std::cout << "iname:  " << iname << std::endl;
    std::cout << "pname:  " << pname << std::endl;
    fhicl::ParameterSet ps = prov->parameterSet();
    std::vector<std::string> names = ps.get_names();

    for (vector<string>::iterator t = names.begin(); t != names.end(); ++t) {
      std::cout << *t << std::endl;
    }
    const DRCalorimeterHitCollection& DRsims(**i);
    _hnDRHits->Fill(DRsims.size());
    for (DRCalorimeterHitCollection::const_iterator j = DRsims.begin(); j != DRsims.end(); ++j) {
      const DRCalorimeterHit& hit = *j;
      unsigned int lay = (int)(hit.GetID() / (numz * numz));
      // unsigned int jj = (int)(((hit.GetID()-kk*(numz*numz)))/numy);
      // unsigned int ii = (int)(hit.GetID()-kk*(numz*numz)-jj*numy);
      // cout << "ID:  "<<hit.GetID()<<"  Layer:  "<< kk<<"  Column:  " << jj<< "  row:  "<<
      // ii<<endl;
      sumDRE = sumDRE + hit.GetEdep();
      sumNCeren = sumNCeren + hit.GetNceren();
      if (iname.find("thin") != std::string::npos) {
        edepthin[lay] = edepthin[lay] + hit.GetEdep();
        ncerenthin[lay] = ncerenthin[lay] + hit.GetNceren();
      } else {
        edepthick[lay] = edepthick[lay] + hit.GetEdep();
        ncerenthick[lay] = ncerenthick[lay] + hit.GetNceren();
      }
    }
  }
  _hDREdep->Fill(sumDRE / CLHEP::GeV);
  _hNCeren->Fill(sumNCeren);
  _hEdepvsNCeren->Fill(sumDRE / CLHEP::GeV, sumNCeren);
  for (unsigned int ijk = 0; ijk < numz; ijk++) {
    vecofhistosthin[ijk]->Fill(edepthin[ijk] / CLHEP::GeV);
    vecofhistosthick[ijk]->Fill(edepthick[ijk] / CLHEP::GeV);
  }
  _ntuple->Fill(event.event(),
                edepthin[0],
                edepthin[1],
                edepthin[2],
                edepthin[3],
                edepthin[4],
                edepthin[5],
                edepthin[6],
                edepthin[7],
                edepthin[8],
                edepthin[9]);
  _ntuple2->Fill(event.event(),
                 ncerenthin.at(0),
                 ncerenthin.at(1),
                 ncerenthin.at(2),
                 ncerenthin.at(3),
                 ncerenthin.at(4),
                 ncerenthin.at(5),
                 ncerenthin.at(6),
                 ncerenthin.at(7),
                 ncerenthin.at(8),
                 ncerenthin.at(9));
  typedef std::vector<art::Handle<ByParticle>> EdepHandleVector;
  // EdepHandleVector allEdeps;
  // event.getManyByType(allEdeps);
  auto allEdeps = event.getMany<ByParticle>();
  bool first = true;
  ByParticle addup;
  ByParticle addupnc;
  double junkie = 0.0;
  for (EdepHandleVector::const_iterator i = allEdeps.begin(); i != allEdeps.end(); ++i) {
    art::Handle<ByParticle> ih = *i;
    auto const* prov = ih.provenance();
    string instancename = prov->productInstanceName();

    if (instancename.find("NCeren") != std::string::npos) {
      const ByParticle& Edeps(**i);
      if (first) {
        addupnc = Edeps;
        junkie = Edeps.at("NCerentot");
        first = false;
      } else {
        junkie = junkie + Edeps.at("NCerentot");
        for (std::map<std::string, double>::const_iterator it = Edeps.begin(); it != Edeps.end();
             ++it) {
          addupnc[it->first] = addupnc[it->first] + it->second;
        }
      }
    } else if (instancename.find("Edep") != std::string::npos) {
      const ByParticle& Edeps(**i);
      if (first) {
        addup = Edeps;
        junkie = Edeps.at("Etot");
        first = false;
      } else {
        junkie = junkie + Edeps.at("Etot");
        for (std::map<std::string, double>::const_iterator it = Edeps.begin(); it != Edeps.end();
             ++it) {
          addup[it->first] = addup[it->first] + it->second;
        }
      }
      /*
        for (std::map<std::string, double>::const_iterator it = Edeps.begin(); it != Edeps.end();
        ++it) {

        if (mapofhistos.find(it->first) != mapofhistos.end())
        mapofhistos[it->first]->Fill((100.*it->second)/junkie);
        }
      */
    }
  }
  for (std::map<std::string, double>::const_iterator it = addup.begin(); it != addup.end(); ++it) {
    if (mapofhistos.find(it->first) != mapofhistos.end())
      mapofhistos[it->first]->Fill((100. * it->second) / junkie);
  }
  for (std::map<std::string, double>::const_iterator it = addupnc.begin(); it != addupnc.end();
       ++it) {
    if (ncmapofhistos.find(it->first) != ncmapofhistos.end())
      ncmapofhistos[it->first]->Fill((100. * it->second) / junkie);
  }
} // end analyze

DEFINE_ART_MODULE(artg4tk::CheckDRCalorimeterHits)
