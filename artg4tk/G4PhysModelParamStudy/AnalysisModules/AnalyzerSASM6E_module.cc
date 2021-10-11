#include "artg4tk/G4PhysModelParamStudy/AnalysisBase/ModelParamAnalyzerBase.hh"

#include "art/Framework/Core/ModuleMacros.h"

#include "art/Framework/Principal/Event.h"

// Run/Eevent data products
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkVtx.hh" // Event data product

#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/RecordChi2.hh"
#include "artg4tk/G4PhysModelParamStudy/StatAnalysisTools/Chi2Calc.hh"

// Root-specific headers
#include "TH1D.h"
#include "TProfile.h"

#include "CLHEP/Units/SystemOfUnits.h"

#include "art_root_io/TFileService.h"

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

namespace artg4tk {

  class AnalyzerSASM6E : public ModelParamAnalyzerBase {

  public:
    explicit AnalyzerSASM6E(const fhicl::ParameterSet&);
    virtual ~AnalyzerSASM6E();

    virtual void analyze(const art::Event& event) override;
    virtual void beginJob() override;
    virtual void endJob() override;

  protected:
    virtual TH1* matchExpSpectrum2MC(const int&, const std::vector<std::string>&, const int&);

  private:
    std::string extractPTBinFromTitle(const std::string&);

    void calculateChi2();

    int fNBins;
    double* fBins;
    double fDeltaPt;

    TH1D* fHistoNSec;
    std::vector<TH1D*> fHistoSecPiPlus;
    std::vector<TH1D*> fHistoSecPiMinus;
    std::vector<TH1D*> fHistoSecKPlus;
    std::vector<TH1D*> fHistoSecKMinus;
    std::vector<TH1D*> fHistoSecProton;
    std::vector<TH1D*> fHistoSecAntiProton;

    Chi2Calc* fChi2Calc;
  };

}

artg4tk::AnalyzerSASM6E::AnalyzerSASM6E(const fhicl::ParameterSet& p)
  : artg4tk::ModelParamAnalyzerBase(p), fNBins(7), fBins(0), fDeltaPt(0.1), fChi2Calc(0)
{
  if (fIncludeExpData) {
    fChi2Calc = new Chi2Calc();
  }
}

artg4tk::AnalyzerSASM6E::~AnalyzerSASM6E()
{
  // do I need any histos delete's here ?!
  // or will TFileService take care of that ?!?!

  if (fBins)
    delete[] fBins;
  if (fChi2Calc)
    delete fChi2Calc;
}

void
artg4tk::AnalyzerSASM6E::beginJob()
{

  TH1::SetDefaultSumw2();

  art::ServiceHandle<art::TFileService> tfs;
  fHistoNSec =
    tfs->make<TH1D>("NSec", "Number of secondary per inelastic interaction", 100, 0., 100);

  std::string hname = "";

  fBins = new double[fNBins + 1];
  fBins[0] = 25.;
  for (int i = 1; i < 7; ++i) {
    fBins[i] = fBins[0] + 10. * ((double)i);
  }
  fBins[7] = 91.; // (this will make center of bin at 88, i.e. 85-91)

  fHistoSecPiPlus.push_back(tfs->make<TH1D>("piplus_pt180", "pt=0.18 [GeV/c]", fNBins, fBins));
  fHistoSecPiPlus.push_back(tfs->make<TH1D>("piplus_pt300", "pt=0.3 [GeV/c]", fNBins, fBins));
  fHistoSecPiPlus.push_back(tfs->make<TH1D>("piplus_pt500", "pt=0.5 [GeV/c]", fNBins, fBins));

  fHistoSecPiMinus.push_back(tfs->make<TH1D>("piminus_pt180", "pt=0.18 [GeV/c]", fNBins, fBins));
  fHistoSecPiMinus.push_back(tfs->make<TH1D>("piminus_pt300", "pt=0.3 [GeV/c]", fNBins, fBins));
  fHistoSecPiMinus.push_back(tfs->make<TH1D>("piminus_pt500", "pt=0.5 [GeV/c]", fNBins, fBins));

  fHistoSecKPlus.push_back(tfs->make<TH1D>("kplus_pt180", "pt=0.18 [GeV/c]", fNBins, fBins));
  fHistoSecKPlus.push_back(tfs->make<TH1D>("kplus_pt300", "pt=0.3 [GeV/c]", fNBins, fBins));
  fHistoSecKPlus.push_back(tfs->make<TH1D>("kplus_pt500", "pt=0.5 [GeV/c]", fNBins, fBins));

  fHistoSecKMinus.push_back(tfs->make<TH1D>("kminus_pt180", "pt=0.18 [GeV/c]", fNBins, fBins));
  fHistoSecKMinus.push_back(tfs->make<TH1D>("kminus_pt300", "pt=0.3 [GeV/c]", fNBins, fBins));
  fHistoSecKMinus.push_back(tfs->make<TH1D>("kminus_pt500", "pt=0.5 [GeV/c]", fNBins, fBins));

  fHistoSecProton.push_back(tfs->make<TH1D>("proton_pt180", "pt=0.18 [GeV/c]", fNBins, fBins));
  fHistoSecProton.push_back(tfs->make<TH1D>("proton_pt300", "pt=0.3 [GeV/c]", fNBins, fBins));
  fHistoSecProton.push_back(tfs->make<TH1D>("proton_pt500", "pt=0.5 [GeV/c]", fNBins, fBins));

  fHistoSecAntiProton.push_back(
    tfs->make<TH1D>("antiproton_pt180", "pt=0.18 [GeV/c]", fNBins, fBins));
  fHistoSecAntiProton.push_back(
    tfs->make<TH1D>("antiproton_pt300", "pt=0.3 [GeV/c]", fNBins, fBins));
  fHistoSecAntiProton.push_back(
    tfs->make<TH1D>("antiproton_pt500", "pt=0.5 [GeV/c]", fNBins, fBins));

  return;
}

void
artg4tk::AnalyzerSASM6E::endJob()
{

  ModelParamAnalyzerBase::endJob();

  if (!fXSecInit)
    return;

  if (fHistoNSec->GetEntries() <= 0)
    return;

  double stat = fHistoNSec->Integral();
  double scale = fXSecOnTarget / ((double)stat);

  // NOTE: no need to scale with the width because
  //       it's already taken into account by the dp3 ("cell size")

  for (size_t i = 0; i < fHistoSecPiPlus.size(); ++i) {
    fHistoSecPiPlus[i]->Scale(scale);
  }

  for (size_t i = 0; i < fHistoSecPiMinus.size(); ++i) {
    fHistoSecPiMinus[i]->Scale(scale);
  }

  for (size_t i = 0; i < fHistoSecKPlus.size(); ++i) {
    fHistoSecKPlus[i]->Scale(scale);
  }

  for (size_t i = 0; i < fHistoSecKMinus.size(); ++i) {
    fHistoSecKMinus[i]->Scale(scale);
  }

  for (size_t i = 0; i < fHistoSecProton.size(); ++i) {
    fHistoSecProton[i]->Scale(scale);
  }

  for (size_t i = 0; i < fHistoSecAntiProton.size(); ++i) {
    fHistoSecAntiProton[i]->Scale(scale);
  }

  if (fIncludeExpData) {
    bool ok =
      matchVDBRec2MC(fBTConf.GetBeamPartID(), fBTConf.GetBeamMomentum(), fBTConf.GetTargetID());
    if (!ok) {
      fLogInfo << " ExpData do NOT match any of the MC; NO bechmarking; bail out";
      return;
    }

    calculateChi2();
    overlayDataMC();
  }

  return;
}

void
artg4tk::AnalyzerSASM6E::analyze(const art::Event& e)
{

  art::Handle<ArtG4tkVtx> firstint;
  e.getByLabel(fProdLabel, firstint);
  if (!firstint.isValid()) {
    fLogInfo << " handle to 1st hadronic interaction is NOT valid"; // << std::endl;
    return;
  }

  if (!fXSecInit)
    initXSecOnTarget(firstint->GetMaterialName(), firstint->GetIncoming());

  if (!ensureBeamTgtConfig(e))
    return;

  int nsec = firstint->GetNumOutcoming();

  if (nsec > 0)
    fHistoNSec->Fill((double)nsec);

  // const ArtG4tkParticle& primary = firstint->GetIncoming();

  for (int ip = 0; ip < nsec; ++ip) {

    const ArtG4tkParticle& sec = firstint->GetOutcoming(ip);

    std::string pname = sec.GetName();

    if (pname == "pi+" || pname == "pi-" || pname == "proton" || pname == "anti_proton" ||
        pname == "kaon+" || pname == "kaon-") {

      CLHEP::HepLorentzVector mom4 = sec.GetMomentum() / CLHEP::GeV;
      CLHEP::Hep3Vector mom = mom4.vect();
      double pt = mom.perp();

      int id = -1;

      if (fabs(pt - 0.18) < fDeltaPt / 2.) {
        id = 0;
      } else if (fabs(pt - 0.3) < fDeltaPt / 2.) {
        id = 1;
      } else if (fabs(pt - 0.5) < fDeltaPt / 2.) {
        id = 2;
      }
      if (id == -1)
        continue;

      double plab = mom.mag();

      if (plab < fBins[0])
        continue;
      if (plab > fBins[fNBins])
        continue;

      int ib = -1;
      for (int j = 0; j < fNBins; ++j) {
        if (plab > fBins[j] && plab <= fBins[j + 1]) {
          ib = j;
          break;
        }
      }
      if (ib == -1)
        continue; // nothing found

      double etot = mom4.e();

      // calculate the dp3
      //
      double ptmin = pt - fDeltaPt / 2.;
      double ptmax = pt + fDeltaPt / 2.;
      double dpt2 = ptmax * ptmax - ptmin * ptmin;
      double pzmin = std::sqrt(fBins[ib] * fBins[ib] - ptmax * ptmax);
      double pzmax = std::sqrt(fBins[ib + 1] * fBins[ib + 1] - ptmin * ptmin);
      double dpz = std::fabs(pzmax - pzmin);
      double dp3 = CLHEP::pi * dpz * dpt2;

      double wt = etot / dp3;

      if (pname == "pi+") {
        if (id >= 0 && id < 3)
          fHistoSecPiPlus[id]->Fill(plab, wt);
      }
      if (pname == "pi-") {
        if (id >= 0 && id < 3)
          fHistoSecPiMinus[id]->Fill(plab, wt);
      }
      if (pname == "proton") {
        if (id >= 0 && id < 3)
          fHistoSecProton[id]->Fill(plab, wt);
      }
      if (pname == "anti_proton") {
        if (id >= 0 && id < 3)
          fHistoSecAntiProton[id]->Fill(plab, wt);
      }
      if (pname == "kaon+") {
        if (id >= 0 && id < 3)
          fHistoSecKPlus[id]->Fill(plab, wt);
      }
      if (pname == "kaon-") {
        if (id >= 0 && id < 3)
          fHistoSecKMinus[id]->Fill(plab, wt);
      }
    }
  }

  return;
}

TH1*
artg4tk::AnalyzerSASM6E::matchExpSpectrum2MC(const int& secid,
                                             const std::vector<std::string>& cond,
                                             const int& observable)
{

  std::string ptbin = extractPTBinFromTitle(cond[0]);

  if (secid == 211) {
    for (size_t i = 0; i < fHistoSecPiPlus.size(); ++i) {
      std::string htitle = fHistoSecPiPlus[i]->GetTitle();
      if (htitle == ptbin) {
        return fHistoSecPiPlus[i];
      }
    }
  } else if (secid == -211) {
    for (size_t i = 0; i < fHistoSecPiMinus.size(); ++i) {
      std::string htitle = fHistoSecPiMinus[i]->GetTitle();
      if (htitle == ptbin) {
        return fHistoSecPiMinus[i];
      }
    }
  } else if (secid == 321) {
    for (size_t i = 0; i < fHistoSecKPlus.size(); ++i) {
      std::string htitle = fHistoSecKPlus[i]->GetTitle();
      if (htitle == ptbin) {
        return fHistoSecKPlus[i];
      }
    }
  } else if (secid == -321) {
    for (size_t i = 0; i < fHistoSecKMinus.size(); ++i) {
      std::string htitle = fHistoSecKMinus[i]->GetTitle();
      if (htitle == ptbin) {
        return fHistoSecKMinus[i];
      }
    }
  } else if (secid == 2212) {
    for (size_t i = 0; i < fHistoSecProton.size(); ++i) {
      std::string htitle = fHistoSecProton[i]->GetTitle();
      if (htitle == ptbin) {
        return fHistoSecProton[i];
      }
    }
  } else if (secid == -2212) {
    for (size_t i = 0; i < fHistoSecAntiProton.size(); ++i) {
      std::string htitle = fHistoSecAntiProton[i]->GetTitle();
      if (htitle == ptbin) {
        return fHistoSecAntiProton[i];
      }
    }
  }

  return NULL;
}

void
artg4tk::AnalyzerSASM6E::calculateChi2()
{

  // this code is (almost) identical in all analyzers
  // need to consolidate somehow...

  art::ServiceHandle<art::TFileService> tfs;
  std::vector<RecordChi2*> vrchi2;

  std::vector<std::pair<int, TH1*>>::iterator itr = fVDBRecID2MC.begin();

  std::string hname = itr->second->GetName();

  std::string secondary = hname.substr(0, hname.find_first_of("_"));

  std::string tmpname = secondary + "_RecordChi2";
  std::string tmptitle = secondary + "-RecordChi2";
  // vrchi2.push_back( tfs->makeAndRegister<RecordChi2>( tmpname, tmptitle, RecordChi2() ) );
  vrchi2.push_back(tfs->make<RecordChi2>());
  vrchi2.back()->SetName(tmpname.c_str());
  vrchi2.back()->SetTitle(tmptitle.c_str());
  gDirectory->Append(vrchi2.back());

  std::map<int, double> chi2ind;
  std::vector<std::pair<std::string, double>> chi2integral;

  double chi2sum = 0.;
  int ndfsum = 0;

  for (; itr != fVDBRecID2MC.end(); ++itr) {

    if (!itr->second) {
      fLogInfo << " No matching MC histo for record = " << itr->first; // << std::endl;
      continue;
    }
    hname = itr->second->GetName();

    if (hname.find(secondary) == std::string::npos ||
        hname.find(secondary) != 0) // change of histo name
    {
      chi2integral.push_back(std::pair<std::string, double>(secondary, chi2sum / ndfsum));
      chi2sum = 0.;
      ndfsum = 0;
      secondary = hname.substr(0, hname.find_first_of("_"));
      tmpname = secondary + "_RecordChi2";
      tmptitle = secondary + "-RecordChi2";
      vrchi2.push_back(tfs->make<RecordChi2>());
      vrchi2.back()->SetName(tmpname.c_str());
      vrchi2.back()->SetTitle(tmptitle.c_str());
      gDirectory->Append(vrchi2.back());
    }

    std::map<int, std::string>::iterator itrda = fJSONRecords.find(itr->first);
    TH1D* hda = 0;

    if (itrda != fJSONRecords.end()) {
      hda = fJSON2Data->Convert2Histo(itrda->second, "tmpdata");
    }
    if (!hda)
      continue;

    int ndf = 0;
    double chi2 = fChi2Calc->Chi2DataMC(hda, itr->second, ndf);
    chi2ind.insert(std::pair<int, double>(itr->first, chi2 / ndf));
    vrchi2.back()->InsertRecord(itr->first, chi2, (double)ndf, 1.);
    std::vector<std::pair<int, double>> chi2mcbin = fChi2Calc->GetChi2MCBin();
    int nrec = vrchi2.back()->GetNRecords();
    for (size_t ib = 0; ib < chi2mcbin.size(); ++ib) {
      vrchi2.back()->AddMCBin2Record(nrec - 1, chi2mcbin[ib].first, chi2mcbin[ib].second);
    }
    chi2sum += chi2;
    ndfsum += ndf;
  }

  // last one
  //
  chi2integral.push_back(std::pair<std::string, double>(secondary, chi2sum / ndfsum));

  std::cout << " In directory: " << gDirectory->GetName() << std::endl;

  std::cout << " ===== chi2/ndf for individial distributions: " << std::endl;
  std::map<int, double>::iterator i = chi2ind.begin();
  for (; i != chi2ind.end(); ++i) {
    std::cout << " DoSSiER ID = " << i->first << " --> chi2/ndf = " << i->second << std::endl;
  }

  std::cout << " ===== Integral chi2/ndf: " << std::endl;
  for (size_t ii = 0; ii < chi2integral.size(); ++ii) {
    std::cout << " secondary = " << chi2integral[ii].first
              << " --> integral chi2/ndf = " << chi2integral[ii].second << std::endl;
  }

  return;
}

std::string
artg4tk::AnalyzerSASM6E::extractPTBinFromTitle(const std::string& title)
{

  std::string ret = "";

  size_t pos = title.find("pt");
  ret = title.substr(pos);

  return ret;
}

using artg4tk::AnalyzerSASM6E;
DEFINE_ART_MODULE(AnalyzerSASM6E)
