
#include "artg4tk/G4PhysModelParamStudy/AnalysisBase/ModelParamAnalyzerBase.hh"

#include "art/Framework/Core/ModuleMacros.h"

#include "art/Framework/Principal/Event.h"

// Run/Eevent data products
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkVtx.hh"        // Event data product

#include "artg4tk/G4PhysModelParamStudy/StatAnalysisTools/Chi2Calc.hh"
#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/RecordChi2.hh"

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
         
   protected:
   
      virtual TH1* matchExpSpectrum2MC( const int&, const std::vector<std::string>& ); 

   private:
            
      // FIXME !!! 
      // This will be replaced by the use of parnames/parvalues !!!
      //
      std::string extractThetaFromTitle( const std::string& );

      void calculateChi2();

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
      
      Chi2Calc*          fChi2Calc; 
                  
      // diagnostics output
      //
// -->      mf::LogInfo fLogInfo; // now in Base... do I nead a separate one for each analyzer ???
        
   };

}

artg4tk::AnalyzerITEP::AnalyzerITEP( const fhicl::ParameterSet& p )
  : artg4tk::ModelParamAnalyzerBase(p),
    fChi2Calc(0) 
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

   if ( fIncludeExpData ) fChi2Calc = new Chi2Calc();

}

artg4tk::AnalyzerITEP::~AnalyzerITEP()
{
   delete fProtonKEBins;
   delete fNeutronKEBins;
   // do I need to delete HISTOGRAMS here ?!
   // or will TFileService take care of that ?!?!
   if ( fChi2Calc ) delete fChi2Calc;
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

      // NOTE: I wish I could use std::to_string but it it makes toooo loooong !
      //
      std::ostringstream tmp;
      tmp << ( fThetaCBin[ith] / CLHEP::deg );
      
      title = "theta=" + tmp.str();
      title += " [deg]";

      name = "proton_at_";
      name += tmp.str();
      name += "deg";
      fProtonKE.push_back( tfs->make<TH1D>( name.c_str(), title.c_str(), fNumProtonKEBins, fProtonKEBins ) );
      
      name = "neutron_at_";
      name += tmp.str();
      name += "deg";
      
      fNeutronKE.push_back( tfs->make<TH1D>( name.c_str(), title.c_str(), fNumNeutronKEBins, fNeutronKEBins ) ); 

   } 
   
   title = "";
   
   for ( int ike=0; ike<fNumProtonKEBins; ++ike )
   {
      std::ostringstream tmp;
      tmp << ( 0.5*(fProtonKEBins[ike]+fProtonKEBins[ike+1]) / CLHEP::GeV );
      
      title = "EKin=" + tmp.str();
      title += "GeV";
      
      name = "proton_at_";
      name += tmp.str();
      name += " [GeV]";
      fProtonCT.push_back( tfs->make<TH1D>( name.c_str(), title.c_str(), 80, -1.0, 1.0 ) );
   } 
   
   title = "";
   
   for ( int ike=0; ike<fNumNeutronKEBins; ++ike )
   {
      std::ostringstream tmp;
      tmp << ( 0.5*(fNeutronKEBins[ike]+fNeutronKEBins[ike+1]) / CLHEP::GeV );
      
      title = "EKin=" + tmp.str();
      title += "GeV";   
      
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
   
//   AnalyzerWithExpDataBase::endJob();
   ModelParamAnalyzerBase::endJob();
   
   if ( !fXSecInit ) return;
   
   if ( fNSec->GetEntries() <= 0 ) return;

   // here I can also pull up whatever xsec I might need
   // but in order to do so, I might need to store material & beam particle
   
   double xbin = 1.;
   double norm = 1.;
   double scale = 1.;
   
   norm = fNSec->Integral();
   
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
   
   if ( fIncludeExpData )
   {
      
      // NOTE: Maybe this fragment can move to the base class.
      //       We may re-scale MC histograms, etc., but the matching
      //       operates on pointers, and those don't change...
      
      bool ok = matchVDBRec2MC( fBTConf.GetBeamPartID(),
                                fBTConf.GetBeamMomentum(),
				fBTConf.GetTargetID() );
      if ( !ok )
      {
         fLogInfo << " ExpData do NOT match any of the MC; NO bechmarking; bail out" ;
	 return;
      }
      
      calculateChi2();
      
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
   
   int nsec = firstint->GetNumOutcoming();
      
   if ( !ensureBeamTgtConfig( e ) ) return;

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

TH1* artg4tk::AnalyzerITEP::matchExpSpectrum2MC( const int& secid, const std::vector<std::string>& input )
{

   // FIXME !!!
   // Redo later with parnames/parvalues !!!
   //
   std::string cond = extractThetaFromTitle( input[0] );
      
   if ( secid == 2212 )
   {
      for ( size_t i=0; i<fProtonKE.size(); ++i )
      {
         std::string htitle = fProtonKE[i]->GetTitle();
	 if ( htitle.find( cond ) != std::string::npos )
	 {
	    return fProtonKE[i];
	    break;
	 }	 
      }
   }
   else if ( secid == 2112 )
   {
      for ( size_t i=0; i<fNeutronKE.size(); ++i )
      {
         std::string htitle = fNeutronKE[i]->GetTitle();
	 if ( htitle.find( cond ) != std::string::npos )
	 {
	    return fNeutronKE[i];
	    break;
	 }	 
      }
   }
         
   return NULL;

}

std::string artg4tk::AnalyzerITEP::extractThetaFromTitle( const std::string& title )
{

   std::string s2find = "theta=";

   size_t pos1 = title.find(s2find);
   pos1 += s2find.length();
   size_t pos2 = title.find(" [deg]");
   
   std::string ret = title.substr( pos1, pos2-pos1 );
   
   pos1 = ret.find(".");
   if ( ret.compare( pos1+1, 1, "0" ) == 0 ) // a bit "flaky" since there may something like "1.02" but it'll be redone anyway
   {
      ret = ret.substr( 0, pos1 );
   }
      
   return ret;

}

void artg4tk::AnalyzerITEP::calculateChi2()
{


   // NOTE & FIXME !!!
   // 
   // This implementation is IDENTICAL to the one of AnalyzerHARP
   // I need to see how to consoludate (move to base class, etc.)
   // So far the only "flaky" thing is that the implementation
   // explicitly assumes that the name of a secondary is hardcoded
   // in the name of a histogram, followd by an underscore, 
   // e.g. piplus_ or proton_

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

      hname = itr->second->GetName();
      if ( hname.find( secondary ) == std::string::npos ) // change of histo name
      {
         chi2integral.push_back( std::pair<std::string,double>( secondary, chi2sum/ndfsum ) );
	 chi2sum = 0.;
	 ndfsum = 0;
	 secondary = hname.substr( 0, hname.find_first_of("_") );
	 tmpname = secondary + "_RecordChi2";
         tmptitle = secondary + "-RecordChi2";
         // vrchi2.push_back( tfs->makeAndRegister<RecordChi2>( tmpname, tmptitle, RecordChi2() ) );
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
      double chi2 = fChi2Calc->Chi2DataMC( itr->second, hda, ndf );
      chi2ind.insert( std::pair<int,double>( itr->first, chi2/ndf ) );
      vrchi2.back()->InsertRecord( itr->first, chi2, (double)ndf );
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
   std::cout << " ===== Now do it again " << std::endl;
   for ( size_t i=0; i<vrchi2.size(); ++i )
   {
      std::cout << " Name = " << vrchi2[i]->GetName() << std::endl;
      vrchi2[i]->Print();
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
using artg4tk::AnalyzerITEP;
DEFINE_ART_MODULE(AnalyzerITEP)
