#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <list>

#include <math.h>
#include <vector>

#include "Rtypes.h"
#include "TROOT.h"
#include "TRint.h"
#include "TObject.h"
#include "TFile.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TCanvas.h"
// --> #include "TPad.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TLine.h"
#include "TArrow.h"

std::vector<std::string> flist;
std::vector< std::pair<std::string,std::string> > particles;
std::vector<int> pcolor;
std::vector<int> pmarker;
std::vector< std::pair<std::string,double> > pdefaults;

void Init()
{

   flist.push_back("proton5GeV_C_rscale_analysis.root");
   flist.push_back("proton5GeV_C_xsscale_analysis.root");
   flist.push_back("proton5GeV_C_fscale_analysis.root");
   flist.push_back("proton5GeV_C_trailingR_analysis.root");

   flist.push_back("piminus5GeV_C_rscale_analysis.root");
   flist.push_back("piminus5GeV_C_xsscale_analysis.root");
   flist.push_back("piminus5GeV_C_fscale_analysis.root");
   flist.push_back("piminus5GeV_C_trailingR_analysis.root");

   particles.push_back( std::pair<std::string,std::string>( "neutron", "neutron" ) );
   particles.push_back( std::pair<std::string,std::string>( "proton", "proton" ) );
   particles.push_back( std::pair<std::string,std::string>( "piplus", "#pi^{+}" ) );
   particles.push_back( std::pair<std::string,std::string>( "piminus", "#pi^{-}" ) );
   particles.push_back( std::pair<std::string,std::string>( "total", "total" ) );

   pcolor.push_back(kBlack);
   pcolor.push_back(kGreen);
   pcolor.push_back(kRed);
   pcolor.push_back(kBlue); 
   pcolor.push_back(kMagenta); 
   
   pmarker.push_back(20);
   pmarker.push_back(21);
   pmarker.push_back(22);
   pmarker.push_back(23);
   pmarker.push_back(34); 
   
   pdefaults.push_back( std::pair<std::string,double>( "RadiusScale", 2.82 ) );
   pdefaults.push_back( std::pair<std::string,double>( "XSecScale", 1.0 ) );
   pdefaults.push_back( std::pair<std::string,double>( "FermiScale", 0.685 ) );
   pdefaults.push_back( std::pair<std::string,double>( "TrailingRadius", 0.0 ) );

   return;

}

void PlotParametersVsChi2()
{
      
   std::string label = "Chi2NDF";

   Init();
   
   int np = particles.size();
   TProfile** hh  = new TProfile*[np];
   TProfile** hh1 = new TProfile*[np];
      
   int nf = flist.size();
   
   TFile** hfile = new TFile*[nf];
   
   TCanvas** cnvlist  = new TCanvas*[nf];
   TPad**    pad1list = new TPad*[nf];
   TPad**    pad2list = new TPad*[nf];
   for ( int ic=0; ic<nf; ++ic )
   {
      // std::string cnv_gtitle = flist[ic].substr( 0, flist[ic].find_first_of("_") ); 
      std::string cnv_gtitle = flist[ic].substr( 0, flist[ic].find_last_of("_") ); 
      std::ostringstream s;
      s << ic;
      // std::string cnvname = cnv_gtitle + "_cnv" + s.str() + "_" + label;
      std::string cnvname = cnv_gtitle + "_" + label;
      cnvlist[ic] = new TCanvas( cnvname.c_str(), "", 900, 500 ) ;
      pad1list[ic] = new TPad( ( "pad1" + s.str() + "_" + label ).c_str(), "", 0.01, 0.21, 0.49, 0.99 );
      pad2list[ic] = new TPad( ( "pad2" + s.str() + "_" + label ).c_str(), "", 0.51, 0.21, 0.99, 0.99 );
      cnvlist[ic]->cd();
      pad1list[ic]->Draw();
      cnvlist[ic]->cd();
      pad2list[ic]->Draw();
   }
      
   for ( unsigned int i=0; i<flist.size(); ++i )
   {
   
      hfile[i] = TFile::Open( flist[i].c_str() );
      
      TIter     next( hfile[i]->GetDirectory( "ParametersAnalysis" )->GetListOfKeys() );
      TKey*     key = (TKey*)next();
      TProfile* h = 0;
      
      double ymax = -1.e+25;
      double ymin =  1.;
      double ymax1 = -1.e+25;
      double ymin1 =  10.;

      bool first=true;
      
      int counter = 0;

      TLegend* leg = new TLegend( 0.25, 0.01, 0.75, 0.20 );
      leg->SetTextFont(62);
      leg->SetFillColor(kWhite);
      
      while ( key )
      {

         h = (TProfile*)key->ReadObj();
         std::string hname = h->GetName();
	 	 
	 if ( hname.find( label ) == std::string::npos ) 
	 {
            key = (TKey*)next();
	    continue;
	 }
	 
	 // name for the "Chi2" counterpart
	 //
	 std::string h1name = hname;
	 std::string sname = "NDF";
	 h1name.erase( h1name.find(sname), sname.length() ); 
	 	 
         unsigned int ip=0;
	 for ( ; ip<particles.size(); ++ip )
	 {
	    if ( hname.find( particles[ip].first ) != std::string::npos ) break;
	 }
	 
	 if ( ip >= particles.size() ) 
	 {
            key = (TKey*)next();
	    continue;
	 }
	 
	 hh[counter] = (TProfile*)key->ReadObj();
	 hh[counter]->SetStats(0);
	 
	 hh1[counter] = (TProfile*)hfile[i]->Get( ( "ParametersAnalysis/" + h1name ).c_str() );
	 hh1[counter]->SetStats(0);
	 	 
	 int imax = hh[counter]->GetMaximumBin();	 
	 if ( ymax < (hh[counter]->GetBinContent(imax)+2.*hh[counter]->GetBinError(imax)) ) 
	      ymax=hh[counter]->GetBinContent(imax)+2.*hh[counter]->GetBinError(imax);

	 int imax1 = hh1[counter]->GetMaximumBin();	 
	 if ( ymax1 < (hh1[counter]->GetBinContent(imax1)+2.*hh1[counter]->GetBinError(imax1)) ) 
	      ymax1=hh1[counter]->GetBinContent(imax1)+2.*hh1[counter]->GetBinError(imax1);

	 hh[counter]->SetMarkerStyle( pmarker[ip] );
	 hh[counter]->SetMarkerColor( pcolor[ip] );
	 hh[counter]->SetMarkerSize(1.);

	 hh1[counter]->SetMarkerStyle( pmarker[ip] );
	 hh1[counter]->SetMarkerColor( pcolor[ip] );
	 hh1[counter]->SetMarkerSize(1.);
	 
	 if ( particles[ip].second == "total" )
	 {
	    leg->AddEntry( hh[counter], ( particles[ip].second ).c_str(), "p" );
	 }
	 else
	 {
	    leg->AddEntry( hh[counter], ( "secondary " + particles[ip].second ).c_str(), "p" );
	 }

	 pad1list[i]->cd();

	 for ( int icnt=0; icnt<=counter; ++icnt )
	 {
	    hh[icnt]->GetYaxis()->SetRangeUser( ymin, ymax*1.1 );
	 }
	 
	 pad1list[i]->Update();
	 cnvlist[i]->Update();

	 pad2list[i]->cd();

	 for ( int icnt=0; icnt<=counter; ++icnt )
	 {
	    hh1[icnt]->GetYaxis()->SetRangeUser( ymin1, ymax1*1.25 );
	 }
	 
	 pad2list[i]->Update();
	 cnvlist[i]->Update();
	 	 
	 if ( first )
	 {
	    pad1list[i]->cd();
	    gPad->SetLeftMargin(0.2);
	    gPad->SetRightMargin(0.1);
	    gPad->SetLogy();
	    hh[counter]->GetYaxis()->SetTitleOffset(1.5);
	    hh[counter]->GetXaxis()->SetTitleFont(62);
	    hh[counter]->GetYaxis()->SetTitleFont(62);
	    hh[counter]->GetXaxis()->SetLabelFont(62);
	    hh[counter]->GetYaxis()->SetLabelFont(62);
	    hh[counter]->Draw("p");
	    pad2list[i]->cd();
	    gPad->SetLeftMargin(0.2);
	    gPad->SetRightMargin(0.1);
	    gPad->SetLogy();
	    hh1[counter]->GetYaxis()->SetTitleOffset(1.5);
	    hh1[counter]->GetXaxis()->SetTitleFont(62);
	    hh1[counter]->GetYaxis()->SetTitleFont(62);
	    hh1[counter]->GetXaxis()->SetLabelFont(62);
	    hh1[counter]->GetYaxis()->SetLabelFont(62);
	    hh1[counter]->Draw("p");	    
	    first = false;
	 }
	 else
	 {
	    pad1list[i]->cd();
	    hh[counter]->Draw("psame");
	    pad2list[i]->cd();
	    hh1[counter]->Draw("psame");
	 }
	 
	 for ( size_t id=0; id<pdefaults.size(); ++id )
	 {
	    if ( hname.find( pdefaults[id].first ) != std::string::npos )
	    {
	       
	       TArrow ar( pdefaults[id].second, ymin, pdefaults[id].second, 0.5*(ymin+ymax), 0.02, "<|" );
	       ar.SetLineWidth(3);
	       ar.SetLineColor(7);
	       pad1list[i]->cd();
	       ar.Draw();
	       // pad2list[i]->cd();
	       // ar.Draw();
	       break;
	    }
	 }
	 	 
	 pad1list[i]->Update();
	 pad2list[i]->Update();
	 cnvlist[i]->Update();
	 
	 counter++;

         key = (TKey*)next();
	 
      } 
      
      cnvlist[i]->cd();
      leg->Draw();
      cnvlist[i]->Update();
        
      // hfile->Close();      
   
   }

   for ( int ic=0; ic<nf; ++ic )
   {
      std::string outname = cnvlist[ic]->GetName();
      outname += ".gif";
      cnvlist[ic]->Print( outname.c_str() );
   }
   
   for ( int ifl=0; ifl<nf; ++ifl )
   {
      hfile[ifl]->Close();
   }
   
   return;
   
}

void PlotParametersVsWt()
{

   std::string label = "Wt_norm";

   Init();
   
   // int np = particles.size();
   // TProfile** hh = new TProfile*[np];
      
   int nf = flist.size();
   
   TFile** hfile = new TFile*[nf];
   
   TCanvas** cnvlist = new TCanvas*[nf];
   TPad**    padlist = new TPad*[nf];  
   for ( int ic=0; ic<nf; ++ic )
   {
     // std::string cnv_gtitle = flist[ic].substr( 0, flist[ic].find_first_of("_") ); 
      std::string cnv_gtitle = flist[ic].substr( 0, flist[ic].find_last_of("_") ); 
      std::ostringstream s;
      s << ic;
      // std::string cnvname = cnv_gtitle + "_cnv" + s.str() + "_" + label;
      std::string cnvname = cnv_gtitle + "_" + label;
      cnvlist[ic] = new TCanvas( cnvname.c_str(), "", 500, 500 ) ;
      padlist[ic] = new TPad( ( "pad" + s.str() + "_" + label ).c_str(), "", 0.01, 0.21, 0.99, 0.99 );
      cnvlist[ic]->cd();
      padlist[ic]->Draw();
   }

   for ( unsigned int i=0; i<flist.size(); ++i )
   {
   
      hfile[i] = TFile::Open( flist[i].c_str() );
      
      TIter     next( hfile[i]->GetDirectory( "ParametersAnalysis" )->GetListOfKeys() );
      TKey*     key = (TKey*)next();
      TProfile* h = 0;

      double ymax = 1.e+25;
      double ymin = 1.e-150;
      if ( flist[i].find("piminus") != std::string::npos || flist[i].find("piplus") != std::string::npos ) ymin = 1.e-250;

      TLegend* leg = new TLegend( 0.15, 0.01, 0.85, 0.20 );
      leg->SetTextFont(62);
      leg->SetFillColor(kWhite);

      while ( key )
      {

         h = (TProfile*)key->ReadObj();
         std::string hname = h->GetName();
	 	 
	 if ( ! ( hname.find( label ) != std::string::npos && hname.find("total") != std::string::npos ) ) 
	 {
            key = (TKey*)next();
	    continue;
	 }
	 
         unsigned int ip=0;
	 for ( ; ip<particles.size(); ++ip )
	 {
	       if ( particles[ip].first == "total" ) break;
	 }
	 
	 if ( ip >= particles.size() ) 
	 {
            key = (TKey*)next();
	    continue;
	 }
	 
	 h->SetStats(0);
	 h->SetMarkerStyle( pmarker[ip] );
	 h->SetMarkerColor( pcolor[ip] );
	 h->SetLineColor( pcolor[ip] );
	 h->SetMarkerSize(1.);
	 
	 padlist[i]->cd();
	 gPad->SetLeftMargin(0.2);
	 gPad->SetRightMargin(0.1);
	 gPad->SetLogy();
	 h->GetYaxis()->SetRangeUser( ymin, ymax );
	 h->GetYaxis()->SetTitleOffset(1.75);
	 h->GetXaxis()->SetTitleFont(62);
	 h->GetYaxis()->SetTitleFont(62);
	 h->GetXaxis()->SetLabelFont(62);
	 h->GetYaxis()->SetLabelFont(62);
	 // h->Draw("p");
	 h->SetLineWidth(3);
	 h->Draw("histE1");
	 
	 std::string tmpname = hname;
	 size_t pos = tmpname.find( label );
	 tmpname.erase( tmpname.find(label), label.length()+1 );
	 std::cout << " hname = " << hname << " tmpname = " << tmpname << std::endl;
	 TProfile* hprof = (TProfile*)hfile[i]->Get( ( "ParametersAnalysis/" + tmpname ).c_str() );
	 if ( hprof ) hprof->Draw("same");
	 leg->Clear();
	 leg->AddEntry( hprof, "original distribution", "L" );
	 // leg->AddEntry( h, "weighted by exp(-0.5*chi2), normalized", "p" );
	 leg->AddEntry( h, "weighted by exp(-0.5*chi2), normalized", "L" );
	 
	 for ( size_t id=0; id<pdefaults.size(); ++id )
	 {
	    if ( hname.find( pdefaults[id].first ) != std::string::npos )
	    {
	       
	       TArrow ar( pdefaults[id].second, ymin, pdefaults[id].second, 0.5*(ymin+ymax), 0.02, "<|" );
	       ar.SetLineWidth(3);
	       ar.SetLineColor(7);
	       ar.Draw();
	       break;
	    }
	 }

	 padlist[i]->Update();
	 cnvlist[i]->Update();
	 
         key = (TKey*)next();
	 
      } 

      cnvlist[i]->cd();
      leg->Draw();
      cnvlist[i]->Update();
        
      // hfile->Close();      

   }

   for ( int ic=0; ic<nf; ++ic )
   {
      std::string outname = cnvlist[ic]->GetName();
      outname += ".gif";
      cnvlist[ic]->Print( outname.c_str() );
   }
   
   for ( int ifl=0; ifl<nf; ++ifl )
   {
      hfile[ifl]->Close();
   }

   return;

}
