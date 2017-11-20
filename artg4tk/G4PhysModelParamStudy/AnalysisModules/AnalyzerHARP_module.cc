
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

   class AnalyzerHARP : public ModelParamAnalyzerBase {
   
   public:
   
      explicit AnalyzerHARP( const fhicl::ParameterSet& );
      virtual ~AnalyzerHARP();
      
      virtual void analyze( const art::Event& event ) override;
      virtual void beginJob()                         override;
      virtual void endJob()                           override;
         
   protected:
   
      virtual TH1* matchExpSpectrum2MC( const int&, const std::vector<std::string>&, const int& ); 
   
   private:
   
      // FIXME !!! 
      // This will be replaced by the use of parnames/parvalues !!!
      //
      std::vector<std::string> extractThetaBinFromTitle( const std::string& );
      
      void calculateChi2();
         
      TH1D*              fNSec;
      std::vector<TH1D*> fHistoSecPiMinusFW; 
      std::vector<TH1D*> fHistoSecPiPlusFW; 
      std::vector<TH1D*> fHistoSecPiMinusLA; 
      std::vector<TH1D*> fHistoSecPiPlusLA; 
      std::vector<TH1D*> fHistoSecProtonFW;
      std::vector<TH1D*> fHistoSecProtonLA;
      
      int                fNThetaBinsFW;
      double             fThetaMinFW;
      double             fDeltaThetaFW;   
      int                fNThetaBinsLA;
      double             fThetaMinLA;
      double             fDeltaThetaLA; 
      
      Chi2Calc*          fChi2Calc; 
                  
      // diagnostics output
      //
// -->      mf::LogInfo fLogInfo; // now in Base... do I nead a separate one for each analyzer ???
        
   };

}

artg4tk::AnalyzerHARP::AnalyzerHARP( const fhicl::ParameterSet& p )
  : artg4tk::ModelParamAnalyzerBase(p), 
    fNThetaBinsFW(4), fThetaMinFW(0.05), fDeltaThetaFW(0.05),
    fNThetaBinsLA(9), fThetaMinLA(0.35), fDeltaThetaLA(0.2),
    fChi2Calc(0)
    // fLogInfo("AnalyzerHARP") // well, maybe each module does need its's own logger ???
{

   if ( fIncludeExpData ) 
   {
      fChi2Calc = new Chi2Calc();
   }

}

artg4tk::AnalyzerHARP::~AnalyzerHARP()
{
   // do I need any histos delete's here ?!
   // or will TFileService take care of that ?!?!
   
   if ( fChi2Calc ) delete fChi2Calc;
      
}

void artg4tk::AnalyzerHARP::beginJob()
{

   TH1::SetDefaultSumw2();
      
   art::ServiceHandle<art::TFileService> tfs;
   fNSec = tfs->make<TH1D>( "NSec", "Number of secondary per inelastic interaction", 100, 0., 100 );
   
   double thetaMin = 0.;
   double thetaMax = 0.;
   std::string theta_bin_fw;
   std::string theta_bin_la;

   std::string hname  = "";

   double parbins_fw[] = { 0.5, 1.0, 1.5, 2., 2.5, 3., 3.5, 4., 5., 6.5, 8. };
   int    nparbins_fw = sizeof(parbins_fw) / sizeof(double) - 1;
      
   // NOTE: use of std::to_string(double) results in something like this: 0.1 --> "0.100000" (too much...)
   //       for this reason, use ostringstream to convert float/double to std::string
   //       although using std::to_string(int) looks just fine 
   //
   
   for ( int i=0; i<fNThetaBinsFW; i++ )
   {
      thetaMin = fThetaMinFW + fDeltaThetaFW*i;
      thetaMax = thetaMin + fDeltaThetaFW;
      
      std::ostringstream osTitle1;
      std::ostringstream osTitle2;
      
      osTitle1.clear();
      osTitle1 << thetaMin;
      theta_bin_fw = osTitle1.str() + "<theta<";
      osTitle2.clear();
      osTitle2 << thetaMax;
      theta_bin_fw += osTitle2.str();
      theta_bin_fw += " [rad]";
//      theta_bin_fw = std::to_string(thetaMin) + "<theta<" + std::to_string(thetaMax) + "[rad]";
            
      hname = "piminus_FW_" + std::to_string(i);       
      fHistoSecPiMinusFW.push_back( tfs->make<TH1D>( hname.c_str(), theta_bin_fw.c_str(), nparbins_fw, parbins_fw ) );

      hname = "piplus_FW_" + std::to_string(i);
      fHistoSecPiPlusFW.push_back( tfs->make<TH1D>( hname.c_str(), theta_bin_fw.c_str(), nparbins_fw, parbins_fw ) );

      hname = "proton_FW_" + std::to_string(i);
      fHistoSecProtonFW.push_back( tfs->make<TH1D>( hname.c_str(), theta_bin_fw.c_str(),nparbins_fw, parbins_fw ) ); 
   
   }

   double parbins_la[] = { 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7, 0.8 };
   int    nparbins_la = sizeof(parbins_la) / sizeof(double) - 1;

   for ( int i=0; i<fNThetaBinsLA; i++ )
   {
     
      thetaMin = fThetaMinLA + fDeltaThetaLA*i;
      thetaMax = thetaMin + fDeltaThetaLA; 
     
      std::ostringstream osTitle1;
      std::ostringstream osTitle2;

      osTitle1.clear();
      osTitle1 << thetaMin;
      theta_bin_la = osTitle1.str() + "<theta<";
      osTitle2.clear();
      osTitle2 << thetaMax;
      theta_bin_la += osTitle2.str();
      theta_bin_la += " [rad]";
      
      hname = "piminus_LA_" + std::to_string(i);         
      fHistoSecPiMinusLA.push_back( tfs->make<TH1D>( hname.c_str(), theta_bin_la.c_str(), nparbins_la, parbins_la ) );

      hname = "piplus_LA_" + std::to_string(i);
      fHistoSecPiPlusLA.push_back( tfs->make<TH1D>( hname.c_str(), theta_bin_la.c_str(), nparbins_la, parbins_la ) );

      hname = "proton_LA_" + std::to_string(i);
      fHistoSecProtonLA.push_back( tfs->make<TH1D>( hname.c_str(), theta_bin_la.c_str(), nparbins_la, parbins_la ) );

   }

//    _directory = gDirectory;
//    std::cout << "******************************We are in the directory named: " << gDirectory->GetName() << std::endl;
//    _file = gDirectory->GetFile();
// NOTE: one can get the file directly from TFileService 

   return;

}

void artg4tk::AnalyzerHARP::endJob()
{
      
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

   for ( size_t i=0; i<fHistoSecPiMinusFW.size(); ++i )
   {
      scale = fXSecOnTarget / (norm*fDeltaThetaFW);
      fHistoSecPiMinusFW[i]->Scale(scale,"width");
   }
   for ( size_t i=0; i<fHistoSecPiMinusLA.size(); ++i )
   {
      scale = fXSecOnTarget / (norm*fDeltaThetaLA);
      fHistoSecPiMinusLA[i]->Scale(scale,"width");
   }

   // secondary pi+
   //
   for ( size_t i=0; i<fHistoSecPiPlusFW.size(); ++i )
   {
      scale = fXSecOnTarget / (norm*fDeltaThetaFW);
      fHistoSecPiPlusFW[i]->Scale(scale,"width");
   }
   for ( size_t i=0; i<fHistoSecPiPlusLA.size(); ++i )
   {
      scale = fXSecOnTarget / (norm*fDeltaThetaLA);
      fHistoSecPiPlusLA[i]->Scale(scale,"width");
   }

   // secondary proton
   //
   for ( size_t i=0; i<fHistoSecProtonFW.size(); ++i )
   {
      scale = fXSecOnTarget / (norm*fDeltaThetaFW);
      fHistoSecProtonFW[i]->Scale( scale, "width" );
   }
   for ( size_t i=0; i<fHistoSecProtonLA.size(); ++i )
   {
      scale = fXSecOnTarget / (norm*fDeltaThetaLA);
      fHistoSecProtonLA[i]->Scale( scale, "width" );
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

/*
      else
      {
         fLogInfo << " ExpData do match (some of) the MC ";
	 for ( size_t i=0; i<fVDBRecID2MC.size(); ++i )
	 {
	    std::cout << " i = " << i << " recid = " << fVDBRecID2MC[i].first;
	    if ( fVDBRecID2MC[i].second != NULL ) 
	    {
	       std::cout << " " << fVDBRecID2MC[i].second->GetName();
	    }
	    else
	    {
	       std::cout << " NULL";
	    }
	    std::cout << std::endl;
	 }
      }
*/      
      calculateChi2();
      overlayDataMC();
   }
   
   return;

}

void artg4tk::AnalyzerHARP::analyze( const art::Event& e )
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
      double theta = sec.GetMomentum().vect().theta();
      
      if ( theta < fThetaMinFW ) continue;
      if ( theta < fThetaMinFW+fDeltaThetaFW*fNThetaBinsFW )
      {
         int ith = ( theta - fThetaMinFW ) / fDeltaThetaFW;
         if ( pname == "pi-" )
         {
	      fHistoSecPiMinusFW[ith]->Fill( pmom ); //, xsec/millibarn );
         }
         else if ( pname == "pi+" )
         {
	      fHistoSecPiPlusFW[ith]->Fill( pmom ); //, xsec/millibarn );
         }
         else if ( pname == "proton" )
         {
	      fHistoSecProtonFW[ith]->Fill( pmom ); //, xsec/millibarn );
	 }
      }
      
      if ( theta < fThetaMinLA ) continue;
      if ( theta > fThetaMinLA+fDeltaThetaLA*fNThetaBinsLA ) continue;
      int    itheta = ( theta - fThetaMinLA ) / fDeltaThetaLA;
      if ( itheta < 0 || itheta >= fNThetaBinsLA ) continue;
      
      if ( pname == "pi-" )
      {
         fHistoSecPiMinusLA[itheta]->Fill( pmom );
      }
      else if ( pname == "pi+" )
      {
         fHistoSecPiPlusLA[itheta]->Fill( pmom );
      }
      else if ( pname == "proton" )
      {
         fHistoSecProtonLA[itheta]->Fill( pmom );
      }    

   } // end loop over secondaries
   
   return;
   
}

TH1* artg4tk::AnalyzerHARP::matchExpSpectrum2MC( const int& secid, const std::vector<std::string>& input, const int& )
{
   
   // FIXME !!!
   // This will be redone once parnames/parvalues are finished !
   //
   std::vector<std::string> cond = extractThetaBinFromTitle( input[0] );
   
   if ( secid == 211 )
   {
      for ( size_t i=0; i<fHistoSecPiPlusFW.size(); ++i )
      {
	 std::string htitle = fHistoSecPiPlusFW[i]->GetTitle();
	 bool match = true;
	 for ( size_t j=0; j<cond.size(); ++j )
	 {
	    if ( htitle.find( cond[j] ) == std::string::npos )
	    {
	       match = false;
	       break;
	    }
	 }
	 if ( match )
	 {
	    return fHistoSecPiPlusFW[i];
	 }	 
      }
      for ( size_t i=0; i<fHistoSecPiPlusLA.size(); ++i )
      {
	 std::string htitle = fHistoSecPiPlusLA[i]->GetTitle();
	 bool match = true;
	 for ( size_t j=0; j<cond.size(); ++j )
	 {
	    if ( htitle.find( cond[j] ) == std::string::npos )
	    {
	       match = false;
	       break;
	    }
	 }
	 if ( match )
	 {
	    return fHistoSecPiPlusLA[i];
	 }	 
      }
   }
   else if ( secid == -211 )
   {
      for ( size_t i=0; i<fHistoSecPiMinusFW.size(); ++i )
      {
	 std::string htitle = fHistoSecPiMinusFW[i]->GetTitle();
	 bool match = true;
	 for ( size_t j=0; j<cond.size(); ++j )
	 {
	    if ( htitle.find( cond[j] ) == std::string::npos )
	    {
	       match = false;
	       break;
	    }
	 }
	 if ( match )
	 {
	    return fHistoSecPiMinusFW[i];
	 }	 
      }
      for ( size_t i=0; i<fHistoSecPiMinusLA.size(); ++i )
      {
	 std::string htitle = fHistoSecPiMinusLA[i]->GetTitle();
	 bool match = true;
	 for ( size_t j=0; j<cond.size(); ++j )
	 {
	    if ( htitle.find( cond[j] ) == std::string::npos )
	    {
	       match = false;
	       break;
	    }
	 }
	 if ( match )
	 {
	    return fHistoSecPiMinusLA[i];
	 }	 
      }
   }
   else if ( secid == 2212 )
   {
      for ( size_t i=0; i<fHistoSecProtonFW.size(); ++i )
      {
	 std::string htitle = fHistoSecProtonFW[i]->GetTitle();
	 bool match = true;
	 for ( size_t j=0; j<cond.size(); ++j )
	 {
	    if ( htitle.find( cond[j] ) == std::string::npos )
	    {
	       match = false;
	       break;
	    }
	 }
	 if ( match )
	 {
	    return fHistoSecProtonFW[i];
	 }	 
      }
      for ( size_t i=0; i<fHistoSecProtonLA.size(); ++i )
      {
	 std::string htitle = fHistoSecProtonLA[i]->GetTitle();
	 bool match = true;
	 for ( size_t j=0; j<cond.size(); ++j )
	 {
	    if ( htitle.find( cond[j] ) == std::string::npos )
	    {
	       match = false;
	       break;
	    }
	 }
	 if ( match )
	 {
	    return fHistoSecProtonLA[i];
	 }	 
      }
   }
   
   return NULL;

}

std::vector<std::string> artg4tk::AnalyzerHARP::extractThetaBinFromTitle( const std::string& title )
{

   size_t pos1 = title.find("<theta<");
   size_t pos2 = title.find_last_of( " ", pos1 );
   std::string th1 = title.substr( pos2+1, pos1-pos2-1 );

   pos2 = title.find_first_of( " ", pos1 );
   std::string tmp = title.substr( pos1, pos2-pos1 );
   pos2 = tmp.find_last_of("<");
   std::string th2 = tmp.substr(pos2+1);
   
   std::vector<std::string> ret;
   ret.push_back( th1 );
   ret.push_back( th2 );
   
   return ret;

}

void artg4tk::AnalyzerHARP::calculateChi2()
{
   
   art::ServiceHandle<art::TFileService> tfs;  
   std::vector<RecordChi2*> vrchi2;
   // ---> std::vector<RecordChi2*> vrchi2total;

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

using artg4tk::AnalyzerHARP;
DEFINE_ART_MODULE(AnalyzerHARP)
