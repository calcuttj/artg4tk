
#include "artg4tk/G4PhysModelParamStudy/AnalysisBase/ModelParamAnalyzerBase.hh"

#include "art/Framework/Core/ModuleMacros.h"

#include "art/Framework/Principal/Event.h"

// Run/Eevent data products
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkVtx.hh"        // Event data product

#include "artg4tk/G4PhysModelParamStudy/StatAnalysisTools/Chi2Calc.hh"
#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/RecordChi2.hh"

// Root-specific headers
#include "TH1D.h"
#include "TProfile.h"

#include "CLHEP/Units/SystemOfUnits.h"

#include "art/Framework/Services/Optional/TFileService.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <memory>

namespace artg4tk {

   class AnalyzerNA49 : public ModelParamAnalyzerBase {

   public:
   
      explicit AnalyzerNA49( const fhicl::ParameterSet& );
      virtual ~AnalyzerNA49();
      
      virtual void analyze( const art::Event& event ) override;
      virtual void beginJob()                         override;
      virtual void endJob()                           override;
   
   protected:

      virtual TH1* matchExpSpectrum2MC( const int&, const std::vector<std::string>&, const int& ); 
         
   private:
   
      double calculateBinWeight( const CLHEP::HepLorentzVector& labp, const std::string& pname, 
                                 double Ekin, double mass, double pT, int xFbin, double sqrt_s );

      std::string extractXFBinFromTitle( const std::string& );
      
      void calculateChi2();
      
      
      TH1D*              fHistoNSec;

      int     fNPBinsXF;
      double* fPBinsXF;
      int     fNPBinsPT;
      double* fPBinsPT;

      int                fNPiBinsXF;
      double*            fPiBinsXF;
      int                fNPiBinsPT;
      double*            fPiBinsPT;

      std::vector<TH1D*> fHistoSecProton;
      std::vector<TH1D*> fHistoSecAntiProton;
      std::vector<TH1D*> fHistoSecPiMinus; 
      std::vector<TH1D*> fHistoSecPiPlus; 
      std::vector<TH1D*> fHistoSecNeutron;   

      std::vector<TH1D*> fHistoPTProton;
      std::vector<TH1D*> fHistoPTPiMinus;
      std::vector<TH1D*> fHistoPTPiPlus;
      
      Chi2Calc*          fChi2Calc;
 
   };  

}

artg4tk::AnalyzerNA49::AnalyzerNA49( const fhicl::ParameterSet& p )
  : artg4tk::ModelParamAnalyzerBase(p),
    fNPBinsXF(0), fNPBinsPT(0), fNPiBinsXF(0), fNPiBinsPT(0),
    fChi2Calc(0)
{
   if ( fIncludeExpData ) 
   {
      fChi2Calc = new Chi2Calc();
   }
}

artg4tk::AnalyzerNA49::~AnalyzerNA49()
{
   // do I need any histos delete's here ?!
   // or will TFileService take care of that ?!?!
   
   if ( fChi2Calc ) delete fChi2Calc;
      
}

void artg4tk::AnalyzerNA49::beginJob()
{

   TH1::SetDefaultSumw2();
      
   art::ServiceHandle<art::TFileService> tfs;
   fHistoNSec = tfs->make<TH1D>( "NSec", "Number of secondary per inelastic interaction", 100, 0., 100 );

   std::string hname = "";
   
   // secondary proton
   
   fNPBinsXF = 41; // 40+1 for bin edges  
   fPBinsXF = new double[fNPBinsXF]; 
  
   std::vector<double> bincenter;
   double center = 0.;
  
   for ( int ii=0; ii<15; ++ii )
   {
     center = -0.8 + 0.05*ii;
     bincenter.push_back( center );
   } 
   for (int ii=0; ii<8; ++ii )
   {
     center = -0.1 + 0.025*(ii+1);
     bincenter.push_back( center );
   }
   for ( int ii=0; ii<17; ++ii ) // 17 = fNPBinsXF-1-23, 23 - from previous 2 loops
   {
     center = 0.1 + 0.05*(ii+1);
     bincenter.push_back( center );
   }
    
   fPBinsXF[0] = -0.825;
   for ( int ii=1; ii<fNPBinsXF-1; ++ii )
   {
     fPBinsXF[ii] = (bincenter[ii-1]+bincenter[ii])/2.;
   }
   fPBinsXF[fNPBinsXF-1] = fPBinsXF[fNPBinsXF-2] + 0.05;
  
   bincenter.clear();
     
   fNPBinsPT = 20; // 19+1 for edges
   for ( int ii=0; ii<10; ++ii )
   {
     center = 0.05 + ii*0.05;
     bincenter.push_back( center );
   }
   for ( int ii=0; ii<4; ++ii )
   {
     center = 0.6 + 0.1*ii;
     bincenter.push_back( center );
   }
   for ( int ii=0; ii<5; ++ii )
   {
     center = 1.1 + ii*0.2;
     bincenter.push_back( center ); 
   }
    
   fPBinsPT = new double[fNPBinsPT];
   fPBinsPT[0] = 0.025;
   for ( int ii=1; ii<fNPBinsPT-1; ++ii )
   {
     fPBinsPT[ii] = (bincenter[ii-1]+bincenter[ii])/2.;
   }
   fPBinsPT[fNPBinsPT-1] = fPBinsPT[fNPBinsPT-2] + 0.2;
    
   bincenter.clear();

   // booking histos for integrated ** proton ** spectra

   fHistoSecProton.push_back( tfs->make<TH1D>( "proton_dNdxF",   "Integrated", fNPBinsXF-1, fPBinsXF ) );
   fHistoSecProton.push_back( tfs->make<TProfile>( "proton_pT",  "Integrated", fNPBinsXF-1, fPBinsXF, 0., 10. ) );
   
   // booking histos for double diff. pt ** proton ** spectra
   
   for ( int nb=0; nb<fNPBinsXF-1; ++nb ) // FIXME !!! fNPBinsXF is the actual # of bins
                                          // while all other are N+1 !!!
   {
     double xF = (fPBinsXF[nb]+fPBinsXF[nb+1])/2.;
     std::ostringstream osXF;
     osXF << xF;
     std::ostringstream osBin;
     osBin << nb;
     std::string title = "xF = " + osXF.str();
     hname = "proton_pT_DDIFF_" + osBin.str();
     fHistoPTProton.push_back( tfs->make<TH1D>( hname.c_str(), title.c_str(), fNPBinsPT-1, fPBinsPT ) );     
   }
   
   // secondary anti-proton
   
   // we specify bin left edge, to make the bin center match the exp.data
   //
   double pbarbins[] = { -0.55, -0.45, -0.35, -0.25, -0.175, -0.125, -0.0875, -0.0625, -0.0375, 
                         -0.0125,
			 0.0125, 0.0375, 0.075, 0.125, 0.175, 0.25, 0.35, 0.45, 0.55 };
   int npbarbins = sizeof(pbarbins) / sizeof(double) - 1;

   fHistoSecAntiProton.push_back( tfs->make<TH1D>( "antiproton_dNdxF",  "Integrated", npbarbins, pbarbins ) );	
   fHistoSecAntiProton.push_back( tfs->make<TProfile>( "antiproton_pT", "Integrated", npbarbins, pbarbins, 0., 10. ) );	
   
   // secondary pions

  fNPiBinsXF = 30;
  fPiBinsXF = new double[fNPiBinsXF];  
  fPiBinsXF[0] = -0.550;
  fPiBinsXF[1] = -0.450;
  fPiBinsXF[2] = -0.350; 
  fPiBinsXF[3] = -0.275;
  fPiBinsXF[4] = -0.225;
  fPiBinsXF[5] = -0.175;
  fPiBinsXF[6] = -0.1375;
  fPiBinsXF[7] = -0.1125;
  fPiBinsXF[8] = -0.0875; 
  fPiBinsXF[9] = -0.0625;
  fPiBinsXF[10] = -0.045;
  fPiBinsXF[11] = -0.035;
  fPiBinsXF[12] = -0.025;
  fPiBinsXF[13] = -0.015; 
  fPiBinsXF[14] = -0.005;
  fPiBinsXF[15] =  0.005;  
  fPiBinsXF[16] =  0.015;
  fPiBinsXF[17] =  0.025;
  fPiBinsXF[18] =  0.035;  
  fPiBinsXF[19] =  0.045;
  fPiBinsXF[20] =  0.0625;  
  fPiBinsXF[21] =  0.0875;
  fPiBinsXF[22] =  0.1125;
  fPiBinsXF[23] =  0.1375;
  fPiBinsXF[24] =  0.175; 
  fPiBinsXF[25] =  0.225;
  fPiBinsXF[26] =  0.275;
  fPiBinsXF[27] =  0.350;
  fPiBinsXF[28] =  0.450;
  fPiBinsXF[29] =  0.55;
  
  std::vector<std::string> PiXFLabel;
  PiXFLabel.push_back( "-0.500" );
  PiXFLabel.push_back( "-0.400" );
  PiXFLabel.push_back( "-0.300" );
  PiXFLabel.push_back( "-0.250" );
  PiXFLabel.push_back( "-0.150" );
  PiXFLabel.push_back( "-0.125" );
  PiXFLabel.push_back( "-0.100" );
  PiXFLabel.push_back( "-0.075" );
  PiXFLabel.push_back( "-0.050" );
  PiXFLabel.push_back( "-0.040" );
  PiXFLabel.push_back( "-0.030" );
  PiXFLabel.push_back( "-0.025" );
  PiXFLabel.push_back( "-0.020" );
  PiXFLabel.push_back( "-0.010" );
  PiXFLabel.push_back(  "0.000" );
  PiXFLabel.push_back(  "0.010" );
  PiXFLabel.push_back(  "0.020" );
  PiXFLabel.push_back(  "0.025" );
  PiXFLabel.push_back(  "0.030" );
  PiXFLabel.push_back(  "0.040" );
  PiXFLabel.push_back(  "0.050" );
  // ---> PiXFLabel.push_back(  "0.060" );
  PiXFLabel.push_back(  "0.075" );
  PiXFLabel.push_back(  "0.100" );
  PiXFLabel.push_back(  "0.125" );
  PiXFLabel.push_back(  "0.150" );
  PiXFLabel.push_back(  "0.200" );
  PiXFLabel.push_back(  "0.250" );
  PiXFLabel.push_back(  "0.300" );
  PiXFLabel.push_back(  "0.400" );
  PiXFLabel.push_back(  "0.500" );
    
  
  fNPiBinsPT = 17;
  fPiBinsPT = new double[fNPiBinsPT];
  fPiBinsPT[0] = 0.025;
  fPiBinsPT[1] = 0.075;
  fPiBinsPT[2] = 0.125;
  fPiBinsPT[3] = 0.175;
  fPiBinsPT[4] = 0.225;
  fPiBinsPT[5] = 0.275;
  fPiBinsPT[6] = 0.35;
  fPiBinsPT[7] = 0.45;
  fPiBinsPT[8] = 0.55;
  fPiBinsPT[9] = 0.65;
  fPiBinsPT[10] = 0.75;
  fPiBinsPT[11] = 0.85;
  fPiBinsPT[12] = 0.95; 
  fPiBinsPT[13] = 1.1;
  fPiBinsPT[14] = 1.3;
  fPiBinsPT[15] = 1.5;
  fPiBinsPT[16] = 1.7;

   // book histos for integrated ** pi- ** spectra
   
   fHistoSecPiMinus.push_back( tfs->make<TH1D>( "piminus_dNdxF",   "Integrated", fNPiBinsXF-1, fPiBinsXF ) );
   fHistoSecPiMinus.push_back( tfs->make<TProfile>( "piminus_pT",  "Integrated", fNPiBinsXF-1, fPiBinsXF, 0., 10. ) );

   // book histos for integrated ** pi+ ** spectra
   
   fHistoSecPiPlus.push_back( tfs->make<TH1D>( "piplus_dNdxF",   "Integrated", fNPiBinsXF-1, fPiBinsXF ) );
   fHistoSecPiPlus.push_back( tfs->make<TProfile>( "piplus_pT",  "Integrated", fNPiBinsXF-1, fPiBinsXF, 0., 10. ) );

   // book histos for double diff. pt ** pi-/pi+ ** spectra

   for ( int nb=0; nb<fNPiBinsXF-1; ++nb )
   {
/*
     double xF = (fPiBinsXF[nb]+fPiBinsXF[nb+1])/2.;
     std::ostringstream osXF;
     osXF << xF;
*/
     std::ostringstream osBin;
     osBin << nb;
     // std::string title = " xF = " + osXF.str();
     // std::string title = "xF of secondary particle = " + PiXFLabel[nb];
     std::string title = "xF=" + PiXFLabel[nb];
     hname = "piminus_pT_DDIFF_" + osBin.str();
     fHistoPTPiMinus.push_back( tfs->make<TH1D>( hname.c_str(), title.c_str(), fNPiBinsPT-1, fPiBinsPT ) ); 
     hname = "piplus_pT_DDIFF_" + osBin.str();  
     fHistoPTPiPlus.push_back( tfs->make<TH1D>( hname.c_str(),  title.c_str(), fNPiBinsPT-1, fPiBinsPT ) );     
   }
   
   // booking histos for secondary ** neutron ** integrated spectra

   fHistoSecNeutron.push_back( tfs->make<TH1D>( "neutron_dNdxF",    "Integrated", 10, 0.05, 1.05 ) );	

   return;

}

void artg4tk::AnalyzerNA49::endJob()
{
      
   ModelParamAnalyzerBase::endJob();
   
   if ( !fXSecInit ) return;
   
   if ( fHistoNSec->GetEntries() <= 0 ) return;

   double stat = fHistoNSec->Integral();
   
   fHistoNSec->Scale( (1./stat), "width" );
   
   for ( size_t i=0; i<fHistoSecProton.size(); ++i )   
   {
      if ( i <1 ) fHistoSecProton[i]->Scale(1./((double)stat),"width");
   }
   for ( size_t i=0; i<fHistoPTProton.size(); ++i )
   {
      fHistoPTProton[i]->Scale( fXSecOnTarget/((double)stat) ); // Note: NO scaling with "width" because it had to be E/dP3
                                                     //         which is already taken into account as a weight 
   }

   for ( size_t i=0; i<fHistoSecAntiProton.size(); ++i )   
   {
      if ( i < 1 ) fHistoSecAntiProton[i]->Scale(1./((double)stat),"width");
   }

   for ( size_t i=0; i<fHistoSecPiMinus.size(); ++i )
   {
      if ( i < 1 ) fHistoSecPiMinus[i]->Scale(1./((double)stat),"width");
   }
   for ( size_t i=0; i<fHistoPTPiMinus.size(); ++i )
   {
      fHistoPTPiMinus[i]->Scale( fXSecOnTarget/((double)stat) ); // Note: NO scaling with "width" because it had to be E/dP3
                                                      //       which is already taken into account as a weight 
   }

   for ( size_t i=0; i<fHistoSecPiPlus.size(); ++i )
   {
      if ( i < 1 ) fHistoSecPiPlus[i]->Scale(1./((double)stat),"width");
   }
   for ( size_t i=0; i<fHistoPTPiPlus.size(); ++i )
   {
      fHistoPTPiPlus[i]->Scale( fXSecOnTarget/((double)stat) ); // NO scaling with "width" - see earlier comment for fHistoPTPiMinus
   }

   for ( size_t i=0; i<fHistoSecNeutron.size(); ++i )
   {
     if ( i < 1 ) fHistoSecNeutron[i]->Scale(1./((double)stat),"width");
   }
   
   
   if ( fIncludeExpData )
   {
      bool ok = matchVDBRec2MC( fBTConf.GetBeamPartID(),
                                fBTConf.GetBeamMomentum(),
				fBTConf.GetTargetID() );
      if ( !ok )
      {
         fLogInfo << " ExpData do NOT match any of the MC; NO bechmarking; bail out" ;
	 return;
      }
      
      calculateChi2();
      overlayDataMC();
      
   }
   
   return;

}

void artg4tk::AnalyzerNA49::analyze( const art::Event& e )
{

   art::Handle<ArtG4tkVtx> firstint;
   e.getByLabel( fProdLabel, firstint );
   if ( !firstint.isValid() )
   {
      fLogInfo << " handle to 1st hadronic interaction is NOT valid"; // << std::endl;
      return;
   }
      
   if ( !fXSecInit ) initXSecOnTarget( firstint->GetMaterialName(), firstint->GetIncoming() );
   
   if ( !ensureBeamTgtConfig( e ) ) return;

   int nsec = firstint->GetNumOutcoming();
      
   if ( nsec > 0 ) fHistoNSec->Fill( (double)nsec );

   // FIXME !!!
   // ---> LATER !!! ---> if ( fDoResDecay ) AccountForResDecay( fInteraction );

   double SQRT_S = 17.2;
   
// FIXME !!! Make LabP/LabV business GENERIC !!!
// See also ModelParamAnalyzerBase::initXSecOnTarget !!!
/*
       // if under an agnle, then like this:
       //labv = G4LorentzVector(mom.x()/CLHEP::GeV, mom.y()/CLHEP::GeV, 
       //			      mom.z()/CLHEP::GeV, (e0+mass+amass)/CLHEP::GeV);
    fBeam->SetLabP( G4LorentzVector( 0., 0., 
                                     std::sqrt(partEnergy*(partEnergy+2.0*partMass))/GeV,
                                     (partEnergy+partMass+G4Proton::Proton()->GetPDGMass())/GeV) );
*/
   const ArtG4tkParticle& primary = firstint->GetIncoming();

   double pmom = primary.GetMomentum().vect().mag();
   pmom /= CLHEP::GeV;
   double etot = primary.GetMomentum().e();
   etot /= CLHEP::GeV;
   double mass = std::sqrt( etot*etot - pmom*pmom );

   CLHEP::HepLorentzVector labp( 0., 0., std::sqrt(etot*(etot+2.*mass)), (etot+mass+0.93827) );
   CLHEP::Hep3Vector boostLabp = labp.boostVector();
   
   for ( int ip=0; ip<nsec; ++ip )
   {
   
      const ArtG4tkParticle& sec = firstint->GetOutcoming( ip );

      std::string pname = sec.GetName();
      
      CLHEP::HepLorentzVector mom4 = sec.GetMomentum() / CLHEP::GeV;
      CLHEP::Hep3Vector mom = mom4.vect();
      pmom = mom.mag();      
      etot = mom4.e();
      mass = std::sqrt( etot*etot - pmom*pmom );
      double ekin = etot-mass;

      CLHEP::HepLorentzVector boostSec( mom, etot );
      
      boostSec.boost(-boostLabp);

      double xF  = 2 * (boostSec.z()) / SQRT_S;

      double pT  = mom.perp() ;
      
      if ( pname == "neutron" )
      {
	   fHistoSecNeutron[0]->Fill( xF );
      }
      else if ( pname == "pi-" )
      {
	   fHistoSecPiMinus[0]->Fill( xF );
	   fHistoSecPiMinus[1]->Fill( xF, pT );
	   int nb = -1;
	   for ( int ib=0; ib<fNPiBinsXF-1; ++ib )
	   {
	         if ( xF >= fPiBinsXF[ib] && xF < fPiBinsXF[ib+1] )
		 {
		    nb = ib;
		    break;
		 }
	   }
	   if ( nb == -1 ) continue;
	   // calculate weight as Epart/dP3 (see NA49 papers on the SPS hadrons site at CERN) 
	   double wei = calculateBinWeight( labp, pname, ekin, mass, pT, nb, SQRT_S );
	   fHistoPTPiMinus[nb]->Fill( pT, wei ); 
      }
      else if ( pname == "pi+" )
      {
	   fHistoSecPiPlus[0]->Fill( xF );
	   fHistoSecPiPlus[1]->Fill( xF, pT );
	   int nb = -1;
	   for ( int ib=0; ib<fNPiBinsXF-1; ++ib )
	   {
	         if ( xF > fPiBinsXF[ib] && xF <= fPiBinsXF[ib+1] )
		 {
		    nb = ib;
		    break;
		 }
	   }
	   if ( nb == -1 ) continue;
	   // calculate weight as Epart/dP3 (see NA49 papers on the SPS hadrons site at CERN) 
	   double wei = calculateBinWeight( labp, pname, ekin, mass, pT, nb, SQRT_S );
	   fHistoPTPiPlus[nb]->Fill( pT, wei ); 
      }
      else if ( pname == "proton" )
      {
	   fHistoSecProton[0]->Fill( xF );
	   fHistoSecProton[1]->Fill( xF, pT );
	   int nb = -1;
	   for ( int ib=0; ib<fNPBinsXF; ++ib ) // # number of proton bins are N and the size is N+1, while for pions # is N+1
	                                        // need to make it uniform !!!
	   {
	         if ( xF > fPBinsXF[ib] && xF <= fPBinsXF[ib+1] )
		 {
		    nb = ib;
		    break;
		 }
	   }
	   if ( nb == -1 ) continue;
	   // calculate weight as Epart/dP3 (see NA49 papers on the SPS hadrons site at CERN) 
	   double wei = calculateBinWeight( labp, pname, ekin, mass, pT, nb, SQRT_S );
	   fHistoPTProton[nb]->Fill( pT, wei ); 
      }
      else if ( pname == "anti_proton" )
      {
	   fHistoSecAntiProton[0]->Fill( xF );
	   fHistoSecAntiProton[1]->Fill( xF, pT );
      }
   
   }   
   
   return;

}

TH1* artg4tk::AnalyzerNA49::matchExpSpectrum2MC( const int& secid, 
                                                 const std::vector<std::string>& cond,
						 const int& observable )
{


// WARNING !!! At present, this works ONLY for INTEGRATED SPECTRA
//             Double diff. spectra are graphs, and that functionality will come later !!!


   std::string xfbin = extractXFBinFromTitle( cond[0] );
   
   if ( secid == 211 )
   {
      
      if ( observable == 4 )
      {
         return fHistoSecPiPlus[0];
      }
      else if ( observable == 5 )
      {
         return fHistoSecPiPlus[1];
      }
      else if ( observable == 9 )
      {
          for ( size_t i=0; i<fHistoPTPiPlus.size(); ++i )
	  {
	     std::string htitle = fHistoPTPiPlus[i]->GetTitle();
	     if ( htitle == xfbin ) 
	     {
		return fHistoPTPiPlus[i];
	     }
	  }
      }
      else
      {
         return NULL;
      }
   }
   else if ( secid == -211 )
   {
      if ( observable == 4 )
      {
         return fHistoSecPiMinus[0];
      }
      else if ( observable == 5 )
      {
         return fHistoSecPiMinus[1];
      }
      else
      {
         return NULL;
      }
   }
   else if ( secid == 2212 )
   {
      if ( observable == 4 )
      {
         return fHistoSecProton[0];
      }
      else if ( observable == 5 )
      {
         return fHistoSecProton[1];
      }
      else
      {
         return NULL;
      }
   }
   else if ( secid == -2212 )
   {
      if ( observable == 4 )
      {
	 return fHistoSecAntiProton[0];
      }
      else if ( observable == 5 )
      {
         return fHistoSecAntiProton[1];
      }
      else
      {
         return NULL;
      }
   }
   else if ( secid == 2112 )
   {
      if ( observable == 4 )
      {
         return fHistoSecNeutron[0];
      }
      else
      {
         return NULL;
      }
   }
   
   return NULL;

}

double artg4tk::AnalyzerNA49::calculateBinWeight( const CLHEP::HepLorentzVector& labp, const std::string& pname, 
                                             double Ekin, double mass, double pT, int xFbin, double sqrt_s )
{
   double wei = 1.;
   int pTbin = -1;
   
// FIXME !!! Need to redo this method and make it more generic !!!

   if ( pname == "pi+" || pname == "pi-" )
   {
      for (  int ib=0; ib<fNPiBinsPT; ++ib )
      {
         if ( pT >= fPiBinsPT[ib] && pT < fPiBinsPT[ib+1] )
         {
            pTbin = ib;
	    break;
         }
      }
   }
   else if ( pname == "proton" )
   {
      for (  int ib=0; ib<fNPBinsPT; ++ib )
      {
         if ( pT >= fPBinsPT[ib] && pT < fPBinsPT[ib+1] )
         {
            pTbin = ib;
	    break;
         }
      }      
   }
   
   if ( pTbin == -1 ) return wei;
   
// NOTE: This fragment of code draws inspiration in a similar application
//       originally implemented by Mike Kordosky (W&M/MINERvA/NuMI).
//       Credits go to Mike !!!
//
   double dpT2  = 0.; // fPiBinsPT[pTbin+1]*fPiBinsPT[pTbin+1] - fPiBinsPT[pTbin]*fPiBinsPT[pTbin];
   double pLmin = 0.; // fPiBinsXF[xFbin]*sqrt_s/2.;
   double pLmax = 0.; // fPiBinsXF[xFbin+1]*sqrt_s/2.;

   if ( pname == "pi+" || pname =="pi-" )
   {
      dpT2 = fPiBinsPT[pTbin+1]*fPiBinsPT[pTbin+1] - fPiBinsPT[pTbin]*fPiBinsPT[pTbin];
      pLmin = fPiBinsXF[xFbin]*sqrt_s/2.;
      pLmax = fPiBinsXF[xFbin+1]*sqrt_s/2.;
   }
   else if ( pname == "proton" )
   {
      dpT2 = fPBinsPT[pTbin+1]*fPBinsPT[pTbin+1] - fPBinsPT[pTbin]*fPBinsPT[pTbin];
      pLmin = fPBinsXF[xFbin]*sqrt_s/2.;
      pLmax = fPBinsXF[xFbin+1]*sqrt_s/2.;
   }
   
   double EPCM1 = std::sqrt( (pLmin*pLmin+pT*pT) + mass*mass );
   double EPCM2 = std::sqrt( (pLmax*pLmax+pT*pT) + mass*mass );
   
   double pZmin = labp.boostVector().gamma()*(labp.beta()*EPCM1 + pLmin );
   double pZmax = labp.boostVector().gamma()*(labp.beta()*EPCM2 + pLmax );
   double dP3   = CLHEP::pi * dpT2 *(pZmax-pZmin);

   wei = (Ekin+mass)/dP3;

   return wei;

}

void artg4tk::AnalyzerNA49::calculateChi2()
{

   // FIXME !!!
   // So far it's (practically) identical to the one in AnalyzerHARP.
   // BUT !!!
   // In the future, we may need to distinguish between "Integrated" and "DDIFF" !!!


   art::ServiceHandle<art::TFileService> tfs;  
   std::vector<RecordChi2*> vrchi2;

   std::vector< std::pair<int,TH1*> >::iterator itr = fVDBRecID2MC.begin();
   
   std::string hname = itr->second->GetName();

   std::string secondary = hname.substr( 0, hname.find_first_of("_") );
   
   std::string tmpname = secondary + "_RecordChi2";
   std::string tmptitle = secondary + "-RecordChi2";
   // vrchi2.push_back( tfs->makeAndRegister<RecordChi2>( tmpname, tmptitle, RecordChi2() ) );
   vrchi2.push_back( tfs->make<RecordChi2>() );
   vrchi2.back()->SetName( tmpname.c_str() );
   vrchi2.back()->SetTitle( tmptitle.c_str() );
   gDirectory->Append( vrchi2.back() );
  
   std::map<int,double> chi2ind;
   std::vector< std::pair<std::string,double> >chi2integral;

   double chi2sum = 0.;
   int ndfsum = 0;

   for ( ; itr!=fVDBRecID2MC.end(); ++itr )
   {

      if ( !itr->second )
      {
         fLogInfo << " No matching MC histo for record = " << itr->first; // << std::endl;
	 continue; 
      }
      hname = itr->second->GetName();
      if ( hname.find( secondary ) == std::string::npos || hname.find( secondary ) != 0 ) // change of histo name
      {
         chi2integral.push_back( std::pair<std::string,double>( secondary, chi2sum/ndfsum ) );
	 chi2sum = 0.;
	 ndfsum = 0;
	 secondary = hname.substr( 0, hname.find_first_of("_") );
	 tmpname = secondary + "_RecordChi2";
         tmptitle = secondary + "-RecordChi2";
         vrchi2.push_back( tfs->make<RecordChi2>() );
         vrchi2.back()->SetName( tmpname.c_str() );
         vrchi2.back()->SetTitle( tmptitle.c_str() );
         gDirectory->Append( vrchi2.back() );
      }
            
      std::map<int,std::string>::iterator itrda = fJSONRecords.find( itr->first );
      TH1D* hda = 0;
      
      if ( itrda != fJSONRecords.end() ) 
      {
	 hda = fJSON2Data->Convert2Histo(itrda->second,"tmpdata");
      }
      if ( !hda ) continue;

      int ndf = 0;
      double chi2 = fChi2Calc->Chi2DataMC( hda, itr->second, ndf );
      chi2ind.insert( std::pair<int,double>( itr->first, chi2/ndf ) );
      vrchi2.back()->InsertRecord( itr->first, chi2, (double)ndf, 1. );
      std::vector< std::pair<int,double> > chi2mcbin = fChi2Calc->GetChi2MCBin();
      int nrec = vrchi2.back()->GetNRecords();
      for ( size_t ib=0; ib<chi2mcbin.size(); ++ib )
      {
         vrchi2.back()->AddMCBin2Record( nrec-1, chi2mcbin[ib].first, chi2mcbin[ib].second );
      }
      chi2sum += chi2;
      ndfsum += ndf; 

   }

   // last one
   //
   chi2integral.push_back( std::pair<std::string,double>( secondary, chi2sum/ndfsum ) );

   std::cout << " In directory: " << gDirectory->GetName() << std::endl;
   
   std::cout << " ===== chi2/ndf for individial distributions: " << std::endl; 
   std::map<int,double>::iterator i = chi2ind.begin();
   for ( ; i!=chi2ind.end(); ++i )
   {
      std::cout << " DoSSiER ID = " << i->first << " --> chi2/ndf = " << i->second << std::endl;
   }

/*
   for ( unsigned int ir=0; ir<vrchi2.size(); ++ir )
   {
      vrchi2[ir]->Print();
   }
*/
   std::cout << " ===== Integral chi2/ndf: " << std::endl;
   for ( size_t ii=0; ii<chi2integral.size(); ++ii )
   {
      std::cout << " secondary = " << chi2integral[ii].first << 
                   " --> integral chi2/ndf = " << chi2integral[ii].second << std::endl;
   }

   return;

}

std::string artg4tk::AnalyzerNA49::extractXFBinFromTitle( const std::string& title )
{

   std::string ret = "";
   
   size_t pos = title.find("xF");
   ret = title.substr( pos );

   return ret;

}

using artg4tk::AnalyzerNA49;
DEFINE_ART_MODULE(AnalyzerNA49)
