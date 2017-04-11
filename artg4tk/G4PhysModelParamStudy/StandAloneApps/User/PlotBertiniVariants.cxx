#include <iostream>
#include <string>
#include <vector>

#include "TRandom3.h"
#include "TCanvas.h"
#include "TFile.h"

#include "Cintex/Cintex.h"

#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/BeamThinTargetConfig.hh"
#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/RecordChi2.hh"

#include "TH1D.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMarker.h"
#include "TKey.h"
#include "TClass.h"

TFile* hfile = 0;
std::vector<TH1D*> piplus_HARP;
std::vector<TH1D*> piminus_HARP;
std::vector<TH1D*> proton_ITEP;
std::vector<TH1D*> neutron_ITEP;

TH1D* GetHisto( std::string, double& );
TH1D* GetExpHARP( TH1D*, const std::vector<TH1D*>&, double& );
void DrawVariantsHARP( const int, std::string*, 
                       std::string, std::string, 
		       int*,
		       double& );  
void VariantsHARP2Legend( const int nv, std::string* vdirs, std::string, std::string rname, 
                          std::string* vlabels, int* colors, TLegend* leg ); 

int main( int argc, char** argv )
{

   std::string fname = argv[1];
   
   ROOT::Cintex::Cintex::Enable();
   
   std::string model = "Bertini";
   std::string def   = "Default";
   
   const int NRsc = 4;
   std::string drsc[NRsc]  = { "Rsc125", "Rsc075", "Rsc05", "Rsc035" };
   std::string rsclabel[NRsc] = { "RadiusScale=3.5", "RadiusScale=2.1",
                                  "RadiusScale=1.4", "RadiusScale=1." }; 
   int colrsc[NRsc] = { kBlue, kGreen, kMagenta, kCyan }; // kCyan=7
   const int NXSsc = 6;
   std::string dxssc[NXSsc] = { "XSsc2", "XSsc15", "XSsc125", "XSsc075", "XSsc05", "XSsc01" };
   std::string xssclabel[NXSsc] = { "XSecScale=2.", "XSecScale=1.5", "XSecScale=1.25",
                                    "XSecScale=0.75", "XSecScale=0.5", "XSecScale=0.1" }; 
   // int colxssc[NXSsc] = { kYellow-2, kTeal+3 };
   int colxssc[NXSsc] = { kYellow+3, kTeal+3, kPink-9, kBlack, kRed-7, kGreen+4 };
   const int NFsc = 2;
   std::string dfsc[NFsc]  = { "Fsc05", "Fsc1" };
   std::string fsclabel[NFsc] = { "FermiScale=0.5", "FermiScale=1." };
   int colfsc[NFsc] = { kOrange-7, kViolet-5 };
   const int NGQDsc = 2;
   std::string dgqdsc[NGQDsc] = { "GDQsc05", "GDQsc2" };
   std::string gqdsclabel[NGQDsc] = { "GammaQDScale=0.5", "GammaQDScale=2." };
   int colgqdsc[NGQDsc] = { kTeal-7, kBlack };
   const int NTrR = 2;
   std::string dtrr[NTrR]  = { "TrR07", "TrR1" };
   std::string trrlabel[NTrR] = { "TrailingRadius=0.7", "TrailingRadius=1." };
   int coltrr[NTrR] = { kYellow, kGray };
   
   hfile = TFile::Open( fname.c_str() );
   
   BeamThinTargetConfig* btconf = (BeamThinTargetConfig*)hfile->Get( "BertiniDefaultHARP/Beam_ThinTarget_Config" );
   // btconf->Print();
   
   std::string gtitle = "Beam: "      + std::to_string(btconf->GetBeamPartID()) 
                      + " Momentum: " + std::to_string(btconf->GetBeamMomentum()) + " GeV/c"
		      + " Target: "   + std::to_string(btconf->GetTargetID());

   // HARP business
   
   // HARP exp. data/records
   //
   TIter     next( hfile->GetDirectory( "BertiniDefaultHARP/ExpData" )->GetListOfKeys() );
   TKey*     key = (TKey*)next();
   TH1D*     hexp = 0;

   while ( key )
   {
      if ( !(TClass::GetClass(key->GetClassName())->InheritsFrom(TH1::Class())) ) 
      {
         key = (TKey*)next();
	 continue;
      }
      hexp = (TH1D*)key->ReadObj();
      std::string htitle = hexp->GetTitle();
      if ( htitle.find( "Production of pi+" ) != std::string::npos ) piplus_HARP.push_back(hexp);
      if ( htitle.find( "Production of pi-" ) != std::string::npos ) piminus_HARP.push_back(hexp);
      key = (TKey*)next();
   }

   std::string title_piplus = gtitle + " Secondary: 211";
   TLatex* ltx_piplus = new TLatex( 0.15, 0.98, title_piplus.c_str() );
   ltx_piplus->SetTextFont(62);
   ltx_piplus->SetTextSize(0.03); 
   std::string title_piminus = gtitle + " Secondary: -211";
   TLatex* ltx_piminus = new TLatex( 0.15, 0.98, title_piminus.c_str() );
   ltx_piminus->SetTextFont(62);
   ltx_piminus->SetTextSize(0.03); 
   //
   // RadiusScale variants - TCanvas
   //
   // secondary pi+
   //
   TCanvas* cnv_rsc_harp_piplus = new TCanvas( "cnv_rsc_harp_piplus", "", 1000, 700 );
   TPad* pad_rsc_harp_piplus_fw = new TPad( "pad_rsc_harp_piplus_fw", "", 0.01, 0.25, 0.399, 0.97 );
   TPad* pad_rsc_harp_piplus_la = new TPad( "pad_rsc_harp_piplus_la", "", 0.401, 0.02, 0.99, 0.97 );
   //cnv_rsc_harp_piplus_fw->Divide(2,2);
   cnv_rsc_harp_piplus->cd();
   ltx_piplus->Draw();
   pad_rsc_harp_piplus_fw->Draw();   
   pad_rsc_harp_piplus_fw->Divide(2,2,0.,0.);
   pad_rsc_harp_piplus_la->Draw();   
   pad_rsc_harp_piplus_la->Divide(3,3,0.,0.);
   TLegend* leg_rsc_harp_piplus = new TLegend( 0.01, 0.02, 0.4, 0.24 );
   leg_rsc_harp_piplus->SetTextFont(62);
   leg_rsc_harp_piplus->SetTextSize(0.02);
   leg_rsc_harp_piplus->SetFillColor(kWhite);
   //
   // secondary pi-
   //
   TCanvas* cnv_rsc_harp_piminus = new TCanvas( "cnv_rsc_harp_piminus", "", 1000, 700 );
   TPad* pad_rsc_harp_piminus_fw = new TPad( "pad_rsc_harp_piminus_fw", "", 0.01, 0.25, 0.399, 0.97 );
   TPad* pad_rsc_harp_piminus_la = new TPad( "pad_rsc_harp_piminus_la", "", 0.401, 0.02, 0.99, 0.97 );
   cnv_rsc_harp_piminus->cd();
   ltx_piminus->Draw();
   pad_rsc_harp_piminus_fw->Draw();   
   pad_rsc_harp_piminus_fw->Divide(2,2,0.,0.);
   pad_rsc_harp_piminus_la->Draw();   
   pad_rsc_harp_piminus_la->Divide(3,3,0.,0.);
   TLegend* leg_rsc_harp_piminus = new TLegend( 0.01, 0.02, 0.4, 0.24 );
   leg_rsc_harp_piminus->SetTextFont(62);
   leg_rsc_harp_piminus->SetTextSize(0.02);
   leg_rsc_harp_piminus->SetFillColor(kWhite);
   //
   // XSecScale variants
   //
   // secondary pi+
   //
   TCanvas* cnv_xssc_harp_piplus = new TCanvas( "cnv_xssc_harp_piplus", "", 1000, 700 );
   TPad* pad_xssc_harp_piplus_fw = new TPad( "pad_xssc_harp_piplus_fw", "", 0.01, 0.25, 0.399, 0.97 );
   TPad* pad_xssc_harp_piplus_la = new TPad( "pad_xssc_harp_piplus_la", "", 0.401, 0.02, 0.99, 0.97 );
   cnv_xssc_harp_piplus->cd();
   ltx_piplus->Draw();
   pad_xssc_harp_piplus_fw->Draw();   
   pad_xssc_harp_piplus_fw->Divide(2,2,0.,0.);
   pad_xssc_harp_piplus_la->Draw();   
   pad_xssc_harp_piplus_la->Divide(3,3,0.,0.);
   TLegend* leg_xssc_harp_piplus = new TLegend( 0.01, 0.02, 0.4, 0.24 );
   leg_xssc_harp_piplus->SetTextFont(62);
   leg_xssc_harp_piplus->SetTextSize(0.02);
   //
   // secondary pi-
   //
   TCanvas* cnv_xssc_harp_piminus = new TCanvas( "cnv_xssc_harp_piminus", "", 1000, 700 );
   TPad* pad_xssc_harp_piminus_fw = new TPad( "pad_xssc_harp_piminus_fw", "", 0.01, 0.25, 0.399, 0.97 );
   TPad* pad_xssc_harp_piminus_la = new TPad( "pad_xssc_harp_piminus_la", "", 0.401, 0.02, 0.99, 0.97 );
   cnv_xssc_harp_piminus->cd();
   ltx_piminus->Draw();
   pad_xssc_harp_piminus_fw->Draw();   
   pad_xssc_harp_piminus_fw->Divide(2,2,0.,0.);
   pad_xssc_harp_piminus_la->Draw();   
   pad_xssc_harp_piminus_la->Divide(3,3,0.,0.);
   TLegend* leg_xssc_harp_piminus = new TLegend( 0.01, 0.02, 0.4, 0.24 );
   leg_xssc_harp_piminus->SetTextFont(62);
   leg_xssc_harp_piminus->SetTextSize(0.02);
   leg_xssc_harp_piminus->SetFillColor(kWhite);
   //
   // FermiScale variants - TCanvas
   //
   // secondary pi+
   //
   TCanvas* cnv_fsc_harp_piplus = new TCanvas( "cnv_fsc_harp_piplus", "", 1000, 700 );
   TPad* pad_fsc_harp_piplus_fw = new TPad( "pad_fsc_harp_piplus_fw", "", 0.01, 0.25, 0.399, 0.97 );
   TPad* pad_fsc_harp_piplus_la = new TPad( "pad_fsc_harp_piplus_la", "", 0.401, 0.02, 0.99, 0.97 );
   cnv_fsc_harp_piplus->cd();
   ltx_piplus->Draw();
   pad_fsc_harp_piplus_fw->Draw();   
   pad_fsc_harp_piplus_fw->Divide(2,2,0.,0.);
   pad_fsc_harp_piplus_la->Draw();   
   pad_fsc_harp_piplus_la->Divide(3,3,0.,0.);
   TLegend* leg_fsc_harp_piplus = new TLegend( 0.01, 0.02, 0.4, 0.24 );
   leg_fsc_harp_piplus->SetTextFont(62);
   leg_fsc_harp_piplus->SetTextSize(0.02);
   leg_fsc_harp_piplus->SetFillColor(kWhite);
   //
   // secondary pi-
   //
   TCanvas* cnv_fsc_harp_piminus = new TCanvas( "cnv_fsc_harp_piminus", "", 1000, 700 );
   TPad* pad_fsc_harp_piminus_fw = new TPad( "pad_fsc_harp_piminus_fw", "", 0.01, 0.25, 0.399, 0.97 );
   TPad* pad_fsc_harp_piminus_la = new TPad( "pad_fsc_harp_piminus_la", "", 0.401, 0.02, 0.99, 0.97 );
   cnv_fsc_harp_piminus->cd();
   ltx_piminus->Draw();
   pad_fsc_harp_piminus_fw->Draw();   
   pad_fsc_harp_piminus_fw->Divide(2,2,0.,0.);
   pad_fsc_harp_piminus_la->Draw();   
   pad_fsc_harp_piminus_la->Divide(3,3,0.,0.);
   TLegend* leg_fsc_harp_piminus = new TLegend( 0.01, 0.02, 0.4, 0.24 );
   leg_fsc_harp_piminus->SetTextFont(62);
   leg_fsc_harp_piminus->SetTextSize(0.02);
   leg_fsc_harp_piminus->SetFillColor(kWhite);
   //
   // GammaQDScale variants
   // NOTE: dir labels are "messed up"  as "gdq" (instead of "gqd that would stand for "gamma quasi-deuteron")
   //
   // secondary pi+
   //
   TCanvas* cnv_gqdsc_harp_piplus = new TCanvas( "cnv_gqdsc_harp_piplus", "", 1000, 700 );
   TPad* pad_gqdsc_harp_piplus_fw = new TPad( "pad_gqdsc_harp_piplus_fw", "", 0.01, 0.25, 0.399, 0.97 );
   TPad* pad_gqdsc_harp_piplus_la = new TPad( "pad_gqdsc_harp_piplus_la", "", 0.401, 0.02, 0.99, 0.97 );
   cnv_gqdsc_harp_piplus->cd();
   ltx_piplus->Draw();
   pad_gqdsc_harp_piplus_fw->Draw();   
   pad_gqdsc_harp_piplus_fw->Divide(2,2,0.,0.);
   pad_gqdsc_harp_piplus_la->Draw();   
   pad_gqdsc_harp_piplus_la->Divide(3,3,0.,0.);
   TLegend* leg_gqdsc_harp_piplus = new TLegend( 0.01, 0.02, 0.4, 0.24 );
   leg_gqdsc_harp_piplus->SetTextFont(62);
   leg_gqdsc_harp_piplus->SetTextSize(0.02);
   leg_gqdsc_harp_piplus->SetFillColor(kWhite);
   //
   // secondary pi-
   //
   TCanvas* cnv_gqdsc_harp_piminus = new TCanvas( "cnv_gqdsc_harp_piminus", "", 1000, 700 );
   TPad* pad_gqdsc_harp_piminus_fw = new TPad( "pad_gqdsc_harp_piminus_fw", "", 0.01, 0.25, 0.399, 0.97 );
   TPad* pad_gqdsc_harp_piminus_la = new TPad( "pad_gqdsc_harp_piminus_la", "", 0.401, 0.02, 0.99, 0.97 );
   cnv_gqdsc_harp_piminus->cd();
   ltx_piminus->Draw();
   pad_gqdsc_harp_piminus_fw->Draw();   
   pad_gqdsc_harp_piminus_fw->Divide(2,2,0.,0.);
   pad_gqdsc_harp_piminus_la->Draw();   
   pad_gqdsc_harp_piminus_la->Divide(3,3,0.,0.);
   TLegend* leg_gqdsc_harp_piminus = new TLegend( 0.01, 0.02, 0.4, 0.24 );
   leg_gqdsc_harp_piminus->SetTextFont(62);
   leg_gqdsc_harp_piminus->SetTextSize(0.02);
   leg_gqdsc_harp_piminus->SetFillColor(kWhite);
   //
   // TrailingEffect (TrailingRadius, aka ShadowingRadius)
   //
   // secondary pi+
   //
   TCanvas* cnv_trr_harp_piplus = new TCanvas( "cnv_trr_harp_piplus", "", 1000, 700 );
   TPad* pad_trr_harp_piplus_fw = new TPad( "pad_trr_harp_piplus_fw", "", 0.01, 0.25, 0.399, 0.97 );
   TPad* pad_trr_harp_piplus_la = new TPad( "pad_trr_harp_piplus_la", "", 0.401, 0.02, 0.99, 0.97 );
   cnv_trr_harp_piplus->cd();
   ltx_piplus->Draw();
   pad_trr_harp_piplus_fw->Draw();   
   pad_trr_harp_piplus_fw->Divide(2,2,0.,0.);
   pad_trr_harp_piplus_la->Draw();   
   pad_trr_harp_piplus_la->Divide(3,3,0.,0.);
   TLegend* leg_trr_harp_piplus = new TLegend( 0.01, 0.02, 0.4, 0.24 );
   leg_trr_harp_piplus->SetTextFont(62);
   leg_trr_harp_piplus->SetTextSize(0.02);
   leg_trr_harp_piplus->SetFillColor(kWhite);
   //
   // secondary pi-
   //
   TCanvas* cnv_trr_harp_piminus = new TCanvas( "cnv_trr_harp_piminus", "", 1000, 700 );
   TPad* pad_trr_harp_piminus_fw = new TPad( "pad_trr_harp_piminus_fw", "", 0.01, 0.25, 0.399, 0.97 );
   TPad* pad_trr_harp_piminus_la = new TPad( "pad_trr_harp_piminus_la", "", 0.401, 0.02, 0.99, 0.97 );
   cnv_trr_harp_piminus->cd();
   ltx_piminus->Draw();
   pad_trr_harp_piminus_fw->Draw();   
   pad_trr_harp_piminus_fw->Divide(2,2,0.,0.);
   pad_trr_harp_piminus_la->Draw();   
   pad_trr_harp_piminus_la->Divide(3,3,0.,0.);
   TLegend* leg_trr_harp_piminus = new TLegend( 0.01, 0.02, 0.4, 0.24 );
   leg_trr_harp_piminus->SetTextFont(62);
   leg_trr_harp_piminus->SetTextSize(0.02);
   leg_trr_harp_piminus->SetFillColor(kWhite);
   
   TH1D*  h[2];
   TH1D*  hex[2];
   double chisq[2];
   double ndf[2];
   
   // FW production
   //
   for ( int i=0; i<4; ++i )
   {
      // secondary pi+
      h[0] = 0;
      double max1 = -1;
      std::string hsubname = "HARP/piplus_FW_" + std::to_string(i);
      std::string hname = model + def + hsubname;
      h[0] = GetHisto( hname, max1 );
      if ( !h[0] ) continue;
      hex[0] = 0;
      hex[0] = GetExpHARP( h[0], piplus_HARP, max1 );
      h[0]->SetLineColor(kRed);
      h[0]->GetXaxis()->SetRangeUser(0.5,5.);
      pad_rsc_harp_piplus_fw->cd(i+1);
      h[0]->Draw("histE1");
      //
      //DrawVariantsHARP( NRsc, drsc, model, hsubname, colrsc, max1 );
      //h[0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
      //pad_fsc_harp_piplus_la->Update();
      //cnv_fsc_harp_piplus->Update();
      //if ( i == 3 ) VariantsHARP2Legend( NRsc, drsc, model, "HARP/piplus_RecordChi2", rsclabel,colrsc, leg_rsc_harp_piplus );  
      //
      pad_xssc_harp_piplus_fw->cd(i+1);
      h[0]->Draw("histE1");
      //
      pad_fsc_harp_piplus_fw->cd(i+1);
      h[0]->Draw("histE1");
      //
      pad_gqdsc_harp_piplus_fw->cd(i+1);
      h[0]->Draw("histE1");
      //
      pad_trr_harp_piplus_fw->cd(i+1);
      h[0]->Draw("histE1");
      // secondary pi-
      h[1] = 0;
      double max2 = -1;
      hsubname = "HARP/piminus_FW_" + std::to_string(i);
      hname = model + def + hsubname;
      h[1] = GetHisto( hname, max2 );
      if ( !h[1] ) continue;
      hex[1] = 0;
      hex[1] = GetExpHARP( h[1], piminus_HARP, max2 );
      h[1]->SetLineColor(kRed);
      h[1]->GetXaxis()->SetRangeUser(0.5,5.);
      pad_rsc_harp_piminus_fw->cd(i+1);
      h[1]->Draw("histE1");
      //
      //DrawVariantsHARP( NRsc, drsc, model, hsubname, colrsc, max2 );
      //h[1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
      //pad_fsc_harp_piminus_la->Update();
      //cnv_fsc_harp_piminus->Update();
      //if ( i == 3 ) VariantsHARP2Legend( NRsc, drsc, model, "HARP/piminus_RecordChi2", rsclabel,colrsc, leg_rsc_harp_piminus );  
      //
      pad_xssc_harp_piminus_fw->cd(i+1);
      h[1]->Draw("histE1");
      //
      pad_fsc_harp_piminus_fw->cd(i+1);
      h[1]->Draw("histE1");
      //
      pad_gqdsc_harp_piminus_fw->cd(i+1);
      h[1]->Draw("histE1");
      //
      pad_trr_harp_piminus_fw->cd(i+1);
      h[1]->Draw("histE1");
      //
      // RadiusScale variants 
      //
      TH1D* h1[NRsc][2];
      double chisq1[NRsc][2];
      double ndf1[NRsc][2];
      for ( int j=0; j<NRsc; ++j )
      {
         // secondary pi+
	 h1[j][0] = 0;
	 hname = model + drsc[j] + "HARP/piplus_FW_" + std::to_string(i);
	 h1[j][0] = GetHisto( hname, max1 );
	 if ( !h1[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h1[j1][0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 h1[j][0]->SetLineColor(colrsc[j]);
         pad_rsc_harp_piplus_fw->cd(i+1);
         h1[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_rsc_harp_piplus_fw->Update();
	 cnv_rsc_harp_piplus->Update();
	 // secondary pi-
	 h1[j][1] = 0;
	 hname = model + drsc[j] + "HARP/piminus_FW_" + std::to_string(i);
	 h1[j][1] = GetHisto( hname, max2 );
	 if ( !h1[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h1[j1][1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 h1[j][1]->SetLineColor(colrsc[j]);
         pad_rsc_harp_piminus_fw->cd(i+1);
         h1[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_rsc_harp_piminus_fw->Update();
	 cnv_rsc_harp_piminus->Update();
	 // fill up legend
	 if ( i == 3 )
	 {
	    chisq1[j][0] = 0.;
	    ndf1[j][0] = 0.;
	    std::string rchi2name = model + drsc[j] + "HARP/piplus_RecordChi2";
            RecordChi2* rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq1[j][0] = rchi2->GetSumChi2();
	       ndf1[j][0]  = rchi2->GetSumNDF();
	    }
	    leg_rsc_harp_piplus->AddEntry( h1[j][0], Form( "%s, chi2/NDF=%.2f", rsclabel[j].c_str(), chisq1[j][0]/ndf1[j][0] ), "L" );
	    chisq1[j][1] = 0.;
	    ndf1[j][1] = 0.;
	    rchi2name = model + drsc[j] + "HARP/piminus_RecordChi2";
            rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq1[j][1] = rchi2->GetSumChi2();
	       ndf1[j][1]  = rchi2->GetSumNDF();
	    }
	    leg_rsc_harp_piminus->AddEntry( h1[j][1], Form( "%s, chi2/NDF=%.2f", rsclabel[j].c_str(), chisq1[j][1]/ndf1[j][1] ), "L" );
	 }
      }
      //
      // XSecScale variants
      //
      TH1D* h2[NXSsc][2];
      double chisq2[NXSsc][2];
      double ndf2[NXSsc][2];
      for ( int j=0; j<NXSsc; ++j )
      {
         // secondary pi+
	 h2[j][0] = 0;
	 hname = model + dxssc[j] + "HARP/piplus_FW_" + std::to_string(i);
	 h2[j][0] = GetHisto( hname, max1 );
	 if ( !h2[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h2[j1][0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 h2[j][0]->SetLineColor(colxssc[j]);
         pad_xssc_harp_piplus_fw->cd(i+1);
         h2[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_xssc_harp_piplus_fw->Update();
	 cnv_xssc_harp_piplus->Update();
	 // secondary pi-
	 h2[j][1] = 0;
	 hname = model + dxssc[j] + "HARP/piminus_FW_" + std::to_string(i);
	 h2[j][1] = GetHisto( hname, max2 );
	 if ( !h2[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h2[j1][1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 h2[j][1]->SetLineColor(colxssc[j]);
         pad_xssc_harp_piminus_fw->cd(i+1);
         h2[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_xssc_harp_piminus_fw->Update();
	 cnv_xssc_harp_piminus->Update();
	 // fill legend
	 if ( i== 3 )
	 {
	    chisq2[j][0] = 0.;
	    ndf2[j][0] = 0.;
	    std::string rchi2name = model + dxssc[j] + "HARP/piplus_RecordChi2";
            RecordChi2* rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq2[j][0] = rchi2->GetSumChi2();
	       ndf2[j][0]  = rchi2->GetSumNDF();
	    }
	    leg_xssc_harp_piplus->AddEntry( h2[j][0], Form( "%s, chi2/NDF=%.2f", xssclabel[j].c_str(), chisq2[j][0]/ndf2[j][0] ), "L" );
	    chisq2[j][1] = 0.;
	    ndf2[j][1] = 0.;
	    rchi2name = model + dxssc[j] + "HARP/piminus_RecordChi2";
            rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq2[j][1] = rchi2->GetSumChi2();
	       ndf2[j][1]  = rchi2->GetSumNDF();
	    }
	    leg_xssc_harp_piminus->AddEntry( h2[j][1], Form( "%s, chi2/NDF=%.2f", xssclabel[j].c_str(), chisq2[j][1]/ndf2[j][1] ), "L" );
	 }
      }
      //
      // FermiScale variants
      //
      TH1D* h3[NFsc][2];
      double chisq3[NFsc][2];
      double ndf3[NFsc][2];
      for ( int j=0; j<NFsc; ++j )
      {
         // secondary pi+
	 h3[j][0] = 0;
	 hname = model + dfsc[j] + "HARP/piplus_FW_" + std::to_string(i);
	 h3[j][0] = GetHisto( hname, max1 );
	 if ( !h3[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h3[j1][0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 h3[j][0]->SetLineColor(colfsc[j]);
         pad_fsc_harp_piplus_fw->cd(i+1);
         h3[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_fsc_harp_piplus_fw->Update();
	 cnv_fsc_harp_piplus->Update();
	 // secondary pi-
	 h3[j][1] = 0;
	 hname = model + dfsc[j] + "HARP/piminus_FW_" + std::to_string(i);
	 h3[j][1] = GetHisto( hname, max2 );
	 if ( !h3[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h3[j1][1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 h3[j][1]->SetLineColor(colfsc[j]);
         pad_fsc_harp_piminus_fw->cd(i+1);
         h3[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_fsc_harp_piminus_fw->Update();
	 cnv_fsc_harp_piminus->Update();
	 // fill legend
	 if ( i == 3 )
	 {
//	    leg_fsc_harp_piplus->AddEntry( h3[j][0], dfsc[j].c_str(), "L" );
//	    leg_fsc_harp_piminus->AddEntry( h3[j][1], dfsc[j].c_str(), "L" );
	    chisq3[j][0] = 0.;
	    ndf3[j][0] = 0.;
	    std::string rchi2name = model + dfsc[j] + "HARP/piplus_RecordChi2";
            RecordChi2* rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq3[j][0] = rchi2->GetSumChi2();
	       ndf3[j][0]  = rchi2->GetSumNDF();
	    }
	    leg_fsc_harp_piplus->AddEntry( h3[j][0], Form( "%s, chi2/NDF=%.2f", fsclabel[j].c_str(), chisq3[j][0]/ndf3[j][0] ), "L" );
	    chisq3[j][1] = 0.;
	    ndf3[j][1] = 0.;
	    rchi2name = model + dfsc[j] + "HARP/piminus_RecordChi2";
            rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq3[j][1] = rchi2->GetSumChi2();
	       ndf3[j][1]  = rchi2->GetSumNDF();
	    }
	    leg_fsc_harp_piminus->AddEntry( h3[j][1], Form( "%s, chi2/NDF=%.2f", fsclabel[j].c_str(), chisq3[j][1]/ndf3[j][1] ), "L" );
	 }
      }
      //
      // GammaQDScale variants
      //
      TH1D* h4[NGQDsc][2];
      double chisq4[NGQDsc][2];
      double ndf4[NGQDsc][2];
      for ( int j=0; j<NGQDsc; ++j )
      {
         // secondary pi+
	 h4[j][0] = 0;
	 hname = model + dgqdsc[j] + "HARP/piplus_FW_" + std::to_string(i);
	 h4[j][0] = GetHisto( hname, max1 );
	 if ( !h4[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h4[j1][0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 h4[j][0]->SetLineColor(colgqdsc[j]);
         pad_gqdsc_harp_piplus_fw->cd(i+1);
         h4[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_gqdsc_harp_piplus_fw->Update();
	 cnv_gqdsc_harp_piplus->Update();
	 // secondary pi-
	 h4[j][1] = 0;
	 hname = model + dgqdsc[j] + "HARP/piminus_FW_" + std::to_string(i);
	 h4[j][1] = GetHisto( hname, max2 );
	 if ( !h4[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h4[j1][1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 h4[j][1]->SetLineColor(colgqdsc[j]);
         pad_gqdsc_harp_piminus_fw->cd(i+1);
         h4[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_gqdsc_harp_piminus_fw->Update();
	 cnv_gqdsc_harp_piminus->Update();
	 // fill up legend
	 if ( i == 3 )
	 {
	    chisq4[j][0] = 0.;
	    ndf4[j][0] = 0.;
	    std::string rchi2name = model + dgqdsc[j] + "HARP/piplus_RecordChi2";
            RecordChi2* rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq4[j][0] = rchi2->GetSumChi2();
	       ndf4[j][0]  = rchi2->GetSumNDF();
	    }
	    leg_gqdsc_harp_piplus->AddEntry( h4[j][0], Form( "%s, chi2/NDF=%.2f", gqdsclabel[j].c_str(), chisq4[j][0]/ndf4[j][0] ), "L" );
	    chisq4[j][1] = 0.;
	    ndf4[j][1] = 0.;
	    rchi2name = model + dgqdsc[j] + "HARP/piminus_RecordChi2";
            rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq4[j][1] = rchi2->GetSumChi2();
	       ndf4[j][1]  = rchi2->GetSumNDF();
	    }
	    leg_gqdsc_harp_piminus->AddEntry( h4[j][1], Form( "%s, chi2/NDF=%.2f", gqdsclabel[j].c_str(), chisq4[j][1]/ndf4[j][1] ), "L" );
	 }
      }
      //
      // TrailingRadius
      //
      TH1D* h5[NTrR][2];
      double chisq5[NTrR][2];
      double ndf5[NTrR][2];
      for ( int j=0; j<NTrR; ++j )
      {
         // secondary pi+
	 h5[j][0] = 0;
	 hname = model + dtrr[j] + "HARP/piplus_FW_" + std::to_string(i);
	 h5[j][0] = GetHisto( hname, max1 );
	 if ( !h5[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h5[j1][0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 h5[j][0]->SetLineColor(coltrr[j]);
	 h5[j][0]->SetLineColor(coltrr[j]);
         pad_trr_harp_piplus_fw->cd(i+1);
         h5[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_trr_harp_piplus_fw->Update();
	 cnv_trr_harp_piplus->Update();
	 // secondary pi-
	 h5[j][1] = 0;
	 hname = model + dtrr[j] + "HARP/piminus_FW_" + std::to_string(i);
	 h5[j][1] = GetHisto( hname, max2 );
	 if ( !h5[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h5[j1][1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 h5[j][1]->SetLineColor(coltrr[j]);
         pad_trr_harp_piminus_fw->cd(i+1);
         h5[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_trr_harp_piminus_fw->Update();
	 cnv_trr_harp_piminus->Update();
	 // fill up legend
	 if ( i == 3 )
	 {
	    chisq5[j][0] = 0.;
	    ndf5[j][0] = 0.;
	    std::string rchi2name = model + dtrr[j] + "HARP/piplus_RecordChi2";
            RecordChi2* rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq5[j][0] = rchi2->GetSumChi2();
	       ndf5[j][0]  = rchi2->GetSumNDF();
	    }
	    leg_trr_harp_piplus->AddEntry( h5[j][0], Form( "%s, chi2/NDF=%.2f", trrlabel[j].c_str(), chisq5[j][0]/ndf5[j][0] ), "L" );
	    chisq5[j][1] = 0.;
	    ndf5[j][1] = 0.;
	    rchi2name = model + dtrr[j] + "HARP/piminus_RecordChi2";
            rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq5[j][1] = rchi2->GetSumChi2();
	       ndf5[j][1]  = rchi2->GetSumNDF();
	    }
	    leg_trr_harp_piminus->AddEntry( h5[j][1], Form( "%s, chi2/NDF=%.2f", trrlabel[j].c_str(), chisq5[j][1]/ndf5[j][1] ), "L" );
	 }
      }
      // final beautifications
      if ( i == 3 )
      {
         chisq[0] = 0.;
	 ndf[0] = 0.;
	 std::string rchi2name = model + def + "HARP/piplus_RecordChi2";
         RecordChi2* rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	 if ( rchi2 )
	 {
	    chisq[0] = rchi2->GetSumChi2();
	    ndf[0]  = rchi2->GetSumNDF();
	 }
	 chisq[1] = 0.;
	 ndf[1]  = 0.;
         rchi2name = model + def + "HARP/piminus_RecordChi2";
         rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	 if ( rchi2 )
	 {
	    chisq[1] = rchi2->GetSumChi2();
	    ndf[1]  = rchi2->GetSumNDF();
	 }
	 leg_rsc_harp_piplus->AddEntry( h[0], Form("Default (RadSc=2.8), chi2/NDF=%.2f",(chisq[0]/ndf[0])), "L" );
	 leg_rsc_harp_piminus->AddEntry( h[1], Form("Default (RadSc=2.8), chi2/NDF=%.2f",(chisq[1]/ndf[1])), "L" );
	 leg_xssc_harp_piplus->AddEntry( h[0], Form("Default (XSecSc=1.), chi2/NDF=%.2f",(chisq[0]/ndf[0])), "L" );
	 leg_xssc_harp_piminus->AddEntry( h[1], Form("Default (XSecSc=1.), chi2/NDF=%.2f",(chisq[1]/ndf[1])), "L" );
	 leg_fsc_harp_piplus->AddEntry( h[0], Form("Default (FermiSc=0.685), chi2/NDF=%.2f",(chisq[0]/ndf[0])), "L" );
	 leg_fsc_harp_piminus->AddEntry( h[1], Form("Default (FermiSc=0.685), chi2/NDF=%.2f",(chisq[1]/ndf[1])), "L" );
	 leg_gqdsc_harp_piplus->AddEntry( h[0], Form("Default (GammaQDSc=1.), chi2/NDF=%.2f",(chisq[0]/ndf[0])), "L" );
	 leg_gqdsc_harp_piminus->AddEntry( h[1], Form("Default (GammaQDSc=1.), chi2/NDF=%.2f",(chisq[1]/ndf[1])), "L" );
	 leg_trr_harp_piplus->AddEntry( h[0], Form("Default (TrailingR=0.), chi2/NDF=%.2f",(chisq[0]/ndf[0])), "L" );
	 leg_trr_harp_piminus->AddEntry( h[1], Form("Default (TrailingR=0.), chi2/NDF=%.2f",(chisq[1]/ndf[1])), "L" );
      }
      // repeat main plot to bring it up
      pad_rsc_harp_piplus_fw->cd(i+1);
      h[0]->Draw("histE1same");
      pad_rsc_harp_piminus_fw->cd(i+1);
      h[1]->Draw("histE1same");
      pad_xssc_harp_piplus_fw->cd(i+1);
      h[0]->Draw("histE1same");
      pad_xssc_harp_piminus_fw->cd(i+1);
      h[1]->Draw("histE1same");
      pad_fsc_harp_piplus_fw->cd(i+1);
      h[0]->Draw("histE1same");
      pad_fsc_harp_piminus_fw->cd(i+1);
      h[1]->Draw("histE1same");
      pad_gqdsc_harp_piplus_fw->cd(i+1);
      h[0]->Draw("histE1same");
      pad_gqdsc_harp_piminus_fw->cd(i+1);
      h[1]->Draw("histE1same");
      pad_trr_harp_piplus_fw->cd(i+1);
      h[0]->Draw("histE1same");
      pad_trr_harp_piminus_fw->cd(i+1);
      h[1]->Draw("histE1same");
      if ( hex[0] )
      {
         pad_rsc_harp_piplus_fw->cd(i+1);
         hex[0]->Draw("psame");
         pad_xssc_harp_piplus_fw->cd(i+1);
         hex[0]->Draw("psame");
         pad_fsc_harp_piplus_fw->cd(i+1);
         hex[0]->Draw("psame");
         pad_gqdsc_harp_piplus_fw->cd(i+1);
         hex[0]->Draw("psame");
         pad_trr_harp_piplus_fw->cd(i+1);
         hex[0]->Draw("psame");
      }
      if ( hex[1] )
      {
         pad_rsc_harp_piminus_fw->cd(i+1);
         hex[1]->Draw("psame");
         pad_xssc_harp_piminus_fw->cd(i+1);
         hex[1]->Draw("psame");
         pad_fsc_harp_piminus_fw->cd(i+1);
         hex[1]->Draw("psame");
         pad_gqdsc_harp_piminus_fw->cd(i+1);
         hex[1]->Draw("psame");
         pad_trr_harp_piminus_fw->cd(i+1);
         hex[1]->Draw("psame");
      }
      if ( i == 3 )
      {
	 leg_rsc_harp_piplus->AddEntry( hex[0], "HARP data", "p" );
	 leg_rsc_harp_piminus->AddEntry( hex[1], "HARP data", "p" );
	 leg_xssc_harp_piplus->AddEntry( hex[0], "HARP data", "p" );
	 leg_xssc_harp_piminus->AddEntry( hex[1], "HARP data", "p" );
	 leg_fsc_harp_piplus->AddEntry( hex[0], "HARP data", "p" );
	 leg_fsc_harp_piminus->AddEntry( hex[1], "HARP data", "p" );
	 leg_gqdsc_harp_piplus->AddEntry( hex[0], "HARP data", "p" );
	 leg_gqdsc_harp_piminus->AddEntry( hex[1], "HARP data", "p" );
	 leg_trr_harp_piplus->AddEntry( hex[0], "HARP data", "p" );
	 leg_trr_harp_piminus->AddEntry( hex[1], "HARP data", "p" );
      }
   }
   
   cnv_rsc_harp_piplus->cd();
   leg_rsc_harp_piplus->Draw();
   cnv_rsc_harp_piminus->cd();
   leg_rsc_harp_piminus->Draw();   

   cnv_xssc_harp_piplus->cd();
   leg_xssc_harp_piplus->Draw();
   cnv_xssc_harp_piminus->cd();
   leg_xssc_harp_piminus->Draw();   

   cnv_fsc_harp_piplus->cd();
   leg_fsc_harp_piplus->Draw();
   cnv_fsc_harp_piminus->cd();
   leg_fsc_harp_piminus->Draw();   

   cnv_gqdsc_harp_piplus->cd();
   leg_gqdsc_harp_piplus->Draw();
   cnv_gqdsc_harp_piminus->cd();
   leg_gqdsc_harp_piminus->Draw();   

   cnv_trr_harp_piplus->cd();
   leg_trr_harp_piplus->Draw();
   cnv_trr_harp_piminus->cd();
   leg_trr_harp_piminus->Draw();   
      
   // LA production
   //

   for ( int i=0; i<9; ++i )
   {
      // secondary pi+
      h[0] = 0;
      double max1 = -1;
      std::string hname = model + def + "HARP/piplus_LA_" + std::to_string(i);
      h[0] = GetHisto( hname, max1 );
      if ( !h[0] ) continue;
      hex[0] = 0;
      hex[0] = GetExpHARP( h[0], piplus_HARP, max1 );
      h[0]->SetLineColor(kRed);
      pad_rsc_harp_piplus_la->cd(i+1);
      h[0]->Draw("histE1");
      pad_xssc_harp_piplus_la->cd(i+1);
      h[0]->Draw("histE1");
      pad_fsc_harp_piplus_la->cd(i+1);
      h[0]->Draw("histE1");
      pad_gqdsc_harp_piplus_la->cd(i+1);
      h[0]->Draw("histE1");
      pad_trr_harp_piplus_la->cd(i+1);
      h[0]->Draw("histE1");
      // secondary pi-
      h[1] = 0;
      double max2 = -1;
      hname = model + def + "HARP/piminus_LA_" + std::to_string(i);
      h[1] = GetHisto( hname, max2 );
      if ( !h[1] ) continue;
      hex[1] = 0;
      hex[1] = GetExpHARP( h[1], piminus_HARP, max2 );
      h[1]->SetLineColor(kRed);
      pad_rsc_harp_piminus_la->cd(i+1);
      h[1]->Draw("histE1");
      pad_xssc_harp_piminus_la->cd(i+1);
      h[1]->Draw("histE1");
      pad_fsc_harp_piminus_la->cd(i+1);
      h[1]->Draw("histE1");
      pad_gqdsc_harp_piminus_la->cd(i+1);
      h[1]->Draw("histE1");
      pad_trr_harp_piminus_la->cd(i+1);
      h[1]->Draw("histE1");
      //
      // RadiusScale variants 
      //
      TH1D* h1[NRsc][2];
      for ( int j=0; j<NRsc; ++j )
      {
         // secondary pi+
	 h1[j][0] = 0;
	 hname = model + drsc[j] + "HARP/piplus_LA_" + std::to_string(i);
	 h1[j][0] = GetHisto( hname, max1 );
	 if ( !h1[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h1[j1][0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 h1[j][0]->SetLineColor(colrsc[j]);
         pad_rsc_harp_piplus_la->cd(i+1);
         h1[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_rsc_harp_piplus_la->Update();
	 cnv_rsc_harp_piplus->Update();
	 // secondary pi-
	 h1[j][1] = 0;
	 hname = model + drsc[j] + "HARP/piminus_LA_" + std::to_string(i);
	 h1[j][1] = GetHisto( hname, max2 );
	 if ( !h1[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h1[j1][1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 h1[j][1]->SetLineColor(colrsc[j]);
         pad_rsc_harp_piminus_la->cd(i+1);
         h1[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_rsc_harp_piminus_la->Update();
	 cnv_rsc_harp_piminus->Update();
      }
      //
      // XSecScale variants
      //
      TH1D* h2[NXSsc][2];
      for ( int j=0; j<NXSsc; ++j )
      {
         // secondary pi+
	 h2[j][0] = 0;
	 hname = model + dxssc[j] + "HARP/piplus_LA_" + std::to_string(i);
	 h2[j][0] = GetHisto( hname, max1 );
	 if ( !h2[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h2[j1][0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 h2[j][0]->SetLineColor(colxssc[j]);
         pad_xssc_harp_piplus_la->cd(i+1);
         h2[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_xssc_harp_piplus_la->Update();
	 cnv_xssc_harp_piplus->Update();
	 // secondary pi-
	 h2[j][1] = 0;
	 hname = model + dxssc[j] + "HARP/piminus_LA_" + std::to_string(i);
	 h2[j][1] = GetHisto( hname, max2 );
	 if ( !h2[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h2[j1][1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 h2[j][1]->SetLineColor(colxssc[j]);
         pad_xssc_harp_piminus_la->cd(i+1);
         h2[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_xssc_harp_piminus_la->Update();
	 cnv_xssc_harp_piminus->Update();
      }
      //
      // FermiScale variants
      //
      TH1D* h3[NFsc][2];
      for ( int j=0; j<NFsc; ++j )
      {
         // secondary pi+
	 h3[j][0] = 0;
	 hname = model + dfsc[j] + "HARP/piplus_LA_" + std::to_string(i);
	 h3[j][0] = GetHisto( hname, max1 );
	 if ( !h3[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h3[j1][0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 h3[j][0]->SetLineColor(colfsc[j]);
         pad_fsc_harp_piplus_la->cd(i+1);
         h3[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_fsc_harp_piplus_la->Update();
	 cnv_fsc_harp_piplus->Update();
	 // secondary pi-
	 h3[j][1] = 0;
	 hname = model + dfsc[j] + "HARP/piminus_LA_" + std::to_string(i);
	 h3[j][1] = GetHisto( hname, max2 );
	 if ( !h3[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h3[j1][1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 h3[j][1]->SetLineColor(colfsc[j]);
         pad_fsc_harp_piminus_la->cd(i+1);
         h3[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_fsc_harp_piminus_la->Update();
	 cnv_fsc_harp_piminus->Update();
      }
      //
      // GammaQDScale 
      //
      TH1D* h4[NGQDsc][2];
      for ( int j=0; j<NGQDsc; ++j )
      {
         // secondary pi+
	 h4[j][0] = 0;
	 hname = model + dgqdsc[j] + "HARP/piplus_LA_" + std::to_string(i);
	 h4[j][0] = GetHisto( hname, max1 );
	 if ( !h4[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h4[j1][0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 h4[j][0]->SetLineColor(colgqdsc[j]);
         pad_gqdsc_harp_piplus_la->cd(i+1);
         h4[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_gqdsc_harp_piplus_la->Update();
	 cnv_gqdsc_harp_piplus->Update();
	 // secondary pi-
	 h4[j][1] = 0;
	 hname = model + dgqdsc[j] + "HARP/piminus_LA_" + std::to_string(i);
	 h4[j][1] = GetHisto( hname, max2 );
	 if ( !h4[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h4[j1][1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 h4[j][1]->SetLineColor(colgqdsc[j]);
         pad_gqdsc_harp_piminus_la->cd(i+1);
         h4[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_gqdsc_harp_piminus_la->Update();
	 cnv_gqdsc_harp_piminus->Update();
      }
      //
      // TrailingRadius (dynamic density effect)
      // 
      TH1D* h5[NTrR][2];
      for ( int j=0; j<NTrR; ++j )
      {
         // secondary pi+
	 h5[j][0] = 0;
	 hname = model + dtrr[j] + "HARP/piplus_LA_" + std::to_string(i);
	 h5[j][0] = GetHisto( hname, max1 );
	 if ( !h5[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h5[j1][0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 0., max1*1.1 );
	 h5[j][0]->SetLineColor(coltrr[j]);
         pad_trr_harp_piplus_la->cd(i+1);
         h5[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_trr_harp_piplus_la->Update();
	 cnv_trr_harp_piplus->Update();
	 // secondary pi-
	 h5[j][1] = 0;
	 hname = model + dtrr[j] + "HARP/piminus_LA_" + std::to_string(i);
	 h5[j][1] = GetHisto( hname, max2 );
	 if ( !h5[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h5[j1][1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 0., max2*1.1 );
	 h5[j][1]->SetLineColor(coltrr[j]);
         pad_trr_harp_piminus_la->cd(i+1);
         h5[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_trr_harp_piminus_la->Update();
	 cnv_trr_harp_piminus->Update();
      }    
      // repeat main plot to bring it up
      pad_rsc_harp_piplus_la->cd(i+1);
      h[0]->Draw("histE1same");
      pad_rsc_harp_piminus_la->cd(i+1);
      h[1]->Draw("histE1same");
      pad_xssc_harp_piplus_la->cd(i+1);
      h[0]->Draw("histE1same");
      pad_xssc_harp_piminus_la->cd(i+1);
      h[1]->Draw("histE1same");
      pad_fsc_harp_piplus_la->cd(i+1);
      h[0]->Draw("histE1same");
      pad_fsc_harp_piminus_la->cd(i+1);
      h[1]->Draw("histE1same");
      pad_gqdsc_harp_piplus_la->cd(i+1);
      h[0]->Draw("histE1same");
      pad_gqdsc_harp_piminus_la->cd(i+1);
      h[1]->Draw("histE1same");
      pad_trr_harp_piplus_la->cd(i+1);
      h[0]->Draw("histE1same");
      pad_trr_harp_piminus_la->cd(i+1);
      h[1]->Draw("histE1same");
      if ( hex[0] )
      {
         pad_rsc_harp_piplus_la->cd(i+1);
         hex[0]->Draw("psame");
         pad_xssc_harp_piplus_la->cd(i+1);
         hex[0]->Draw("psame");
         pad_fsc_harp_piplus_la->cd(i+1);
         hex[0]->Draw("psame");
         pad_gqdsc_harp_piplus_la->cd(i+1);
         hex[0]->Draw("psame");
         pad_trr_harp_piplus_la->cd(i+1);
         hex[0]->Draw("psame");
      }
      if ( hex[1] )
      {
         pad_rsc_harp_piminus_la->cd(i+1);
         hex[1]->Draw("psame");
         pad_xssc_harp_piminus_la->cd(i+1);
         hex[1]->Draw("psame");
         pad_fsc_harp_piminus_la->cd(i+1);
         hex[1]->Draw("psame");
         pad_gqdsc_harp_piminus_la->cd(i+1);
         hex[1]->Draw("psame");
         pad_trr_harp_piminus_la->cd(i+1);
         hex[1]->Draw("psame");
      }
   }

   // ITEP business
   //
   // HARP exp. data/records
   //
   TIter     next1( hfile->GetDirectory( "BertiniDefaultITEP/ExpData" )->GetListOfKeys() );
   TKey*     key1 = (TKey*)next1();
   TH1D*     hexp1 = 0;

   while ( key1 )
   {
      if ( !(TClass::GetClass(key1->GetClassName())->InheritsFrom(TH1::Class())) ) 
      {
         key1 = (TKey*)next1();
	 continue;
      }
      hexp1 = (TH1D*)key1->ReadObj();
      std::string htitle1 = hexp1->GetTitle();
      if ( htitle1.find( "Production of proton" ) != std::string::npos ) proton_ITEP.push_back(hexp1);
      if ( htitle1.find( "Production of neutron" ) != std::string::npos ) neutron_ITEP.push_back(hexp1);
      key1 = (TKey*)next1();
   }
   
   std::string title_proton = gtitle + " Secondary: 2212";
   TLatex* ltx_proton = new TLatex( 0.15, 0.98, title_proton.c_str() );
   ltx_proton->SetTextFont(62);
   ltx_proton->SetTextSize(0.03); 
   std::string title_neutron = gtitle + " Secondary: 2112";
   TLatex* ltx_neutron = new TLatex( 0.15, 0.98, title_neutron.c_str() );
   ltx_neutron->SetTextFont(62);
   ltx_neutron->SetTextSize(0.03); 
   //
   // RadiusScale variants - TCanvas, etc.
   //
   // secondary proton
   //
   TCanvas* cnv_rsc_itep_proton = new TCanvas( "cnv_rsc_itep_proton", "", 1000, 700 );
   TPad* pad_rsc_itep_proton = new TPad( "pad_rsc_itep_proton", "", 0.1, 0.25, 0.9, 0.97 );
   cnv_rsc_itep_proton->cd();
   ltx_proton->Draw();
   pad_rsc_itep_proton->Draw();   
   pad_rsc_itep_proton->Divide(2,2,0.,0.);
   TLegend* leg_rsc_itep_proton = new TLegend( 0.2, 0.02, 0.6, 0.24 );
   leg_rsc_itep_proton->SetTextFont(62);
   leg_rsc_itep_proton->SetTextSize(0.02);
   leg_rsc_itep_proton->SetFillColor(kWhite);
   //
   // secondary neutron
   //
   TCanvas* cnv_rsc_itep_neutron = new TCanvas( "cnv_rsc_itep_neutron", "", 1000, 700 );
   TPad* pad_rsc_itep_neutron = new TPad( "pad_rsc_itep_neutron", "", 0.1, 0.25, 0.9, 0.97 );
   cnv_rsc_itep_neutron->cd();
   ltx_neutron->Draw();
   pad_rsc_itep_neutron->Draw();   
   pad_rsc_itep_neutron->Divide(2,2,0.,0.);
   TLegend* leg_rsc_itep_neutron = new TLegend( 0.2, 0.02, 0.6, 0.24 );
   leg_rsc_itep_neutron->SetTextFont(62);
   leg_rsc_itep_neutron->SetTextSize(0.02);
   leg_rsc_itep_neutron->SetFillColor(kWhite);
   //
   // XSecScale variants - TCanvas, etc.
   //
   // secondary proton
   //
   TCanvas* cnv_xssc_itep_proton = new TCanvas( "cnv_xssc_itep_proton", "", 1000, 700 );
   TPad* pad_xssc_itep_proton = new TPad( "pad_xssc_itep_proton", "", 0.1, 0.25, 0.9, 0.97 );
   cnv_xssc_itep_proton->cd();
   ltx_proton->Draw();
   pad_xssc_itep_proton->Draw();   
   pad_xssc_itep_proton->Divide(2,2,0.,0.);
   TLegend* leg_xssc_itep_proton = new TLegend( 0.2, 0.02, 0.6, 0.24 );
   leg_xssc_itep_proton->SetTextFont(62);
   leg_xssc_itep_proton->SetTextSize(0.02);
   leg_xssc_itep_proton->SetFillColor(kWhite);
   //
   // secondary neutron
   //
   TCanvas* cnv_xssc_itep_neutron = new TCanvas( "cnv_xssc_itep_neutron", "", 1000, 700 );
   TPad* pad_xssc_itep_neutron = new TPad( "pad_xssc_itep_neutron", "", 0.1, 0.25, 0.9, 0.97 );
   cnv_xssc_itep_neutron->cd();
   ltx_neutron->Draw();
   pad_xssc_itep_neutron->Draw();   
   pad_xssc_itep_neutron->Divide(2,2,0.,0.);
   TLegend* leg_xssc_itep_neutron = new TLegend( 0.2, 0.02, 0.6, 0.24 );
   leg_xssc_itep_neutron->SetTextFont(62);
   leg_xssc_itep_neutron->SetTextSize(0.02);
   leg_xssc_itep_neutron->SetFillColor(kWhite);
   //
   // FermiScale variants - TCanvas, etc.
   //
   // secondary proton
   //
   TCanvas* cnv_fsc_itep_proton = new TCanvas( "cnv_fsc_itep_proton", "", 1000, 700 );
   TPad* pad_fsc_itep_proton = new TPad( "pad_fsc_itep_proton", "", 0.1, 0.25, 0.9, 0.97 );
   cnv_fsc_itep_proton->cd();
   ltx_proton->Draw();
   pad_fsc_itep_proton->Draw();   
   pad_fsc_itep_proton->Divide(2,2,0.,0.);
   TLegend* leg_fsc_itep_proton = new TLegend( 0.2, 0.02, 0.6, 0.24 );
   leg_fsc_itep_proton->SetTextFont(62);
   leg_fsc_itep_proton->SetTextSize(0.02);
   leg_fsc_itep_proton->SetFillColor(kWhite);
   //
   // secondary neutron
   //
   TCanvas* cnv_fsc_itep_neutron = new TCanvas( "cnv_fsc_itep_neutron", "", 1000, 700 );
   TPad* pad_fsc_itep_neutron = new TPad( "pad_fsc_itep_neutron", "", 0.1, 0.25, 0.9, 0.97 );
   cnv_fsc_itep_neutron->cd();
   ltx_neutron->Draw();
   pad_fsc_itep_neutron->Draw();   
   pad_fsc_itep_neutron->Divide(2,2,0.,0.);
   TLegend* leg_fsc_itep_neutron = new TLegend( 0.2, 0.02, 0.6, 0.24 );
   leg_fsc_itep_neutron->SetTextFont(62);
   leg_fsc_itep_neutron->SetTextSize(0.02);
   leg_fsc_itep_neutron->SetFillColor(kWhite);
   //
   // GammaQDScale variants - TCanvas, etc.
   //
   // secondary proton
   //
   TCanvas* cnv_gqdsc_itep_proton = new TCanvas( "cnv_gqdsc_itep_proton", "", 1000, 700 );
   TPad* pad_gqdsc_itep_proton = new TPad( "pad_gqdsc_itep_proton", "", 0.1, 0.25, 0.9, 0.97 );
   cnv_gqdsc_itep_proton->cd();
   ltx_proton->Draw();
   pad_gqdsc_itep_proton->Draw();   
   pad_gqdsc_itep_proton->Divide(2,2,0.,0.);
   TLegend* leg_gqdsc_itep_proton = new TLegend( 0.2, 0.02, 0.6, 0.24 );
   leg_gqdsc_itep_proton->SetTextFont(62);
   leg_gqdsc_itep_proton->SetTextSize(0.02);
   leg_gqdsc_itep_proton->SetFillColor(kWhite);
   //
   // secondary neutron
   //
   TCanvas* cnv_gqdsc_itep_neutron = new TCanvas( "cnv_gqdsc_itep_neutron", "", 1000, 700 );
   TPad* pad_gqdsc_itep_neutron = new TPad( "pad_gqdsc_itep_neutron", "", 0.1, 0.25, 0.9, 0.97 );
   cnv_gqdsc_itep_neutron->cd();
   ltx_neutron->Draw();
   pad_gqdsc_itep_neutron->Draw();   
   pad_gqdsc_itep_neutron->Divide(2,2,0.,0.);
   TLegend* leg_gqdsc_itep_neutron = new TLegend( 0.2, 0.02, 0.6, 0.24 );
   leg_gqdsc_itep_neutron->SetTextFont(62);
   leg_gqdsc_itep_neutron->SetTextSize(0.02);
   leg_gqdsc_itep_neutron->SetFillColor(kWhite);
   //
   // Trailing effect - TCanvas, etc.
   //
   // secondary proton
   //
   TCanvas* cnv_trr_itep_proton = new TCanvas( "cnv_trr_itep_proton", "", 1000, 700 );
   TPad* pad_trr_itep_proton = new TPad( "pad_trr_itep_proton", "", 0.1, 0.25, 0.9, 0.97 );
   cnv_trr_itep_proton->cd();
   ltx_proton->Draw();
   pad_trr_itep_proton->Draw();   
   pad_trr_itep_proton->Divide(2,2,0.,0.);
   TLegend* leg_trr_itep_proton = new TLegend( 0.2, 0.02, 0.6, 0.24 );
   leg_trr_itep_proton->SetTextFont(62);
   leg_trr_itep_proton->SetTextSize(0.02);
   leg_trr_itep_proton->SetFillColor(kWhite);
   //
   // secondary neutron
   //
   TCanvas* cnv_trr_itep_neutron = new TCanvas( "cnv_trr_itep_neutron", "", 1000, 700 );
   TPad* pad_trr_itep_neutron = new TPad( "pad_trr_itep_neutron", "", 0.1, 0.25, 0.9, 0.97 );
   cnv_trr_itep_neutron->cd();
   ltx_neutron->Draw();
   pad_trr_itep_neutron->Draw();   
   pad_trr_itep_neutron->Divide(2,2,0.,0.);
   TLegend* leg_trr_itep_neutron = new TLegend( 0.2, 0.02, 0.6, 0.24 );
   leg_trr_itep_neutron->SetTextFont(62);
   leg_trr_itep_neutron->SetTextSize(0.02);
   leg_trr_itep_neutron->SetFillColor(kWhite);
   
   const int NPlotsITEP = 4;
   std::string angles[NPlotsITEP] = { "59.1", "89", "119", "159.6" };
   
   for ( int i=0; i<NPlotsITEP; ++i )
   {
      // secondary proton
      h[0] = 0;
      double max1 = -1;
      std::string hname = model + def + "ITEP/proton_at_" + angles[i] + "deg";
      h[0] = GetHisto( hname, max1 );
      if ( !h[0] ) continue;
      hex[0] = 0;
      hex[0] = GetExpHARP( h[0], proton_ITEP, max1 );
      h[0]->SetLineColor(kRed);
      pad_rsc_itep_proton->cd(i+1);
      gPad->SetLogy();
      h[0]->Draw("histE1");
      pad_xssc_itep_proton->cd(i+1);
      gPad->SetLogy();
      h[0]->Draw("histE1");
      pad_fsc_itep_proton->cd(i+1);
      gPad->SetLogy();
      h[0]->Draw("histE1");
      pad_gqdsc_itep_proton->cd(i+1);
      gPad->SetLogy();
      h[0]->Draw("histE1");
      pad_trr_itep_proton->cd(i+1);
      gPad->SetLogy();
      h[0]->Draw("histE1");
      // secondary neutron
      h[1] = 0;
      double max2 = -1;
      hname = model + def + "ITEP/neutron_at_" + angles[i] + "deg";
      h[1] = GetHisto( hname, max2 );
      if ( !h[1] ) continue;
      hex[1] = 0;
      hex[1] = GetExpHARP( h[1], neutron_ITEP, max2 );
      h[1]->SetLineColor(kRed);
      pad_rsc_itep_neutron->cd(i+1);
      gPad->SetLogy();
      h[1]->Draw("histE1");
      pad_xssc_itep_neutron->cd(i+1);
      gPad->SetLogy();
      h[1]->Draw("histE1");
      pad_fsc_itep_neutron->cd(i+1);
      gPad->SetLogy();
      h[1]->Draw("histE1");
      pad_gqdsc_itep_neutron->cd(i+1);
      gPad->SetLogy();
      h[1]->Draw("histE1");
      pad_trr_itep_neutron->cd(i+1);
      gPad->SetLogy();
      h[1]->Draw("histE1");
      //
      // RadiusScale variants 
      //
      TH1D* h1[NRsc][2];
      double chisq1[NRsc][2];
      double ndf1[NRsc][2];
      for ( int j=0; j<NRsc; ++j )
      {
         // secondary proton
	 h1[j][0] = 0;
	 hname = model + drsc[j] + "ITEP/proton_at_" + angles[i] +"deg";
	 h1[j][0] = GetHisto( hname, max1 );
	 if ( !h1[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h1[j1][0]->GetYaxis()->SetRangeUser( 10., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 10., max1*1.1 );
	 h1[j][0]->SetLineColor(colrsc[j]);
         pad_rsc_itep_proton->cd(i+1);
         h1[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_rsc_itep_proton->Update();
	 cnv_rsc_itep_proton->Update();
         // secondary neutron
	 h1[j][1] = 0;
	 hname = model + drsc[j] + "ITEP/neutron_at_" + angles[i] +"deg";
	 h1[j][1] = GetHisto( hname, max2 );
	 if ( !h1[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h1[j1][1]->GetYaxis()->SetRangeUser( 10., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 10., max2*1.1 );
	 h1[j][1]->SetLineColor(colrsc[j]);
         pad_rsc_itep_neutron->cd(i+1);
         h1[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_rsc_itep_neutron->Update();
	 cnv_rsc_itep_neutron->Update();
	 // fill up legend
	 if ( i == 3 )
	 {
	    chisq1[j][0] = 0.;
	    ndf1[j][0] = 0.;
	    std::string rchi2name = model + drsc[j] + "ITEP/proton_RecordChi2";
            RecordChi2* rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq1[j][0] = rchi2->GetSumChi2();
	       ndf1[j][0]  = rchi2->GetSumNDF();
	    }
	    leg_rsc_itep_proton->AddEntry( h1[j][0], Form( "%s, chi2/NDF=%.2f", rsclabel[j].c_str(), chisq1[j][0]/ndf1[j][0] ), "L" );
	    chisq1[j][1] = 0.;
	    ndf1[j][1] = 0.;
	    rchi2name = model + drsc[j] + "ITEP/neutron_RecordChi2";
            rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq1[j][1] = rchi2->GetSumChi2();
	       ndf1[j][1]  = rchi2->GetSumNDF();
	    }
	    leg_rsc_itep_neutron->AddEntry( h1[j][1], Form( "%s, chi2/NDF=%.2f", rsclabel[j].c_str(), chisq1[j][1]/ndf1[j][1] ), "L" );
	 }
      }
      //
      // XSecScale variants
      //
      TH1D* h2[NXSsc][2];
      double chisq2[NXSsc][2];
      double ndf2[NXSsc][2];
      for ( int j=0; j<NXSsc; ++j )
      {
         // secondary proton
	 h2[j][0] = 0;
	 hname = model + dxssc[j] + "ITEP/proton_at_" + angles[i] +"deg";
	 h2[j][0] = GetHisto( hname, max1 );
	 if ( !h2[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h2[j1][0]->GetYaxis()->SetRangeUser( 10., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 10., max1*1.1 );
	 h2[j][0]->SetLineColor(colxssc[j]);
         pad_xssc_itep_proton->cd(i+1);
         h2[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_xssc_itep_proton->Update();
	 cnv_xssc_itep_proton->Update();
         // secondary neutron
	 h2[j][1] = 0;
	 hname = model + dxssc[j] + "ITEP/neutron_at_" + angles[i] +"deg";
	 h2[j][1] = GetHisto( hname, max2 );
	 if ( !h2[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h2[j1][1]->GetYaxis()->SetRangeUser( 10., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 10., max2*1.1 );
	 h2[j][1]->SetLineColor(colxssc[j]);
         pad_xssc_itep_neutron->cd(i+1);
         h2[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_xssc_itep_neutron->Update();
	 cnv_xssc_itep_neutron->Update();
	 // fill up legend
	 if ( i == 3 )
	 {
	    chisq2[j][0] = 0.;
	    ndf2[j][0] = 0.;
	    std::string rchi2name = model + dxssc[j] + "ITEP/proton_RecordChi2";
            RecordChi2* rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq2[j][0] = rchi2->GetSumChi2();
	       ndf2[j][0]  = rchi2->GetSumNDF();
	    }
	    leg_xssc_itep_proton->AddEntry( h2[j][0], Form( "%s, chi2/NDF=%.2f", xssclabel[j].c_str(), chisq2[j][0]/ndf2[j][0] ), "L" );
	    chisq2[j][1] = 0.;
	    ndf2[j][1] = 0.;
	    rchi2name = model + dxssc[j] + "ITEP/neutron_RecordChi2";
            rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq2[j][1] = rchi2->GetSumChi2();
	       ndf2[j][1]  = rchi2->GetSumNDF();
	    }
	    leg_xssc_itep_neutron->AddEntry( h2[j][1], Form( "%s, chi2/NDF=%.2f", xssclabel[j].c_str(), chisq2[j][1]/ndf2[j][1] ), "L" );
	 }
      }            
      //
      // FermiScale variants
      //
      TH1D* h3[NFsc][2];
      double chisq3[NFsc][2];
      double ndf3[NFsc][2];
      for ( int j=0; j<NFsc; ++j )
      {
         // secondary proton
	 h3[j][0] = 0;
	 hname = model + dfsc[j] + "ITEP/proton_at_" + angles[i] +"deg";
	 h3[j][0] = GetHisto( hname, max1 );
	 if ( !h3[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h3[j1][0]->GetYaxis()->SetRangeUser( 10., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 10., max1*1.1 );
	 h3[j][0]->SetLineColor(colfsc[j]);
         pad_fsc_itep_proton->cd(i+1);
         h3[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_fsc_itep_proton->Update();
	 cnv_fsc_itep_proton->Update();
         // secondary neutron
	 h3[j][1] = 0;
	 hname = model + dfsc[j] + "ITEP/neutron_at_" + angles[i] +"deg";
	 h3[j][1] = GetHisto( hname, max2 );
	 if ( !h3[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h3[j1][1]->GetYaxis()->SetRangeUser( 10., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 10., max2*1.1 );
	 h3[j][1]->SetLineColor(colfsc[j]);
         pad_fsc_itep_neutron->cd(i+1);
         h3[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_fsc_itep_neutron->Update();
	 cnv_fsc_itep_neutron->Update();
	 // fill up legend
	 if ( i == 3 )
	 {
	    chisq3[j][0] = 0.;
	    ndf3[j][0] = 0.;
	    std::string rchi2name = model + dfsc[j] + "ITEP/proton_RecordChi2";
            RecordChi2* rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq3[j][0] = rchi2->GetSumChi2();
	       ndf3[j][0]  = rchi2->GetSumNDF();
	    }
	    leg_fsc_itep_proton->AddEntry( h3[j][0], Form( "%s, chi2/NDF=%.2f", fsclabel[j].c_str(), chisq3[j][0]/ndf3[j][0] ), "L" );
	    chisq3[j][1] = 0.;
	    ndf3[j][1] = 0.;
	    rchi2name = model + dfsc[j] + "ITEP/neutron_RecordChi2";
            rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq3[j][1] = rchi2->GetSumChi2();
	       ndf3[j][1]  = rchi2->GetSumNDF();
	    }
	    leg_fsc_itep_neutron->AddEntry( h3[j][1], Form( "%s, chi2/NDF=%.2f", fsclabel[j].c_str(), chisq3[j][1]/ndf3[j][1] ), "L" );
	 }
      }
      //
      // GammaQD variants
      //
      TH1D* h4[NGQDsc][2];
      double chisq4[NGQDsc][2];
      double ndf4[NGQDsc][2];
      for ( int j=0; j<NGQDsc; ++j )
      {
         // secondary proton
	 h4[j][0] = 0;
	 hname = model + dgqdsc[j] + "ITEP/proton_at_" + angles[i] +"deg";
	 h4[j][0] = GetHisto( hname, max1 );
	 if ( !h4[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h4[j1][0]->GetYaxis()->SetRangeUser( 10., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 10., max1*1.1 );
	 h4[j][0]->SetLineColor(colgqdsc[j]);
         pad_gqdsc_itep_proton->cd(i+1);
         h4[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_gqdsc_itep_proton->Update();
	 cnv_gqdsc_itep_proton->Update();
         // secondary neutron
	 h4[j][1] = 0;
	 hname = model + dgqdsc[j] + "ITEP/neutron_at_" + angles[i] +"deg";
	 h4[j][1] = GetHisto( hname, max2 );
	 if ( !h4[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h4[j1][1]->GetYaxis()->SetRangeUser( 10., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 10., max2*1.1 );
	 h4[j][1]->SetLineColor(colgqdsc[j]);
         pad_gqdsc_itep_neutron->cd(i+1);
         h4[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_gqdsc_itep_neutron->Update();
	 cnv_gqdsc_itep_neutron->Update();
	 // fill up legend
	 if ( i == 3 )
	 {
	    chisq4[j][0] = 0.;
	    ndf4[j][0] = 0.;
	    std::string rchi2name = model + dgqdsc[j] + "ITEP/proton_RecordChi2";
            RecordChi2* rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq4[j][0] = rchi2->GetSumChi2();
	       ndf4[j][0]  = rchi2->GetSumNDF();
	    }
	    leg_gqdsc_itep_proton->AddEntry( h4[j][0], Form( "%s, chi2/NDF=%.2f", gqdsclabel[j].c_str(), chisq4[j][0]/ndf4[j][0] ), "L" );
	    chisq4[j][1] = 0.;
	    ndf4[j][1] = 0.;
	    rchi2name = model + dgqdsc[j] + "ITEP/neutron_RecordChi2";
            rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq4[j][1] = rchi2->GetSumChi2();
	       ndf4[j][1]  = rchi2->GetSumNDF();
	    }
	    leg_gqdsc_itep_neutron->AddEntry( h4[j][1], Form( "%s, chi2/NDF=%.2f", gqdsclabel[j].c_str(), chisq4[j][1]/ndf4[j][1] ), "L" );
	 }
      }
      //
      // TrailingRadius variants
      //
      TH1D* h5[NTrR][2];
      double chisq5[NTrR][2];
      double ndf5[NTrR][2];
      for ( int j=0; j<NTrR; ++j )
      {
         // secondary proton
	 h5[j][0] = 0;
	 hname = model + dtrr[j] + "ITEP/proton_at_" + angles[i] +"deg";
	 h5[j][0] = GetHisto( hname, max1 );
	 if ( !h5[j][0] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h5[j1][0]->GetYaxis()->SetRangeUser( 10., max1*1.1 );
	 }
	 h[0]->GetYaxis()->SetRangeUser( 10., max1*1.1 );
	 h5[j][0]->SetLineColor(coltrr[j]);
         pad_trr_itep_proton->cd(i+1);
         h5[j][0]->Draw("histE1same");
	 gPad->Update();
	 pad_trr_itep_proton->Update();
	 cnv_trr_itep_proton->Update();
         // secondary neutron
	 h5[j][1] = 0;
	 hname = model + dtrr[j] + "ITEP/neutron_at_" + angles[i] +"deg";
	 h5[j][1] = GetHisto( hname, max2 );
	 if ( !h5[j][1] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h5[j1][1]->GetYaxis()->SetRangeUser( 10., max2*1.1 );
	 }
	 h[1]->GetYaxis()->SetRangeUser( 10., max2*1.1 );
	 h5[j][1]->SetLineColor(coltrr[j]);
         pad_trr_itep_neutron->cd(i+1);
         h5[j][1]->Draw("histE1same");
	 gPad->Update();
	 pad_trr_itep_neutron->Update();
	 cnv_trr_itep_neutron->Update();
	 // fill up legend
	 if ( i == 3 )
	 {
	    chisq5[j][0] = 0.;
	    ndf5[j][0] = 0.;
	    std::string rchi2name = model + dtrr[j] + "ITEP/proton_RecordChi2";
            RecordChi2* rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq5[j][0] = rchi2->GetSumChi2();
	       ndf5[j][0]  = rchi2->GetSumNDF();
	    }
	    leg_trr_itep_proton->AddEntry( h5[j][0], Form( "%s, chi2/NDF=%.2f", trrlabel[j].c_str(), chisq5[j][0]/ndf5[j][0] ), "L" );
	    chisq5[j][1] = 0.;
	    ndf5[j][1] = 0.;
	    rchi2name = model + dtrr[j] + "ITEP/neutron_RecordChi2";
            rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq5[j][1] = rchi2->GetSumChi2();
	       ndf5[j][1]  = rchi2->GetSumNDF();
	    }
	    leg_trr_itep_neutron->AddEntry( h5[j][1], Form( "%s, chi2/NDF=%.2f", trrlabel[j].c_str(), chisq5[j][1]/ndf5[j][1] ), "L" );
	 }
      }
      // fill up legend with "Default"
      if ( i == 3 )
      {
         chisq[0] = 0.;
	 ndf[0] = 0.;
	 std::string rchi2name = model + def + "ITEP/proton_RecordChi2";
         RecordChi2* rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	 if ( rchi2 )
	 {
	    chisq[0] = rchi2->GetSumChi2();
	    ndf[0]  = rchi2->GetSumNDF();
	 }
	 chisq[1] = 0.;
	 ndf[1]  = 0.;
         rchi2name = model + def + "ITEP/neutron_RecordChi2";
         rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	 if ( rchi2 )
	 {
	    chisq[1] = rchi2->GetSumChi2();
	    ndf[1]  = rchi2->GetSumNDF();
	 }
	 leg_rsc_itep_proton->AddEntry( h[0], Form("Default (RadSc=2.8), chi2/NDF=%.2f",(chisq[0]/ndf[0])), "L" );
	 leg_rsc_itep_neutron->AddEntry( h[1], Form("Default (RadSc=2.8), chi2/NDF=%.2f",(chisq[1]/ndf[1])), "L" );
	 leg_xssc_itep_proton->AddEntry( h[0], Form("Default (XSecSc=1.), chi2/NDF=%.2f",(chisq[0]/ndf[0])), "L" );
	 leg_xssc_itep_neutron->AddEntry( h[1], Form("Default (XSecSc=1.), chi2/NDF=%.2f",(chisq[1]/ndf[1])), "L" );
	 leg_fsc_itep_proton->AddEntry( h[0], Form("Default (FermiSc=0.685), chi2/NDF=%.2f",(chisq[0]/ndf[0])), "L" );
	 leg_fsc_itep_neutron->AddEntry( h[1], Form("Default (FermiSc=0.685), chi2/NDF=%.2f",(chisq[1]/ndf[1])), "L" );
	 leg_gqdsc_itep_proton->AddEntry( h[0], Form("Default (GammaQDSc=1.), chi2/NDF=%.2f",(chisq[0]/ndf[0])), "L" );
	 leg_gqdsc_itep_neutron->AddEntry( h[1], Form("Default (GammaQDSc=1.), chi2/NDF=%.2f",(chisq[1]/ndf[1])), "L" );
	 leg_trr_itep_proton->AddEntry( h[0], Form("Default (TrailingR=0.), chi2/NDF=%.2f",(chisq[0]/ndf[0])), "L" );
	 leg_trr_itep_neutron->AddEntry( h[1], Form("Default (TrailingR=0.), chi2/NDF=%.2f",(chisq[1]/ndf[1])), "L" );
      }
      // repeat "Default" distro's to bring them up
      pad_rsc_itep_proton->cd(i+1);
      h[0]->Draw("histE1same");  
      pad_rsc_itep_neutron->cd(i+1);
      h[1]->Draw("histE1same"); 
      pad_xssc_itep_proton->cd(i+1);
      h[0]->Draw("histE1same");  
      pad_xssc_itep_neutron->cd(i+1);
      h[1]->Draw("histE1same"); 
      pad_fsc_itep_proton->cd(i+1);
      h[0]->Draw("histE1same");  
      pad_fsc_itep_neutron->cd(i+1);
      h[1]->Draw("histE1same"); 
      pad_gqdsc_itep_proton->cd(i+1);
      h[0]->Draw("histE1same");  
      pad_gqdsc_itep_neutron->cd(i+1);
      h[1]->Draw("histE1same"); 
      pad_trr_itep_proton->cd(i+1);
      h[0]->Draw("histE1same");  
      pad_trr_itep_neutron->cd(i+1);
      h[1]->Draw("histE1same"); 
      // overlay exp.data 
      if ( hex[0] )
      {
         pad_rsc_itep_proton->cd(i+1);
         hex[0]->Draw("psame");
         pad_xssc_itep_proton->cd(i+1);
         hex[0]->Draw("psame");
         pad_fsc_itep_proton->cd(i+1);
         hex[0]->Draw("psame");
         pad_gqdsc_itep_proton->cd(i+1);
         hex[0]->Draw("psame");
         pad_trr_itep_proton->cd(i+1);
         hex[0]->Draw("psame");
      }
      if ( hex[1] )
      {
         pad_rsc_itep_neutron->cd(i+1);
         hex[1]->Draw("psame");
         pad_xssc_itep_neutron->cd(i+1);
         hex[1]->Draw("psame");
         pad_fsc_itep_neutron->cd(i+1);
         hex[1]->Draw("psame");
         pad_gqdsc_itep_neutron->cd(i+1);
         hex[1]->Draw("psame");
         pad_trr_itep_neutron->cd(i+1);
         hex[1]->Draw("psame");
      } 
      if ( i == 3 )
      {
	 // ceating - always add symbol for the proton exp.data graph 
	 // since some of the neutron's data are missing
	 leg_rsc_itep_proton->AddEntry( hex[0], "ITEP771 data", "p" );
	 leg_rsc_itep_neutron->AddEntry( hex[0], "ITEP771 data", "p" );
	 leg_xssc_itep_proton->AddEntry( hex[0], "ITEP771 data", "p" );
	 leg_xssc_itep_neutron->AddEntry( hex[0], "ITEP771 data", "p" );
	 leg_fsc_itep_proton->AddEntry( hex[0], "ITEP771 data", "p" );
	 leg_fsc_itep_neutron->AddEntry( hex[0], "ITEP771 data", "p" );
	 leg_gqdsc_itep_proton->AddEntry( hex[0], "ITEP771 data", "p" );
	 leg_gqdsc_itep_neutron->AddEntry( hex[0], "ITEP771 data", "p" );
	 leg_trr_itep_proton->AddEntry( hex[0], "ITEP771 data", "p" );
	 leg_trr_itep_neutron->AddEntry( hex[0], "ITEP771 data", "p" );
      }
   }   

   cnv_rsc_itep_proton->cd();
   leg_rsc_itep_proton->Draw();
   cnv_rsc_itep_neutron->cd();
   leg_rsc_itep_neutron->Draw();   

   cnv_xssc_itep_proton->cd();
   leg_xssc_itep_proton->Draw();
   cnv_xssc_itep_neutron->cd();
   leg_xssc_itep_neutron->Draw();   

   cnv_fsc_itep_proton->cd();
   leg_fsc_itep_proton->Draw();
   cnv_fsc_itep_neutron->cd();
   leg_fsc_itep_neutron->Draw();   

   cnv_gqdsc_itep_proton->cd();
   leg_gqdsc_itep_proton->Draw();
   cnv_gqdsc_itep_neutron->cd();
   leg_gqdsc_itep_neutron->Draw();   

   cnv_trr_itep_proton->cd();
   leg_trr_itep_proton->Draw();
   cnv_trr_itep_neutron->cd();
   leg_trr_itep_neutron->Draw();   

   // output(s)
   //
   
   size_t pos1 = fname.find_last_of("/");
   if ( pos1 == std::string::npos ) pos1 = -1;
   size_t pos2 = fname.find("root");
   std::string output = fname.substr( pos1+1, (pos2-(pos1+1)) );
   output += "ps";
   
   cnv_rsc_harp_piplus->Print( Form("%s%s",output.c_str(),"["), "psLandscape" );
   // harp
   cnv_rsc_harp_piplus->Print( output.c_str(), "psLandscape" );
   cnv_rsc_harp_piminus->Print( output.c_str(), "psLandscape" );
   cnv_xssc_harp_piplus->Print( output.c_str(), "psLandscape" );
   cnv_xssc_harp_piminus->Print( output.c_str(), "psLandscape" );
   cnv_fsc_harp_piplus->Print( output.c_str(), "psLandscape" );
   cnv_fsc_harp_piminus->Print( output.c_str(), "psLandscape" );
   cnv_gqdsc_harp_piplus->Print( output.c_str(), "psLandscape" );
   cnv_gqdsc_harp_piminus->Print( output.c_str(), "psLandscape" );
   cnv_trr_harp_piplus->Print( output.c_str(), "psLandscape" );
   cnv_trr_harp_piminus->Print( output.c_str(), "psLandscape" );
   //itep
   cnv_rsc_itep_proton->Print( output.c_str(), "psLandscape" );
   cnv_rsc_itep_neutron->Print( output.c_str(), "psLandscape" );
   cnv_xssc_itep_proton->Print( output.c_str(), "psLandscape" );
   cnv_xssc_itep_neutron->Print( output.c_str(), "psLandscape" );
   cnv_fsc_itep_proton->Print( output.c_str(), "psLandscape" );
   cnv_fsc_itep_neutron->Print( output.c_str(), "psLandscape" );
   cnv_gqdsc_itep_proton->Print( output.c_str(), "psLandscape" );
   cnv_gqdsc_itep_neutron->Print( output.c_str(), "psLandscape" );
   cnv_trr_itep_proton->Print( output.c_str(), "psLandscape" );
   cnv_trr_itep_neutron->Print( output.c_str(), "psLandscape" );
   //
   cnv_trr_harp_piplus->Print( Form("%s%s",output.c_str(),"]"), "psLandscape" );
     
   return 0;

}

TH1D* GetHisto( std::string hname, double& max )
{

   TH1D* h = (TH1D*)hfile->Get( hname.c_str() );
   if ( !h )
   {
         std::cout << " NULL histo " << hname << std::endl;
	 return NULL;
   }
   int imb = h->GetMaximumBin();
   if ( h->GetBinContent(imb)+2.*h->GetBinError(imb) > max ) 
      max = h->GetBinContent(imb)+2.*h->GetBinError(imb);
   h->SetLineWidth(2);
   h->SetStats(0);
   h->GetXaxis()->SetLabelFont(62);
   h->GetYaxis()->SetLabelFont(62);

   return h;
   
}

TH1D* GetExpHARP( TH1D* hmc, const std::vector<TH1D*>& recs, double& max )
{

   TH1D* hex = 0;
   std::string htitle = hmc->GetTitle();
   
   size_t pos = htitle.find_first_of("[");
   std::string htt = "";
   
   if ( pos == std::string::npos )
   {
      htt = htitle;
   }
   else
   {
      htt = htitle.substr(0,pos-1);
   }
   
   for ( size_t ie=0; ie<recs.size(); ++ie )
   {
         std::string exptitle = recs[ie]->GetTitle();
	 if ( exptitle.find( htt ) != std::string::npos )
	 {
	    hex = recs[ie];
	    break;
	 }
   }
   
   if ( !hex ) return NULL; 

   hex->SetMarkerStyle(22);
   hex->SetMarkerColor(kBlack);
   hex->SetMarkerSize(1.25);
   int imb = hex->GetMaximumBin();
   if ( hex->GetBinContent(imb)+2.*hex->GetBinError(imb) > max ) 
      max = hex->GetBinContent(imb)+2.*hex->GetBinError(imb);
   
   return hex;

}

void DrawVariantsHARP( const int nv, std::string* vdirs, 
                       std::string model,
		       std::string hsubname, 
		       int* colors,
		       double& max )  
{
   
   TH1D* h[nv];
   
   for ( int j=0; j<nv; ++j )
   {
         // secondary pi+
	 h[j] = 0;
	 std::string hname = model + vdirs[j] + hsubname;
	 h[j] = GetHisto( hname, max );
	 if ( !h[j] ) continue;
         for ( int j1=0; j1<=j; ++j1 )
	 {
	    h[j1]->GetYaxis()->SetRangeUser( 0., max*1.1 );
	 }
	 // h[0]->GetYaxis()->SetRangeUser( 0., max*1.1 );
	 h[j]->SetLineColor(colors[j]);
         // pad_fsc_harp_piplus_la->cd(i+1);
         h[j]->Draw("histE1same");
	 gPad->Update();
	 // pad_fsc_harp_piplus_la->Update();
	 // cnv_fsc_harp_piplus->Update();
	 //
	 // fill up legend if necessary
	 //
   }

   return;

}

void VariantsHARP2Legend( const int nv, std::string* vdirs, std::string model, std::string rname, 
                          std::string* vlabels, int* colors, TLegend* leg ) 
{

   double chisq[nv];
   double ndf[nv];
   // TLegendEntry* lent = 0;
   for ( int j=0; j<nv; ++j )
   {
//      if ( leg->GetNRows() < 2 )
//      {
	    chisq[j] = 0.;
	    ndf[j] = 0.;
	    std::string rchi2name = model + vdirs[j] + rname;
            RecordChi2* rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str());
	    if ( rchi2 )
	    {
	       chisq[j] = rchi2->GetSumChi2();
	       ndf[j]  = rchi2->GetSumNDF();
	    }
	    TMarker marker;
	    marker.SetMarkerStyle(kFullCross);
	    marker.SetMarkerColor(colors[j]);
	    /* lent = */ leg->AddEntry( &marker, Form( "%s, chi2/NDF=%.2f", vlabels[j].c_str(), chisq[j]/ndf[j] ), "p" );
//      }
   }
   
   return;

}
