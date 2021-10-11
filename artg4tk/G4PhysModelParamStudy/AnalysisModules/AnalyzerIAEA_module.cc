

#include "artg4tk/G4PhysModelParamStudy/AnalysisBase/ModelParamAnalyzerBase.hh"

#include "art/Framework/Core/ModuleMacros.h"

#include "art/Framework/Principal/Event.h"

// Run/Eevent data products
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkVtx.hh" // Event data product

#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/RecordChi2.hh"
#include "artg4tk/G4PhysModelParamStudy/StatAnalysisTools/Chi2Calc.hh"

// Root-specific headers
#include "TH1D.h"

#include "CLHEP/Units/SystemOfUnits.h"

#include "art_root_io/TFileService.h"

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

namespace artg4tk {

  class AnalyzerIAEA : public ModelParamAnalyzerBase {

  public:
    explicit AnalyzerIAEA(const fhicl::ParameterSet&);
    virtual ~AnalyzerIAEA();

    virtual void analyze(const art::Event&) override;
    virtual void beginJob() override;
    virtual void endJob() override;

  protected:
    virtual TH1* matchExpSpectrum2MC(const int&, const std::vector<std::string>&, const int&);

  private:
    // FIXME !!!
    // This will be replaced by the use of parnames/parvalues !!!
    //
    std::string extractThetaFromTitle(const std::string&);

    void calculateChi2();

    double fMaxKENeutron;
    int fNBinsTmpKENeutron;
    TH1D* fNSec;
    std::vector<TH1D*> fTmpKENeutron;

    std::vector<double> fTheta;
    std::vector<double> fDeltaCosTh;
    double fDeltaTheta;
    std::vector<double> fCosThMin;
    std::vector<double> fCosThMax;

    Chi2Calc* fChi2Calc;
  };

}

artg4tk::AnalyzerIAEA::AnalyzerIAEA(const fhicl::ParameterSet& p)
  : artg4tk::ModelParamAnalyzerBase(p), fChi2Calc(0)
// fLogInfo("AnalyzerITEP") // well, maybe each module does need its's own logger ???
{

  fMaxKENeutron = p.get<double>("MaxKENeutron", 3000.);
  fNBinsTmpKENeutron = p.get<int>("NBinsTmpKENeutron", 6000);
  fNSec = 0;

  fTheta.push_back(7.5);
  fTheta.push_back(15.);
  fTheta.push_back(30.);
  fTheta.push_back(60.);
  fTheta.push_back(90.);
  fTheta.push_back(120.);
  fTheta.push_back(150.);

  fDeltaTheta = 5.; // degree

  TH1::SetDefaultSumw2();

  for (size_t i = 0; i < fTheta.size(); ++i) {

    double cth1 = std::cos(std::min((fTheta[i] + fDeltaTheta) * CLHEP::deg, 180. * CLHEP::deg));
    double cth2 = std::cos(std::max((fTheta[i] - fDeltaTheta) * CLHEP::deg, 0. * CLHEP::deg));
    fCosThMin.push_back(std::min(cth1, cth2));
    fCosThMax.push_back(std::max(cth1, cth2));
    fDeltaCosTh.push_back(std::abs(cth1 - cth2));

    std::ostringstream os;
    os << fTheta[i];
    std::string hname = "TmpKENeutron" + os.str();
    std::string htitle = "theta=" + os.str() + " [deg]";
    //
    // NOTE (JVY): IAEA data do NOT go beyond 3GeV,
    //             and whatever the dtaset, the smallest bin size is 0.5MeV
    //             that's why 6000/3000. is fair enough
    //
    fTmpKENeutron.push_back(
      new TH1D(hname.c_str(), htitle.c_str(), fNBinsTmpKENeutron, 0., fMaxKENeutron));
  }
}

artg4tk::AnalyzerIAEA::~AnalyzerIAEA()
{

  // do I need to delete here HISTOGRAMS created vis TFileService ?!
  // or will TFileService take care of that ?!?!
  //
  // I think I should delete at least the TMP ones !!!
  //
  for (size_t i = 0; i < fTmpKENeutron.size(); ++i) {
    delete fTmpKENeutron[i];
    fTmpKENeutron[i] = NULL;
  }
  if (fChi2Calc)
    delete fChi2Calc;
}

void
artg4tk::AnalyzerIAEA::beginJob()
{

  TH1::SetDefaultSumw2();

  art::ServiceHandle<art::TFileService> tfs;
  fNSec = tfs->make<TH1D>("NSec", "Number of secondary per inelastic interaction", 100, 0., 100);

  return;
}

void
artg4tk::AnalyzerIAEA::endJob()
{

  //   AnalyzerWithExpDataBase::endJob();
  ModelParamAnalyzerBase::endJob();

  if (!fXSecInit)
    return;

  if (fNSec->GetEntries() <= 0)
    return;

  art::ServiceHandle<art::TFileService> tfs;

  double norm = fNSec->Integral();
  fNSec->Scale((1. / (norm * fNSec->GetBinWidth(1))));

  TH1::SetDefaultSumw2();

  if (fIncludeExpData) {

    // scale MC histos with the xsec and theta-bin
    //
    for (size_t ih = 0; ih < fTmpKENeutron.size(); ++ih) {
      double scale = fXSecOnTarget / (norm * 2. * CLHEP::pi * fDeltaCosTh[ih]);
      fTmpKENeutron[ih]->Scale(scale);
    }

    bool ok =
      matchVDBRec2MC(fBTConf.GetBeamPartID(), fBTConf.GetBeamMomentum(), fBTConf.GetTargetID());

    if (!ok) {
      fLogInfo << " ExpData do NOT match any of the MC; NO bechmarking; bail out";
      return;
    }

    std::vector<std::pair<int, TH1*>>::iterator itr;

    // find and mark up unmatched MC histos (if any);
    // create copies to be written to the Root output file
    //
    std::vector<int> unmatched;
    size_t ih = 0;
    for (; ih < fTmpKENeutron.size(); ++ih) {
      for (itr = fVDBRecID2MC.begin(); itr != fVDBRecID2MC.end(); ++itr) {
        if (fTmpKENeutron[ih] == itr->second)
          break;
      }
      if (itr == fVDBRecID2MC.end())
        unmatched.push_back(ih);
    }
    for (size_t ium = 0; ium < unmatched.size(); ++ium) {
      TH1D* h = fTmpKENeutron[unmatched[ium]];
      TH1D* h1 = tfs->make<TH1D>(*h);
      std::string hname = h->GetName();
      size_t pos = hname.find("Tmp");
      if (pos != std::string::npos)
        hname.erase(pos, std::string("Tmp").length());
      h1->SetName(hname.c_str());
      h1->SetTitle(h->GetTitle());
      h1->Scale(1., "width");
    }

    // now those MC's that are matched to exp.data
    //
    rebinMC2Data("Tmp");

    // and finally re-scale each histo to the EKin bin width
    // NOTE: scaling to the XSec and theta-interval is done earlier
    //
    for (itr = fVDBRecID2MC.begin(); itr != fVDBRecID2MC.end(); ++itr) {
      (itr->second)->Scale(1., "width");
    }

    // TO BE ADDED SHORTLY; the chi2 calc business needs to be implemented !
    // calculateChi2();
    overlayDataMC();

  } else {

    /* --->
          int NEKinBins = 27;
          double* EKinBins = new double[NEKinBins+1];

          EKinBins[0]  =    1.;
          EKinBins[1]  =    1.5;
          EKinBins[2]  =    2.;
          EKinBins[3]  =    3.;
          EKinBins[4]  =    4.;
          EKinBins[5]  =    5.;
          EKinBins[6]  =    6.;
          EKinBins[7]  =    8.;
          EKinBins[8]  =   10.;
          EKinBins[9]  =   15.;
          EKinBins[10] =   20.;
          EKinBins[11] =   30.;
          EKinBins[12] =   40.;
          EKinBins[13] =   50.;
          EKinBins[14] =   60.;
          EKinBins[15] =   80.;
          EKinBins[16] =  100.;
          EKinBins[17] =  150.;
          EKinBins[18] =  200.;
          EKinBins[19] =  300.;
          EKinBins[20] =  400.;
          EKinBins[21] =  500.;
          EKinBins[22] =  600.;
          EKinBins[23] =  800.;
          EKinBins[24] = 1000.;
          EKinBins[25] = 1500.;
          EKinBins[26] = 2000.;
          EKinBins[27] = 3000.;

          double* err2  = new double[NEKinBins];
          double* bcont = new double[NEKinBins];
          for ( int ib=0; ib<NEKinBins; ++ib )
          {
             err2[ib] = 0.;
             bcont[ib] = 0.;
          }
    */
    for (size_t i = 0; i < fTmpKENeutron.size(); ++i) {
      std::string hname = fTmpKENeutron[i]->GetName();
      size_t pos = hname.find("Tmp");
      hname.erase(pos, std::string("Tmp").length());
      TH1D* h = tfs->make<TH1D>(*(fTmpKENeutron[i]));
      h->SetName(hname.c_str());
      double scale = fXSecOnTarget / (norm * 2. * CLHEP::pi * fDeltaCosTh[i]);
      h->Scale(scale, "width");

      /* --->
               for ( int ib=0; ib<NEKinBins; ++ib )
               {
                  err2[ib] = 0.;
                  bcont[ib] = 0.;
               }
               std::string uhname = "Rebin_" + hname;
               TH1D* hh = tfs->make<TH1D>( uhname.c_str(), fTmpKENeutron[i]->GetTitle(), NEKinBins,
         EKinBins ); for ( int ib=1; ib<=fTmpKENeutron[i]->GetNbinsX(); ++ib )
               {
                  double xx  = fTmpKENeutron[i]->GetBinCenter(ib);
                  double yy  = fTmpKENeutron[i]->GetBinContent(ib);
                  double eyy = fTmpKENeutron[i]->GetBinError(ib);
                  int    ibf = hh->FindBin(xx);
                  if ( ibf > 0 && ibf <= NEKinBins )
                  {
                     err2[ibf-1] += eyy*eyy;
                     bcont[ibf-1] += yy;
                  }
               }
               for ( int ib=1; ib<=NEKinBins; ++ib )
               {
                  hh->SetBinContent( ib, bcont[ib-1] );
                  double err = std::sqrt( err2[ib-1] );
                  hh->SetBinError( ib, err );
               }
               hh->Scale( scale, "width" );
      */
    } // end of loop over tmp histos

  } // end of if ( fIncludeExpData )

  return;
}

void
artg4tk::AnalyzerIAEA::analyze(const art::Event& e)
{

  art::Handle<ArtG4tkVtx> firstint;
  e.getByLabel(fProdLabel, firstint);
  if (!firstint.isValid()) {
    fLogInfo << " handle to 1st hadronic interaction is NOT valid"; // << std::endl;
    return;
  }

  if (!fXSecInit)
    initXSecOnTarget(firstint->GetMaterialName(), firstint->GetIncoming());

  int nsec = firstint->GetNumOutcoming();

  if (!ensureBeamTgtConfig(e))
    return;

  if (nsec > 0)
    fNSec->Fill((double)nsec);

  for (int ip = 0; ip < nsec; ++ip) {

    const ArtG4tkParticle& sec = firstint->GetOutcoming(ip);

    std::string pname = sec.GetName();

    double pmom = sec.GetMomentum().vect().mag();
    pmom /= CLHEP::MeV;

    double etot = sec.GetMomentum().e();
    etot /= CLHEP::MeV;
    double mass = std::sqrt(etot * etot - pmom * pmom);

    double theta = sec.GetMomentum().vect().theta();
    double cth = std::cos(theta);

    double ke = etot - mass;

    for (size_t ith = 0; ith < fTheta.size(); ++ith) {
      if (cth > fCosThMin[ith] && cth <= fCosThMax[ith]) {
        if (pname == "neutron") {
          fTmpKENeutron[ith]->Fill(ke);
        }
      }
    }
  }

  return;
}

TH1*
artg4tk::AnalyzerIAEA::matchExpSpectrum2MC(const int& secid,
                                           const std::vector<std::string>& input,
                                           const int&)
{

  // FIXME !!!
  // Redo later with parnames/parvalues !!!
  //
  std::string cond = extractThetaFromTitle(input[0]);
  if (secid == 2112) {
    for (size_t i = 0; i < fTmpKENeutron.size(); ++i) {
      std::string htitle = fTmpKENeutron[i]->GetTitle();
      if (htitle.find(cond) != std::string::npos) {
        return fTmpKENeutron[i];
        break;
      }
    }
  }

  return NULL;
}

std::string
artg4tk::AnalyzerIAEA::extractThetaFromTitle(const std::string& title)
{

  // NOTE (JVY): IDENTICAL to the one in AnalyzerITEP !!!
  //             Need consolidation... although it'll be redone anyway...

  std::string s2find = "theta=";

  size_t pos1 = title.find(s2find);
  pos1 += s2find.length();
  size_t pos2 = title.find(" [deg]");

  std::string ret = title.substr(pos1, pos2 - pos1);

  pos1 = ret.find(".");
  if (ret.compare(pos1 + 1, 1, "0") ==
      0) // a bit "flaky" since there may something like "1.02" but it'll be redone anyway
  {
    ret = ret.substr(0, pos1);
  }

  return ret;
}

void
artg4tk::AnalyzerIAEA::calculateChi2()
{

  return;
}

using artg4tk::AnalyzerIAEA;
DEFINE_ART_MODULE(AnalyzerIAEA)
