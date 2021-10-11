#include "artg4tk/G4PhysModelParamStudy/AnalysisBase/ModelParamAnalyzerBase.hh"

#include "art/Framework/Core/ModuleMacros.h"

#include "art/Framework/Principal/Event.h"

// Run/Eevent data products
//
// (1st) HAD interaction product
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkVtx.hh"
//
// "Trk" Hits (actually, it's edep along the track in the designeted volume)
#include "artg4tk/pluginDetectors/gdml/TrackerHit.hh"
//
// Event Generator products
#include "artg4tk/DataProducts/EventGenerators/GenParticle.hh"
#include "artg4tk/DataProducts/EventGenerators/GenParticleCollection.hh"

// Root-specific headers
#include "TGraphErrors.h"
#include "TH1D.h"

#include "CLHEP/Units/SystemOfUnits.h"

#include "art_root_io/TFileService.h"

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

namespace artg4tk {

  class AnalyzerEdepTrk : public ModelParamAnalyzerBase {

  public:
    explicit AnalyzerEdepTrk(const fhicl::ParameterSet&);
    virtual ~AnalyzerEdepTrk();

    void analyze(const art::Event& event) override;
    void beginJob() override;
    void endJob() override;

  private:
    int fEvtStart;
    int fCurrentEvt;
    TH1D* fNSec1stInt;
    TH1D* fHadIntRateVsEKin;
    TH1D* fNEdepHits;
    TH1D* fHShowerNEdepHits;
    TH1D* fEdep;
    TH1D* fHShowerEdep;
    TH1D* fEdepR;
    TH1D* fHShowerEdepR;
    TH1D* fEdepZ;
    TH1D* fHShowerEdepZ;

    TH1D* fExpDataH;
    TGraphErrors* fExpDataG;

    // diagnostics output
    //
    // -->      mf::LogInfo fLogInfo; // now in Base... do I nead a separate one for each analyzer
    // ???
  };

}

artg4tk::AnalyzerEdepTrk::AnalyzerEdepTrk(const fhicl::ParameterSet& p)
  : artg4tk::ModelParamAnalyzerBase(p), fEvtStart(-1), fCurrentEvt(-1)
// fLogInfo("AnalyzerHARP") // well, maybe each module does need its's own logger ???
{

  TH1::SetDefaultSumw2();

  art::ServiceHandle<art::TFileService> tfs;

  std::vector<double> limits;

  limits = p.get<std::vector<double>>("NSecHistoLimits");

  int nbins = (int)(limits[1] - limits[0]);
  fNSec1stInt = tfs->make<TH1D>(
    "NSec1stInt", "Number of secondary per inelastic interaction", nbins, limits[0], limits[1]);
  fNSec1stInt->GetXaxis()->SetTitle("Interaction\'s  Multiplicity (num. of sec.)");
  fNSec1stInt->GetYaxis()->SetTitle("Event Rate");

  limits.clear();
  limits = p.get<std::vector<double>>("HadIntRateHistoLimits");
  nbins = (int)((limits[1] - limits[0]) / 0.01); // binwidth of 10MeV (0.01GeV)
  limits[1] = limits[0] + nbins * 0.01;
  fHadIntRateVsEKin =
    tfs->make<TH1D>("HadIntRateVsEKin", "Had.Inel.Interactions Rate", nbins, limits[0], limits[1]);
  fHadIntRateVsEKin->GetXaxis()->SetTitle("EKin of projectile [GeV]");
  fHadIntRateVsEKin->GetYaxis()->SetTitle("NumHadInteractions / (NumEvtTotal*GeV)");

  limits.clear();
  limits = p.get<std::vector<double>>("NEdepHitsHistoLimits");
  nbins = (int)(limits[1] - limits[0]);
  fNEdepHits =
    tfs->make<TH1D>("NEdepHits", "Number of non-zero Edep per event", nbins, limits[0], limits[1]);
  fNEdepHits->GetXaxis()->SetTitle("Number of Hits (non-zero edep)");
  fNEdepHits->GetYaxis()->SetTitle("Event Rate");
  fHShowerNEdepHits = tfs->make<TH1D>("HShowerNEdepHits",
                                      "Number of non-zero Edep by Had Shower, per event",
                                      nbins,
                                      limits[0],
                                      limits[1]);
  fHShowerNEdepHits->GetXaxis()->SetTitle("Number of Hits (non-zero edep)");
  fHShowerNEdepHits->GetYaxis()->SetTitle("Event Rate");

  limits.clear();
  limits = p.get<std::vector<double>>("EdepHistoLimits");
  nbins = (int)((limits[1] - limits[0]) / 0.01); // make the binwidth always be 10MeV (for now ^_^)
  limits[1] = limits[0] + nbins * 0.01;          // this is a bit of a hack but will work for now...
  fEdep = tfs->make<TH1D>("Edep", "Total Energy Deposit in Volume", nbins, limits[0], limits[1]);
  fEdep->GetXaxis()->SetTitle("Energy Deposit [GeV]");
  fEdep->GetYaxis()->SetTitle("Number of Events");
  fHShowerEdep = tfs->make<TH1D>(
    "HShowerEdep", "Energy Deposit by Had Shower in Volume", nbins, limits[0], limits[1]);
  fHShowerEdep->GetXaxis()->SetTitle("Energy Deposit [GeV]");
  fHShowerEdep->GetYaxis()->SetTitle("Number of Events");

  limits.clear();
  limits = p.get<std::vector<double>>("EdepRHistoLimits");
  nbins = (int)(limits[1] - limits[0]); // in cm, and the binwidth is always 1cm
  fEdepR =
    tfs->make<TH1D>("EdepR", "Total Energy Deposit - Radial Profile", nbins, limits[0], limits[1]);
  fEdepR->GetXaxis()->SetTitle("Distance from Projectile [cm]");
  fEdepR->GetYaxis()->SetTitle("Average Energy Deposit [GeV/cm]");
  fHShowerEdepR = tfs->make<TH1D>(
    "HShowerEdepR", "Energy Deposit by Had Shower - Radial Profile", nbins, limits[0], limits[1]);
  fHShowerEdepR->GetXaxis()->SetTitle("Distance from Projectile [cm]");
  fHShowerEdepR->GetYaxis()->SetTitle("Average Energy Deposit [GeV/cm]");

  limits.clear();
  limits = p.get<std::vector<double>>("EdepZHistoLimits");
  nbins = (int)(limits[1] - limits[0]);
  fEdepZ =
    tfs->make<TH1D>("EdepZ", "Energy Deposit - Longitudinal Profile", nbins, limits[0], limits[1]);
  fEdepZ->GetXaxis()->SetTitle("Global Z-position [cm]");
  fEdepZ->GetYaxis()->SetTitle("Average Energy Deposit [GeV/cm]");
  fHShowerEdepZ = tfs->make<TH1D>("HShowerEdepZ",
                                  "Energy Deposit by Had Shower - Longitudinal Profile",
                                  nbins,
                                  limits[0],
                                  limits[1]);
  fHShowerEdepZ->GetXaxis()->SetTitle("Global Z-position [cm]");
  fHShowerEdepZ->GetYaxis()->SetTitle("Average Energy Deposit [GeV/cm]");
}

artg4tk::AnalyzerEdepTrk::~AnalyzerEdepTrk()
{
  // do I need any delete's here ?!
  // or will TFileService take care of that ?!?!
}

void
artg4tk::AnalyzerEdepTrk::beginJob()
{

  //    _directory = gDirectory;
  //    std::cout << "******************************We are in the directory named: " <<
  //    gDirectory->GetName() << std::endl; _file = gDirectory->GetFile();
  // NOTE: one can get the file directly from TFileService

  return;
}

void
artg4tk::AnalyzerEdepTrk::endJob()
{

  // AnalyzerWithExpDataBase::endJob();
  ModelParamAnalyzerBase::endJob();

  // -->   if ( !fXSecInit ) return;

  if (fNSec1stInt->GetEntries() <= 0)
    return;

  double scale = 1.;

  double norm = fNSec1stInt->Integral();
  scale = 1. / norm;
  fNSec1stInt->Scale(scale, "width");

  assert((fCurrentEvt - fEvtStart) > 0);

  int nevt = fCurrentEvt - fEvtStart;
  if (fEvtStart > 0)
    nevt += 1;
  scale = 1. / (double)nevt;
  fHadIntRateVsEKin->Scale(scale, "width");

  norm = fNEdepHits->Integral();
  if (norm <= 0.)
    return;

  scale = 1. / norm;
  fNEdepHits->Scale(scale, "width");
  fEdep->Scale(scale, "width");
  fEdepR->Scale(scale, "width");
  fEdepZ->Scale(scale, "width");

  norm = fHShowerNEdepHits->Integral();
  if (norm <= 0.)
    return;

  scale = 1. / norm;
  fHShowerNEdepHits->Scale(scale, "width");
  fHShowerEdep->Scale(scale, "width");
  fHShowerEdepR->Scale(scale, "width");
  fHShowerEdepZ->Scale(scale, "width");

  return;
}

void
artg4tk::AnalyzerEdepTrk::analyze(const art::Event& e)
{

  // Keep counters for further normalization
  //
  if (fEvtStart < 0)
    fEvtStart = e.id().event();
  fCurrentEvt = e.id().event();

  art::Handle<GenParticleCollection> primgenparts;
  e.getByLabel("PrimaryGenerator", primgenparts);
  if (!primgenparts.isValid()) {
    fLogInfo << " primgen handle is NOT valid "; //  << std::endl;
    return;
  }

  art::Handle<ArtG4tkVtx> firstint;
  e.getByLabel(fProdLabel, firstint);
  if (!firstint.isValid()) {
    fLogInfo << " handle to 1st hadronic interaction is NOT valid"; // << std::endl;
                                                                    //      return;
  } else {
    int nsec = firstint->GetNumOutcoming();
    if (nsec > 0) {
      fNSec1stInt->Fill((double)nsec);
      const ArtG4tkParticle& inparticle = firstint->GetIncoming();
      double inetot = inparticle.GetMomentum().e() / CLHEP::GeV;
      double inp = inparticle.GetMomentum().vect().mag() / CLHEP::GeV;
      double mass = std::sqrt(inetot * inetot - inp * inp);
      double inekin = inetot - mass; // already in GeV
      fHadIntRateVsEKin->Fill(inekin);
    }
  }

  art::Handle<TrackerHitCollection> edeptrkhits;
  e.getByLabel(fProdLabel, edeptrkhits);
  if (!edeptrkhits.isValid()) {
    fLogInfo << " handle to traking hits (edep in LAr) is NOT valid"; // << std::endl;
  } else {
    int nhits = edeptrkhits->size();
    if (nhits > 0) {
      fNEdepHits->Fill((double)nhits);
      if (firstint.isValid())
        fHShowerNEdepHits->Fill((double)nhits);
      double esum = 0.;
      GenParticleCollection::const_iterator i =
        primgenparts->begin(); // HACK: take the 1st particle in the list
      for (int ih = 0; ih < nhits; ++ih) {
        double edep = ((*edeptrkhits)[ih].Edep) / CLHEP::GeV;
        esum += edep;
        double xedep = i->position().x() - ((*edeptrkhits)[ih].xpos) / CLHEP::cm;
        double yedep = i->position().y() - ((*edeptrkhits)[ih].ypos) / CLHEP::cm;
        double redep = std::sqrt(xedep * xedep + yedep * yedep);
        fEdepR->Fill(redep, edep);
        if (firstint.isValid())
          fHShowerEdepR->Fill(redep, edep);
        double zedep = ((*edeptrkhits)[ih].zpos) / CLHEP::cm;
        fEdepZ->Fill(zedep, edep);
        if (firstint.isValid())
          fHShowerEdepZ->Fill(zedep, edep);
      }
      fEdep->Fill(esum);
      if (firstint.isValid())
        fHShowerEdep->Fill(esum);
    }
  }

  return;
}

DEFINE_ART_MODULE(artg4tk::AnalyzerEdepTrk)
