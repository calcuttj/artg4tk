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
#include "TH1F.h"
#include "TCanvas.h"
// --> #include "TPad.h"
#include "TStyle.h"
#include "TGraph.h"

void PlotLArIAT( std::string fname )
{

   std::string hnameLAr[6]   = { "Edep", "EdepZ", "EdepR", "HShowerEdep", "HShowerEdepZ", "HShowerEdepR" }; 
   //
   std::string thetaHARP_FW[5] = { "0.05", "0.1", "0.15", "0.2", "0.25" };
   std::string thetaHARP_LA[10] = { "0.35", "0.55", "0.75", "0.95", "1.15", 
                                    "1.35", "1.55", "1.75", "1.95", "2.15" }; 
   //
   std::string thetaITEP[4] = { "59.1", "89", "119", "159.6" };
   // 
   std::string dirnameLAr[3] = { "EdepTrkG4Default", "EdepTrkBertRsc1", "EdepTrkBertXsc01" };
   std::string dirnameHARP[3] = { "HARPG4Default", "HARPBertRsc1", "HARPBertXsc01" };
   std::string dirnameITEP[3] = { "ITEPG4Default", "ITEPBertRsc1", "ITEPBertXsc01" };
   //
   std::string variants[3] = { "G4-default", "Bertini-RScale=1. (D=2.82)", "Bertini-XSecScale=0.1 (D=1.)" };
   int colors[3] = { kRed, kBlue, kGreen };
   //
   double ylimitsLAr[6] = { 12., 0.01, 0.16, 3., 0.01, 0.16 };
//   double ylimitsHARP_FW[4] = { 500., 1000., 1000., 1000. }; 
   
   TCanvas* mycLAr = new TCanvas( "mycLAr", "", 1050., 650. );
   // mycLAr->Divide(3,2);
   TPad* padLAr = new TPad( "padLAr", "", 0.01, 0.01, 0.99, 0.93 );
   mycLAr->cd();
   padLAr->Draw();
   padLAr->Divide(3,2); 
   
   TFile* f = new TFile( fname.c_str() );
   
   TH1D* h[6];
   
   TLegend* leg = new TLegend( 0.35, 0.65, 0.99, 0.90 );
   
   for ( int i=0; i<3; ++i )
   {
      for ( int j=0; j<6; ++j )
      {
         std::string tmp = dirnameLAr[i] + "/" + hnameLAr[j];
	 h[j] = (TH1D*)f->Get( tmp.c_str() );
	 if ( !h[j] )
	 {
	    std::cout << " NULL histo " << tmp << std::endl;
	    continue;
	 }
	 h[j]->SetStats(0);
	 h[j]->GetYaxis()->SetRangeUser( 0., ylimitsLAr[j] );
	 h[j]->SetLineColor(colors[i]);
	 h[j]->SetLineWidth(5-i);
	 h[j]->GetYaxis()->SetTitleOffset(2.5);
	 h[j]->GetXaxis()->SetTitleFont(62);
	 h[j]->GetYaxis()->SetTitleFont(62);
	 // mycLAr->cd(j+1);
	 padLAr->cd(j+1);
	 gPad->SetLeftMargin(0.2);
	 gPad->SetRightMargin(0.1);
	 if ( i == 0 )
	 {
	    h[j]->Draw( "histE1" );
	 }
	 else h[j]->Draw( "histE1same" );
      }
      leg->AddEntry( h[0], variants[i].c_str(), "L" );      
   }
   
   // mycLAr->cd(6);
   padLAr->cd(6);
   leg->SetTextFont(62);
   leg->Draw();
   leg->SetFillColor(kWhite);
   
   TLatex* ltxt = new TLatex( 0.3, 0.96, "2GeV/c #pi^{+} on LAr (LArIAT sensitive volume)" );
   ltxt->SetTextSize(0.035);
   mycLAr->cd();
   ltxt->Draw();  
   
   mycLAr->Print( "piplus-LAr-2GeV-Edep.gif" );

   TCanvas* mycHARP_piplus_FW = new TCanvas( "mycHARP_piplus_FW", "", 600., 600. );
   // mycHARP_piplus_FW->Divide(2,2);
   TPad* padHARP_piplus_FW = new TPad( "padHARP_piplus_FW", "", 0.01, 0.01, 0.99, 0.93 );
   mycHARP_piplus_FW->cd();
   padHARP_piplus_FW->Draw();
   padHARP_piplus_FW->Divide(2,2);
   
   TH1D* h1[4];
   
   for ( int i=0; i<3; ++i )
   {
      for ( int j=0; j<4; ++j )
      {
         std::ostringstream os;
	 os << j;
	 std::string tmp = dirnameHARP[i] + "/piplus_FW_" + os.str();
	 h1[j] = (TH1D*)f->Get( tmp.c_str() ); 
	 if ( !h1[j] )
	 {
	    std::cout << " NULL histo " << tmp << std::endl;
	    continue;
	 }
	 h1[j]->SetStats(0);
//	 h1[j]->GetYaxis()->SetRangeUser( 0., ylimitsHARP_FW[j] );
	 h1[j]->GetYaxis()->SetRangeUser( 0., 900. );
	 h1[j]->SetLineColor(colors[i]);
	 h1[j]->SetLineWidth(5-i);
	 h1[j]->GetYaxis()->SetTitleOffset(2.5);
	 h1[j]->GetXaxis()->SetTitleFont(62);
	 h1[j]->GetYaxis()->SetTitleFont(62);
	 // mycHARP_piplus_FW->cd(j+1);
	 padHARP_piplus_FW->cd(j+1);
	 gPad->SetLeftMargin(0.2);
	 gPad->SetRightMargin(0.1);
	 if ( i == 0 )
	 {
	    h1[j]->Draw( "histE1" );
	    std::string txt = thetaHARP_FW[j] + " < #theta < " + thetaHARP_FW[j+1] + " [rad]";	    
	    h1[j]->SetTitle( txt.c_str() ); 
	    h1[j]->GetXaxis()->SetTitle( "momentum of secondary #pi^{+} [GeV]" );
	    h1[j]->GetXaxis()->SetTitleFont(62);
	    h1[j]->GetYaxis()->SetTitle( "d^{2}#sigma / dpd#Theta [mb/(GeV/c/rad)]");
	    h1[j]->GetYaxis()->SetTitleFont(62);
	 }
	 else h1[j]->Draw( "histE1same" );
      }
   }
   
   mycHARP_piplus_FW->cd(1);
   leg->Draw();
   
   TLatex* ltxt1 = new TLatex( 0.3, 0.95, "2GeV/c #pi^{+} + LAr #rightarrow #pi^{+} + X" );
   ltxt1->SetTextSize(0.03);
   mycHARP_piplus_FW->cd();
   ltxt1->Draw();
   
   mycHARP_piplus_FW->Print( "piplus-LAr-2GeV-piplus-FW.gif" );
   
   TCanvas* mycHARP_piminus_FW = new TCanvas( "mycHARP_piminus_FW", "", 600., 600. );
   // mycHARP_piminus_FW->Divide(2,2);
   TPad* padHARP_piminus_FW = new TPad( "padHARP_piminus_FW", "", 0.01, 0.01, 0.99, 0.93 );
   mycHARP_piminus_FW->cd();
   padHARP_piminus_FW->Draw();
   padHARP_piminus_FW->Divide(2,2);
   
   TH1D* h2[4];
   
   for ( int i=0; i<3; ++i )
   {
      for ( int j=0; j<4; ++j )
      {
         std::ostringstream os;
	 os << j;
	 std::string tmp = dirnameHARP[i] + "/piminus_FW_" + os.str();
	 h2[j] = (TH1D*)f->Get( tmp.c_str() ); 
	 if ( !h2[j] )
	 {
	    std::cout << " NULL histo " << tmp << std::endl;
	    continue;
	 }
	 h2[j]->SetStats(0);
	 // h2[j]->GetYaxis()->SetRangeUser( 0., ylimitsHARP_FW[j] );
	 h2[j]->GetYaxis()->SetRangeUser( 0., 100. );
	 h2[j]->SetLineColor(colors[i]);
	 h2[j]->SetLineWidth(5-i);
	 h2[j]->GetYaxis()->SetTitleOffset(2.5);
	 h2[j]->GetXaxis()->SetTitleFont(62);
	 h2[j]->GetYaxis()->SetTitleFont(62);
	 // mycHARP_piminus_FW->cd(j+1);
	 padHARP_piminus_FW->cd(j+1);
	 gPad->SetLeftMargin(0.2);
	 gPad->SetRightMargin(0.1);
	 if ( i == 0 )
	 {
	    h2[j]->Draw( "histE1" );
	    std::string txt = thetaHARP_FW[j] + " < #theta < " + thetaHARP_FW[j+1] + " [rad]";	    
	    h2[j]->SetTitle( txt.c_str() ); 
	    h2[j]->GetXaxis()->SetTitle( "momentum of secondary #pi^{-} [GeV]" );
	    h2[j]->GetXaxis()->SetTitleFont(62);
	    h2[j]->GetYaxis()->SetTitle( "d^{2}#sigma / dpd#Theta [mb/(GeV/c/rad)]");
	    h2[j]->GetYaxis()->SetTitleFont(62);
	 }
	 else h2[j]->Draw( "histE1same" );
      }
   }

   padHARP_piminus_FW->cd(1);
   leg->Draw();
   
   TLatex* ltxt2 = new TLatex( 0.3, 0.95, "2GeV/c #pi^{+} + LAr #rightarrow #pi^{-} + X" );
   ltxt2->SetTextSize(0.03);
   mycHARP_piminus_FW->cd();
   ltxt2->Draw();
   
   mycHARP_piminus_FW->Print( "piplus-LAr-2GeV-piminus-FW.gif" );
 
   TCanvas* mycHARP_piplus_LA = new TCanvas( "mycHARP_piplus_LA", "", 750., 750. );
   // mycHARP_piplus_LA->Divide(3,3);
   TPad* padHARP_piplus_LA = new TPad( "padHARP_piplus_LA", "", 0.01, 0.01, 0.99, 0.93 );
   mycHARP_piplus_LA->cd();
   padHARP_piplus_LA->Draw();
   padHARP_piplus_LA->Divide(3,3);

   TH1D* h3[9]; 
   
   for ( int i=0; i<3; ++i )
   {
      for ( int j=0; j<9; ++j )
      {
         std::ostringstream os;
	 os << j;
	 std::string tmp = dirnameHARP[i] + "/piplus_LA_" + os.str();
	 h3[j] = (TH1D*)f->Get( tmp.c_str() ); 
	 if ( !h3[j] )
	 {
	    std::cout << " NULL histo " << tmp << std::endl;
	    continue;
	 }
	 h3[j]->SetStats(0);
	 h3[j]->GetYaxis()->SetRangeUser( 0., 500. );
	 h3[j]->SetLineColor(colors[i]);
	 h3[j]->SetLineWidth(5-i);
	 h3[j]->GetYaxis()->SetTitleOffset(2.5);
	 h3[j]->GetXaxis()->SetTitleFont(62);
	 h3[j]->GetYaxis()->SetTitleFont(62);
	 // mycHARP_piplus_LA->cd(j+1);
	 padHARP_piplus_LA->cd(j+1);
	 gPad->SetLeftMargin(0.2);
	 gPad->SetRightMargin(0.1);
	 if ( i == 0 )
	 {
	    h3[j]->Draw( "histE1" );
	    std::string txt = thetaHARP_LA[j] + " < #theta < " + thetaHARP_LA[j+1] + " [rad]";	    
	    h3[j]->SetTitle( txt.c_str() ); 
	    h3[j]->GetXaxis()->SetTitle( "momentum of secondary #pi^{+} [GeV]" );
	    h3[j]->GetXaxis()->SetTitleFont(62);
	    h3[j]->GetYaxis()->SetTitle( "d^{2}#sigma / dpd#Theta [mb/(GeV/c/rad)]");
	    h3[j]->GetYaxis()->SetTitleFont(62);
	 }
	 else h3[j]->Draw( "histE1same" );
      }
   }

   padHARP_piplus_LA->cd(9);
   leg->Draw();
   
   mycHARP_piplus_LA->cd();
   ltxt1->Draw();
   
   mycHARP_piplus_LA->Print( "piplus-LAr-2GeV-piplus-LA.gif" );

   TCanvas* mycHARP_piminus_LA = new TCanvas( "mycHARP_piminus_LA", "", 750., 750. );
   // mycHARP_piminus_LA->Divide(3,3);
   TPad* padHARP_piminus_LA = new TPad( "padHARP_piminus_LA", "", 0.01, 0.01, 0.99, 0.93 );
   mycHARP_piminus_LA->cd();
   padHARP_piminus_LA->Draw();
   padHARP_piminus_LA->Divide(3,3);

   TH1D* h4[9]; 
   
   for ( int i=0; i<3; ++i )
   {
      for ( int j=0; j<9; ++j )
      {
         std::ostringstream os;
	 os << j;
	 std::string tmp = dirnameHARP[i] + "/piminus_LA_" + os.str();
	 h4[j] = (TH1D*)f->Get( tmp.c_str() ); 
	 if ( !h4[j] )
	 {
	    std::cout << " NULL histo " << tmp << std::endl;
	    continue;
	 }
	 h4[j]->SetStats(0);
	 h4[j]->GetYaxis()->SetRangeUser( 0., 500. );
	 h4[j]->SetLineColor(colors[i]);
	 h4[j]->SetLineWidth(5-i);
	 h4[j]->GetYaxis()->SetTitleOffset(2.5);
	 h4[j]->GetXaxis()->SetTitleFont(62);
	 h4[j]->GetYaxis()->SetTitleFont(62);
	 // mycHARP_piminus_LA->cd(j+1);
	 padHARP_piminus_LA->cd(j+1);
	 gPad->SetLeftMargin(0.2);
	 gPad->SetRightMargin(0.1);
	 if ( i == 0 )
	 {
	    h4[j]->Draw( "histE1" );
	    std::string txt = thetaHARP_LA[j] + " < #theta < " + thetaHARP_LA[j+1] + " [rad]";	    
	    h4[j]->SetTitle( txt.c_str() ); 
	    h4[j]->GetXaxis()->SetTitle( "momentum of secondary #pi^{-} [GeV]" );
	    h4[j]->GetXaxis()->SetTitleFont(62);
	    h4[j]->GetYaxis()->SetTitle( "d^{2}#sigma / dpd#Theta [mb/(GeV/c/rad)]");
	    h4[j]->GetYaxis()->SetTitleFont(62);
	 }
	 else h4[j]->Draw( "histE1same" );
      }
   }

   padHARP_piminus_LA->cd(9);
   leg->Draw();
   
   mycHARP_piminus_LA->cd();
   ltxt2->Draw();
   
   mycHARP_piminus_LA->Print( "piplus-LAr-2GeV-piminus-LA.gif" );
   
   TCanvas* mycITEP_proton = new TCanvas( "mycITEP_proton", "", 600., 600. );
   // mycITEP_proton->Divide(2,2);
   TPad* padITEP_proton = new TPad( "padITEP_proton", "", 0.01, 0.01, 0.99, 0.93 );
   mycITEP_proton->cd();
   padITEP_proton->Draw();
   padITEP_proton->Divide(2,2);
   
   TH1D* h5[4];
   
   for ( int i=0; i<3; ++i )
   {
      for ( int j=0; j<4; ++j )
      {
         std::string tmp = dirnameITEP[i] + "/proton_at_" + thetaITEP[j] + "deg";
	 h5[j] = (TH1D*)f->Get( tmp.c_str() );
	 if ( !h5[j] )
	 {
	    std::cout << " NULL histo " << tmp << std::endl;
	    continue;
	 }
	 h5[j]->SetStats(0);
	 // h5[j]->GetYaxis()->SetRangeUser( 0., 500. );
	 h5[j]->SetLineColor(colors[i]);
	 h5[j]->SetLineWidth(5-i);
	 h5[j]->GetYaxis()->SetTitleOffset(2.5);
	 h5[j]->GetXaxis()->SetTitleFont(62);
	 h5[j]->GetYaxis()->SetTitleFont(62);
	 // mycITEP_proton->cd(j+1);
	 padITEP_proton->cd(j+1);
	 gPad->SetLogy();
	 gPad->SetLeftMargin(0.2);
	 gPad->SetRightMargin(0.1);
	 if ( i == 0 )
	 {
	    h5[j]->Draw( "histE1" );
	    std::string txt = "#theta = " + thetaITEP[j] + " [deg]";
	    h5[j]->SetTitle( txt.c_str() );
	    h5[j]->GetXaxis()->SetTitle( "kinetic energy of secondary proton [GeV]" );
	    h5[j]->GetXaxis()->SetTitleFont(62);
	    h5[j]->GetYaxis()->SetTitle( "d^{3}#sigma / dp^{3} [mb/(GeV/c)^{3}]" );
	    h5[j]->GetYaxis()->SetTitleFont(62);
	 }
	 else h5[j]->Draw( "histE1same" );	 
      }
   }
   
   padITEP_proton->cd(4);
   leg->Draw();
   
   TLatex* ltxt3 = new TLatex( 0.3, 0.95, "2GeV/c #pi^{+} + LAr #rightarrow proton + X" );
   ltxt3->SetTextSize(0.03);
   mycITEP_proton->cd();
   ltxt3->Draw();
   
   mycITEP_proton->Print( "piplus-LAr-2GeV-proton.gif" );

   TCanvas* mycITEP_neutron = new TCanvas( "mycITEP_neutron", "", 600., 600. );
   // mycITEP_neutron->Divide(2,2);
   TPad* padITEP_neutron = new TPad( "padITEP_neutron", "", 0.01, 0.01, 0.99, 0.93 );
   mycITEP_neutron->cd();
   padITEP_neutron->Draw();
   padITEP_neutron->Divide(2,2);

   TH1D* h6[4];
   
   for ( int i=0; i<3; ++i )
   {
      for ( int j=0; j<4; ++j )
      {
         std::string tmp = dirnameITEP[i] + "/neutron_at_" + thetaITEP[j] + "deg";
	 h6[j] = (TH1D*)f->Get( tmp.c_str() );
	 if ( !h6[j] )
	 {
	    std::cout << " NULL histo " << tmp << std::endl;
	    continue;
	 }
	 h6[j]->SetStats(0);
	 // h5[j]->GetYaxis()->SetRangeUser( 0., 500. );
	 h6[j]->SetLineColor(colors[i]);
	 h6[j]->SetLineWidth(5-i);
	 h6[j]->GetYaxis()->SetTitleOffset(2.5);
	 h6[j]->GetXaxis()->SetTitleFont(62);
	 h6[j]->GetYaxis()->SetTitleFont(62);
	 // mycITEP_neutron->cd(j+1);
	 padITEP_neutron->cd(j+1);
	 gPad->SetLogy();
	 gPad->SetLeftMargin(0.2);
	 gPad->SetRightMargin(0.1);
	 if ( i == 0 )
	 {
	    h6[j]->Draw( "histE1" );
	    std::string txt = "#theta = " + thetaITEP[j] + " [deg]";
	    h6[j]->SetTitle( txt.c_str() );
	    h6[j]->GetXaxis()->SetTitle( "kinetic energy of secondary neutron [GeV]" );
	    h6[j]->GetXaxis()->SetTitleFont(62);
	    h6[j]->GetYaxis()->SetTitle( "d^{3}#sigma / dp^{3} [mb/(GeV/c)^{3}]" );
	    h6[j]->GetYaxis()->SetTitleFont(62);
	 }
	 else h6[j]->Draw( "histE1same" );	 
      }
   }

   padITEP_neutron->cd(4);
   leg->Draw();
   
   TLatex* ltxt4 = new TLatex( 0.3, 0.95, "2GeV/c #pi^{+} + LAr #rightarrow neutron + X" );
   ltxt4->SetTextSize(0.03);
   mycITEP_neutron->cd();
   ltxt4->Draw();
   
   mycITEP_neutron->Print( "piplus-LAr-2GeV-neutron.gif" );

   return;

}
