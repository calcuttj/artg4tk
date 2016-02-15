
#include "artg4tk/G4PhysModelParamStudy/AnalysisBase/ModelParamAnalyzerBase.hh"

#include "art/Framework/Core/ModuleMacros.h"

#include "art/Framework/Principal/Event.h"

// Run/Eevent data products
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkVtx.hh"        // Event data product

// Root-specific headers
#include "TH1D.h"

#include "CLHEP/Units/SystemOfUnits.h"

#include "art/Framework/Services/Optional/TFileService.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <memory>

namespace artg4tk {

   class AnalyzerITEP : public ModelParamAnalyzerBase {
   
   public:
   
      explicit AnalyzerITEP( const fhicl::ParameterSet& );
      virtual ~AnalyzerITEP();
      
      virtual void analyze( const art::Event& event ) override;
      virtual void beginJob()                         override;
      virtual void endJob()                           override;
         
   private:
            
      int    fNumProtonKEBins, fNumNeutronKEBins;
      
      double* fProtonKEBins;
      double* fNeutronKEBins;
 
      // "leftover" from old impl.- for conveninece convenience
      //
      double  fDeltaTheta;      
      std::vector<double> /* fKECBin, emin, emax, */ fThetaCBin, fCosThetaMin, fCosThetaMax, fDeltaCosTheta;

      TH1D*                 fNSec;
      std::vector<TH1D*>    fProtonKE,  fProtonCT ;
      std::vector<TH1D*>    fNeutronKE, fNeutronCT ;
      
      // diagnostics output
      //
// -->      mf::LogInfo fLogInfo; // now in Base... do I nead a separate one for each analyzer ???
        
   };

}

artg4tk::AnalyzerITEP::AnalyzerITEP( const fhicl::ParameterSet& p )
  : artg4tk::ModelParamAnalyzerBase(p) 
    // fLogInfo("AnalyzerITEP") // well, maybe each module does need its's own logger ???
{
  
   fNumProtonKEBins = 13;
   fNumNeutronKEBins = 16;				 
   
   fProtonKEBins = new double[fNumProtonKEBins+1];
   fProtonKEBins[0] = 0.;
   for ( int ike=1; ike<=fNumProtonKEBins; ++ike )
   {
      fProtonKEBins[ike] = fProtonKEBins[ike-1] + 0.02; 
   }
   
   fNeutronKEBins = new double[fNumNeutronKEBins+1];
   fNeutronKEBins[0] = 0.012;
   for ( int ike=1; ike<5; ++ike )
   {
      fNeutronKEBins[ike] = fNeutronKEBins[ike-1] + 0.002;
   }
   fNeutronKEBins[5] = 0.024;
   fNeutronKEBins[6] = 0.030;
   for ( int ike=7; ike<=9; ++ike )
   {
      fNeutronKEBins[ike] = fNeutronKEBins[ike-1] + 0.010;
   }
   for ( int ike=10; ike<=fNumNeutronKEBins; ++ike )
   {
      fNeutronKEBins[ike] = fNeutronKEBins[ike-1] + 0.020;
   }
      
   fDeltaTheta = 4.0*CLHEP::deg; // this is an "artificial" bin width 
                                 // although this is realistic/practical enough
				 
   // theta's (in deg) correspond to the actual measurements ("center bin"),
   // plus there're maybe several extra points (MC only)
   //
   fThetaCBin.push_back(10.1*CLHEP::deg);
   fThetaCBin.push_back(15.0*CLHEP::deg);
   fThetaCBin.push_back(19.8*CLHEP::deg);
   fThetaCBin.push_back(24.8*CLHEP::deg);
   fThetaCBin.push_back(29.5*CLHEP::deg);
   fThetaCBin.push_back(34.6*CLHEP::deg);
   fThetaCBin.push_back(39.6*CLHEP::deg);
   fThetaCBin.push_back(44.3*CLHEP::deg);
   fThetaCBin.push_back(49.3*CLHEP::deg);
   fThetaCBin.push_back(54.2*CLHEP::deg);
   fThetaCBin.push_back(59.1*CLHEP::deg);
   fThetaCBin.push_back(64.1*CLHEP::deg);
   fThetaCBin.push_back(69.1*CLHEP::deg);
   fThetaCBin.push_back(74.1*CLHEP::deg);
   fThetaCBin.push_back(79.1*CLHEP::deg);
   fThetaCBin.push_back(84.1*CLHEP::deg);
   fThetaCBin.push_back(89.0*CLHEP::deg);
   fThetaCBin.push_back(98.9*CLHEP::deg);
   fThetaCBin.push_back(108.9*CLHEP::deg);
   fThetaCBin.push_back(119.0*CLHEP::deg);
   fThetaCBin.push_back(129.1*CLHEP::deg);
   fThetaCBin.push_back(139.1*CLHEP::deg);
   fThetaCBin.push_back(149.3*CLHEP::deg);
   fThetaCBin.push_back(159.6*CLHEP::deg);
   fThetaCBin.push_back(161.4*CLHEP::deg);
   fThetaCBin.push_back(165.5*CLHEP::deg);
   fThetaCBin.push_back(169.5*CLHEP::deg);
   fThetaCBin.push_back(173.5*CLHEP::deg);
   fThetaCBin.push_back(177.0*CLHEP::deg);

   for (unsigned int ii=0; ii<fThetaCBin.size(); ii++)
   {
      double cth1 = std::cos(std::min(fThetaCBin[ii]+fDeltaTheta,180*CLHEP::deg));
      double cth2 = std::cos(std::max(fThetaCBin[ii]-fDeltaTheta,0.0*CLHEP::deg));
      fDeltaCosTheta.push_back(std::abs(cth1-cth2));
      fCosThetaMin.push_back(std::min(cth1,cth2));
      fCosThetaMax.push_back(std::max(cth1,cth2));
/*
      std::cout << " ii = " << ii << ", dcth = " << fDeltaCosTheta[ii] 
                << ", cthmin = " << fCosThetaMin[ii]
		<< ", cthmax = " << fCosThetaMax[ii] << std::endl; 
*/
   }

}

artg4tk::AnalyzerITEP::~AnalyzerITEP()
{
   delete fProtonKEBins;
   delete fNeutronKEBins;
   // do I need to delete HISTOGRAMS here ?!
   // or will TFileService take care of that ?!?!
}

void artg4tk::AnalyzerITEP::beginJob()
{

   TH1::SetDefaultSumw2();
      
   art::ServiceHandle<art::TFileService> tfs;
   fNSec = tfs->make<TH1D>( "NSec", "Number of secondary per inelastic interaction", 100, 0., 100 );
         
   std::string name = "";
   std::string title = ""; // FIXME LATER !!!

/*   
   std::cout << " ====== CROSS-CHECKS ====== " << std::endl;
   std::cout << " fNumProtonKEBins = " << fNumProtonKEBins << std::endl;
   for ( int i=0; i<=fNumProtonKEBins; ++i )
   {
      std::cout << " fProtonKEBins[" << i << "] = " << fProtonKEBins[i] << std::endl; 
   }
   std::cout << " fNumNeutronKEBins = " << fNumNeutronKEBins << std::endl;
   for ( int i=0; i<=fNumNeutronKEBins; ++i )
   {
      std::cout << " fNeutronKEBins[" << i << "] = " << fNeutronKEBins[i] << std::endl; 
   }
   std::cout << " =========================== " << std::endl;
*/   
   
   for ( size_t ith=0; ith<fThetaCBin.size(); ++ith )
   {

      std::ostringstream tmp;
      // G4double ang = angles[ii] / CLHEP::deg;
      tmp << ( fThetaCBin[ith] / CLHEP::deg );
      
      name = "proton_at_";
      name += tmp.str();
      name += "deg";
      fProtonKE.push_back( tfs->make<TH1D>( name.c_str(), title.c_str(), fNumProtonKEBins, fProtonKEBins ) );
      
      name = "neutron_at_";
      name += tmp.str();
      name += "deg";
      fNeutronKE.push_back( tfs->make<TH1D>( name.c_str(), title.c_str(), fNumNeutronKEBins, fNeutronKEBins ) ); 

   } 
   
   for ( int ike=0; ike<fNumProtonKEBins; ++ike )
   {
      std::ostringstream tmp;
      tmp << ( 0.5*(fProtonKEBins[ike]+fProtonKEBins[ike+1]) / CLHEP::GeV );
      name = "proton_at_";
      name += tmp.str();
      name += "GeV";
      fProtonCT.push_back( tfs->make<TH1D>( name.c_str(), title.c_str(), 80, -1.0, 1.0 ) );
   } 
   for ( int ike=0; ike<fNumNeutronKEBins; ++ike )
   {
      std::ostringstream tmp;
      tmp << ( 0.5*(fNeutronKEBins[ike]+fNeutronKEBins[ike+1]) / CLHEP::GeV );
      name = "neutron_at_";
      name += tmp.str();
      name += "GeV";
      fNeutronCT.push_back( tfs->make<TH1D>( name.c_str(), title.c_str(), 80, -1.0, 1.0 ) );
   } 

//    _directory = gDirectory;
//    std::cout << "******************************We are in the directory named: " << gDirectory->GetName() << std::endl;
//    _file = gDirectory->GetFile();
// NOTE: one can get the file directly from TFileService 
   
   return;

}

void artg4tk::AnalyzerITEP::endJob()
{
   
   if ( !fXSecInit ) return;
   
   if ( fNSec->GetEntries() <= 0 ) return;

   // here I can also pull up whatever xsec I might need
   // but in order to do so, I might need to store material & beam particle
   
   double xbin = 1.;
   double norm = 1.;
   double scale = 1.;
   
   norm = fNSec->Integral();

/*   
   std::cout << " nevt = " << norm << std::endl;
   std::cout << " xsec = " << fXSecOnTarget << std::endl;
*/
   
   xbin = fNSec->GetBinWidth(1);
   scale = 1. / (xbin*norm);
   fNSec->Scale( scale ) ;
   
   // NOTE: no need to Write each histo as TFileService will do it   

   for ( size_t i=0; i<fProtonKE.size(); ++i )
   {
      scale = fXSecOnTarget / ( norm * 2.*CLHEP::pi*fDeltaCosTheta[i] );
      fProtonKE[i]->Scale( scale, "width" );
   }
   for ( size_t i=0; i<fNeutronKE.size(); ++i )
   {
      scale = fXSecOnTarget / ( norm * 2.*CLHEP::pi*fDeltaCosTheta[i] );
      fNeutronKE[i]->Scale( scale, "width" );
   }

   for ( size_t i=0; i<fProtonCT.size(); ++i )
   {
      scale = fXSecOnTarget / ( norm * 2.*CLHEP::pi*(fProtonKEBins[i+1]-fProtonKEBins[i]) );
      fProtonCT[i]->Scale( scale, "width" );
   }
   for ( size_t i=0; i<fNeutronCT.size(); ++i )
   {
      scale = fXSecOnTarget / ( norm * 2.*CLHEP::pi*(fNeutronKEBins[i+1]-fNeutronKEBins[i]) );
      fNeutronCT[i]->Scale( scale, "width" );
   }
   
   return;

}

void artg4tk::AnalyzerITEP::analyze( const art::Event& e )
{
      
   art::Handle<ArtG4tkVtx> firstint;
   e.getByLabel( fProdLabel, firstint );
   if ( !firstint.isValid() )
   {
      fLogInfo << " handle to 1st hadronic interaction is NOT valid"; // << std::endl;
      return;
   }
   
   if ( !fXSecInit ) initXSecOnTarget( firstint->GetMaterialName(), firstint->GetIncoming() );
   
   // const std::vector<ArtG4tkParticle>& secs = firstint->GetAllOutcoming();
   // int nsec = secs.size();
   int nsec = firstint->GetNumOutcoming();
      
   if ( nsec > 0 ) fNSec->Fill( (double)nsec );
   
   for ( int ip=0; ip<nsec; ++ip )
   {
      
      const ArtG4tkParticle& sec = firstint->GetOutcoming( ip );
      
      std::string pname = sec.GetName();
      
      double pmom = sec.GetMomentum().vect().mag();      
      pmom /= CLHEP::GeV;

      double etot = sec.GetMomentum().e();
      etot /= CLHEP::GeV;
      double mass = std::sqrt( etot*etot - pmom*pmom );
     
      double theta = sec.GetMomentum().vect().theta();
      double cth = std::cos(theta);
      double wt = 1./pmom;

      double ke = etot - mass;
            
      // KE spectra
      //
      for ( size_t i=0; i<fThetaCBin.size(); ++i )
      {
         if ( cth > fCosThetaMin[i] && cth <= fCosThetaMax[i] )
	 {
	    if ( pname == "proton" )
	    {
	       fProtonKE[i]->Fill( ke, wt );
	    }
	    else if ( pname == "neutron" )
	    {
	       fNeutronKE[i]->Fill( ke, wt );
	    }
	 }
      }
      
      // Cos(theta) spectra
      //
      for ( int i=0; i<fNumProtonKEBins; ++i )
      {
         if ( ke > fProtonKEBins[i] && ke <= fProtonKEBins[i+1] )
	 {
	    fProtonCT[i]->Fill(cth,wt);
	 }
      }
      for ( int i=0; i<fNumNeutronKEBins; ++i )
      {
         if ( ke > fNeutronKEBins[i] && ke <= fNeutronKEBins[i+1] )
	 {
	    fNeutronCT[i]->Fill(cth,wt);
	 }
      }

   } // end loop over secondaries
   
   return;
   
}

using artg4tk::AnalyzerITEP;
DEFINE_ART_MODULE(AnalyzerITEP)
