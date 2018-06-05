
#include "artg4tk/G4PhysModelParamStudy/AnalysisBase/AnalyzerWithExpDataBase.hh"

#include "art/Framework/Services/Optional/TFileService.h"
#include "TFile.h"
#include "TCanvas.h"

#include "TProfile.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <memory>

artg4tk::AnalyzerWithExpDataBase::AnalyzerWithExpDataBase( const fhicl::ParameterSet& p )
  : art::EDAnalyzer(p),
    fLogInfo("AnalyzerExpData")
{

   // fIncludeExpData = p.get<bool>("IncludeExpData",false);
   fIncludeExpData = false;
   fVDBConnect = 0;
   fJSON2Data = 0;
   if ( p.has_key("IncludeExpData") )
   {   
      fIncludeExpData = true;
      fVDBRecordID = (p.get<fhicl::ParameterSet>("IncludeExpData")).get<std::vector<int> >("DBRecords");
      fVDBConnect = new VDBConnect();
      bool status = fVDBConnect->Init();
      if ( !status )
      {
         fLogInfo << " Exp.data are requested but connection to VDB fails"; // << std::endl;
      }
      fJSON2Data = new JSON2Data();
      fJSON2Data->BuildDictionaries( "Particle",   fVDBConnect->GetDictionary("Particle") );
      fJSON2Data->BuildDictionaries( "Material",   fVDBConnect->GetDictionary("Material") );
      fJSON2Data->BuildDictionaries( "Beam",       fVDBConnect->GetDictionary("Beam") );
      fJSON2Data->BuildDictionaries( "Observable", fVDBConnect->GetDictionary("Observable") );
      fJSON2Data->BuildDictionaries( "Reference",  fVDBConnect->GetDictionary("Reference") );
      fJSON2Data->BuildDictionaries( "Datatypes",  fVDBConnect->GetDictionary("Datatypes") ); 
   }
   
}

artg4tk::AnalyzerWithExpDataBase::~AnalyzerWithExpDataBase()
{
   
   if ( fVDBConnect ) delete fVDBConnect;
   if ( fJSON2Data )  delete fJSON2Data;
}

void artg4tk::AnalyzerWithExpDataBase::endJob()
{
   
   // In case exp.data are involved/requested
   //
   if ( fIncludeExpData )
   {
                  
      if ( fVDBConnect->IsInitialized() )
      {
	  
	 art::ServiceHandle<art::TFileService> tfs;
	 art::TFileDirectory exdir = tfs->mkdir( "ExpData" );  
	
	 for ( size_t ir=0; ir<fVDBRecordID.size(); ++ir )
	 {
	    // fJSONRecords.push_back( fVDBConnect->GetHTTPResponse( fVDBRecordID[ir] ) );
	    std::string rjson = fVDBConnect->GetHTTPResponse( fVDBRecordID[ir] );
	    fJSONRecords.insert( std::pair<int, std::string>( fVDBRecordID[ir], rjson ) );	                                                      
	    std::string hname = "ExpDataR" + std::to_string(fVDBRecordID[ir]);
	    exdir.make<TH1D>( *(fJSON2Data->Convert2Histo(rjson,hname.c_str())) );
	 }
      }
   }

   return;

}

bool artg4tk::AnalyzerWithExpDataBase::matchVDBRec2MC( const int& bid,
                                                       const double& bmom,
						       const int& tgt )
{

   bool ok = false;
   
   ok = findExpDataByBeamTarget( bid, bmom, tgt );
   if ( !ok ) 
   {
      fLogInfo << " No exp.data to match beam particle = " << bid
               << " of momentum = " << bmom 
	       << " and target id = " << tgt ; 
      return ok;
   }
   
   std::map<int,std::string>::iterator itr;
   
   for ( size_t i=0; i<fVDBRecID2MC.size(); ++i )
   {
      
      itr = fJSONRecords.find( fVDBRecID2MC[i].first );
      if ( itr == fJSONRecords.end() ) continue;
      fJSON2Data->ParseMetaData( itr->second );
      //
      // FIXME !!!
      // In the next iteration this should be parnames/parvalues !!!!!
      //
      std::vector<std::string> cond;
      cond.push_back( fJSON2Data->GetMetaData().fTitle );      
      //
      TH1* h = matchExpSpectrum2MC( fJSON2Data->GetMetaData().fSecondaryPID, cond, fJSON2Data->GetMetaData().fObservable );
      if ( h )
      {
	 fVDBRecID2MC[i].second = h;
	 ok = true;
      } 
      else
      {
         std::cout << " NO match for: " << fJSON2Data->GetMetaData().fTitle << " ---> histo = NULL " << std::endl;
      }
   }
   
/* for testing/debugging purposes

   for ( size_t i=0; i<fVDBRecID2MC.size(); ++i )
   {
      std::cout << " RecID=" << fVDBRecID2MC[i].first << " --> " ;
      if ( fVDBRecID2MC[i].second )
      {
         std::cout << fVDBRecID2MC[i].second->GetName() << " " 
		   << fVDBRecID2MC[i].second->GetTitle() ;
      }
      else
      {
         std::cout << " NULL ";
      }
      std::cout << std::endl;
   }
*/   
      
   return ok;

}

bool artg4tk::AnalyzerWithExpDataBase::findExpDataByBeamTarget( const int& bid, 
                                                                const double& bmom, 
                                                                const int& tgt )
{

   if ( !fJSON2Data ) return false;
   
   std::map<int,std::string>::iterator itr = fJSONRecords.begin();
   
   for ( ; itr!=fJSONRecords.end(); ++itr )
   {
      fJSON2Data->ParseMetaData( itr->second );
      if ( fJSON2Data->GetMetaData().IsBeamTargetMatch( bid, bmom, tgt ) ) 
      {
         fVDBRecID2MC.push_back( std::pair<int,TH1*>( itr->first, NULL ) );
      }
   }
   
   return ( !fVDBRecID2MC.empty() );

}

void artg4tk::AnalyzerWithExpDataBase::rebinMC2Data( const std::string& tag2rm )
{

   art::ServiceHandle<art::TFileService> tfs;  
   TH1::SetDefaultSumw2();

   std::vector< std::pair<int,TH1*> >::iterator itr = fVDBRecID2MC.begin();
   for ( ; itr!=fVDBRecID2MC.end(); ++itr )
   {
         if ( ! itr->second ) continue; // NULL histo for this exp.data record (some might be skipped)
	 std::map<int,std::string>::iterator itrda = fJSONRecords.find( itr->first );
         TH1D* hda = 0;      
         if ( itrda != fJSONRecords.end() ) 
         {
	    hda = fJSON2Data->Convert2Histo(itrda->second,"tmpdata");
         }
         if ( !hda ) 
	 {
	    fLogInfo << " Can NOT find ExpData by (DoSSiER) record = " << itr->first;
	    continue;
	 }
	 int nbins = hda->GetNbinsX();
	 double* xbins = new double[nbins+1];
	 xbins[0] = hda->GetBinLowEdge(1);
	 for ( int ib=1; ib<=nbins; ++ib )
	 {
	    xbins[ib] = xbins[ib-1] + hda->GetBinWidth(ib);
	 }
	 std::string hname = (itr->second)->GetName();
	 size_t pos = hname.find( tag2rm );
	 if ( pos != std::string::npos ) hname.erase( pos, std::string(tag2rm).length() );	 
	 std::string hname_new = "rebin_" + hname;
	 //
	 // NOTE (JVY): for now, do a tmp histo for rebining, then copy it bin-by-bin
	 //             into the one "made of data", because it's not clear how to handle
	 //             the case via TFileService machinery
	 //
	 TH1* tmprebin = (itr->second)->Rebin( nbins, hname_new.c_str(), xbins );
	 TH1D* h = tfs->make<TH1D>( *hda );
	 h->Reset();
	 for ( int ib=1; ib<=nbins; ++ib )
	 {
	    h->SetBinContent( ib, tmprebin->GetBinContent(ib) );
	    h->SetBinError( ib, tmprebin->GetBinError(ib) );
	 }
	 h->SetName( hname.c_str() );
	 h->SetTitle( (itr->second)->GetTitle() );
	 itr->second = h;
/* ---> initial version... 
// ...presents kind of issue when rebinning TProfile 
//    because there's no direct access to the bin weights (private functions only)
//
	 TH1D* h = tfs->make<TH1D>( *hda );
	 int nbdata = h->GetNbinsX();
	 double* bcont = new double[nbdata];
	 double* err2  = new double[nbdata];
	 for ( int ib=0; ib<nbdata; ++ib )
	 {
	    bcont[ib] = 0.;
	    err2[ib]  = 0.;
	 }
	 h->Reset();
         std::string hname = (itr->second)->GetName();
	 size_t pos = hname.find("Tmp");
	 if ( pos != std::string::npos ) hname.erase( pos, std::string("Tmp").length() );	 
	 h->SetName( hname.c_str() );
	 h->SetTitle( (itr->second)->GetTitle() );	 
	 for ( int ib=1; ib<=(itr->second)->GetNbinsX(); ++ib )
	 {
	    double xx  = (itr->second)->GetBinCenter(ib);
	    double yy  = (itr->second)->GetBinContent(ib);
	    double eyy = (itr->second)->GetBinError(ib);
	    int    ibf = h->FindBin(xx);
	    if ( ibf > 0 && ibf <= nbdata ) 
	    {
	       err2[ibf-1] += eyy*eyy;
	       bcont[ibf-1] += yy;
	    }
	 }
         for ( int ib=1; ib<=nbdata; ++ib )
         {
            h->SetBinContent( ib, bcont[ib-1] );
	    double err = std::sqrt( err2[ib-1] );
	    h->SetBinError( ib, err ); 
         }
	 itr->second = h;
*/
   }

   return; 

}

void artg4tk::AnalyzerWithExpDataBase::overlayDataMC()
{

   if ( !fIncludeExpData ) return;
   
   std::string modlabel = gDirectory->GetName(); // it'll match module label
   
   art::ServiceHandle<art::TFileService> tfs;
   art::TFileDirectory plotdir = tfs->mkdir( "Plots" );
   
   TFile& hfile = tfs->file();
   
   std::vector< std::pair<int,TH1*> >::iterator itr = fVDBRecID2MC.begin();
   
   for ( ; itr!=fVDBRecID2MC.end(); ++itr )
   {
      if ( !itr->second ) continue;
      std::string dname = modlabel + "/ExpData/ExpDataR" + std::to_string(itr->first);
      TH1D* hd = (TH1D*)(hfile.Get(dname.c_str()));
      if ( !hd ) continue;
      int imax = hd->GetMaximumBin();
      double ymax = hd->GetBinContent(imax)+hd->GetBinError(imax);
      imax = itr->second->GetMaximumBin();
      ymax = std::max( ymax, itr->second->GetBinContent(imax)+itr->second->GetBinError(imax) );
      hd->GetYaxis()->SetRangeUser( 0., ymax );
      itr->second->GetYaxis()->SetRangeUser( 0., ymax );
      std::string cname = "cnv_" + modlabel + "_" + std::string( itr->second->GetName() );
      TCanvas* cnv = plotdir.make<TCanvas>( cname.c_str(), "", 500, 500 );
//      cnv->cd();
      itr->second->Draw("histE1");
      hd->SetMarkerStyle(22);
      hd->SetMarkerColor(kBlue);
      hd->SetMarkerSize(1.5);
      hd->Draw("pE1same");
      //
      // NOTE: TCanvas is NOT automatically appended to the directory (unlike e.g. TH objects).
      // In principle, there's TFileDirectory::makeAndRegister but it requires name and title, 
      // and TCanvas' ctor also wants such args, so it'd look quite goofy !
      // We can Append it or use the Write() method.
      //
      // cnv->Write();
      gDirectory->Append(cnv);
/*
      std::string output = cname + ".gif";
      cnv->Print( output.c_str() ); 
*/
//      gDirectory->cd();    
   }
        
   return;

}
