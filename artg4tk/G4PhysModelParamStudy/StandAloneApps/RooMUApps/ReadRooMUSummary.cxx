
// system includes
//
#include <iostream>
// --> #include <sstream> // if one wants to use std::ostringstream 
#include <string>
#include <vector>

// native Root includes
//
#include "TCanvas.h"
#include "TFile.h"

// RooMUHistos includes
//
#include "PlotUtils/MUH1D.h"
#include "PlotUtils/MUApplication.h"
#include "PlotUtils/MUPlotter.h"

using namespace PlotUtils;

int main( int argc, char** argv )
{

   if ( argc != 5 )
   {
      std::cout << " Missing argument(s): input file and/or RooMUHistos directory" << std::endl;
      std::cout << " Usage format: ReadRooMUSummary -i <inputfilename>.root -d <roomuhistosdir>" << std::endl;
      return 1;
   }
   
   TH1::SetDefaultSumw2();

   // FIXME !!!
   // TMP impl. - need better handling of input arguments !!!
   //
   std::string hfilename = argv[2];   
   std::string summarydir = argv[4];
         
   TFile* tstfile = TFile::Open( hfilename.c_str(), "READ" );
   
   if ( !tstfile->GetDirectory(summarydir.c_str()) )
   {
      std::cout << " Directory " << summarydir 
                << " does NOT exist in the Root file " << tstfile 
		<< "; bail out " << std::endl;
      return 0;
   }
   
   TIter  next( tstfile->GetDirectory( summarydir.c_str() )->GetListOfKeys() );
   TKey*     key = (TKey*)next();
   MUH1D*    muh1tst = 0;    
   
   MUPlotter* muplt = new MUPlotter();
   while ( key )
   {
      if ( !(TClass::GetClass((key->ReadObj())->ClassName())->InheritsFrom(TH1::Class())) ) 
      {
	 key = (TKey*)next();
	 continue;
      }
      muh1tst = (MUH1D*)key->ReadObj();
      std::string hname( muh1tst->GetName() );
/*
      if ( hname.find("piplus") == std::string::npos && 
           hname.find("NSec") == std::string::npos ) 
      {
         key = (TKey*)next();
	 continue;
      }
*/
      //
      // NOTE-1 (JVY): This will explicitly retrieve the original CV histo.
      //               There's another similar method, GetCVHistoWithError() 
      //               - see comment later in the code.
      //
      TH1D hcv = muh1tst->GetCVHistoWithStatError();
      hcv.SetLineColor(kRed);
      hcv.SetLineWidth(3);
      int maxbin = hcv.GetMaximumBin();
      double max = hcv.GetBinContent(maxbin) + hcv.GetBinError(maxbin);

      std::vector<std::string> vbandnames = muh1tst->GetVertErrorBandNames();
      int nbands = vbandnames.size();

      TCanvas* cnv = new TCanvas( hname.c_str(), "", (600.*(float)nbands), 600. );
      cnv->Divide( nbands+1, 1 );
      
      for ( int i=0; i<nbands; ++i )
      {
	 const MUVertErrorBand* vband = muh1tst->GetVertErrorBand( vbandnames[i] );
         int nvbhi = vband->GetNHists();
         for ( int ih=0; ih<nvbhi; ++ih )
         {
	    TH1D* vbhi = (TH1D*)vband->GetHist(ih); // bad trick - casting awat const !!!
	    maxbin = vbhi->GetMaximumBin();
	    if ( max < (vbhi->GetBinContent(maxbin)+vbhi->GetBinError(maxbin)) ) 
	          max = vbhi->GetBinContent(maxbin)+vbhi->GetBinError(maxbin);
	 }
         hcv.GetYaxis()->SetRangeUser(0.,max*1.1);
	 cnv->cd(i+1);
         hcv.DrawCopy("E1hist");
         for ( int ih=0; ih<nvbhi; ++ih )
         {
	    TH1D* vbhi = (TH1D*)vband->GetHist(ih); // bad trick - casting awat const !!!
	    vbhi->SetLineColor(kBlue+ih*3);
	    vbhi->SetLineStyle(2);
	    vbhi->SetLineWidth(3);
	    vbhi->DrawCopy("histsame");
         }
      } 
      //
      // NOTE-2(JVY): Now, get the CV outfitted with TOTAL errors 
      //              (not only Stat ones as we did earlier).
      //              The machinery will do the following:  
      //              a) check if there're error bands there, and how many
      //              b) if yes, a sys error matrix will be calculated 
      //                 automatically over ALL bands 
      //              c) it'll be combined with the stat one (default behaviour), 
      //                 and the total error will be calculated for each bin. 
      //  
      // NOTE-3 (JVY): Here it's also useful to keep in mind that sys errors
      //               calculation may go one of the following 3 ways:
      //               1) for 1 variant ("universe") ** in a band **  
      //                  they use the full max spread in the errors calculation
      //               2) for 2-9, it takes 1/2 of the max spread
      //               3) for 10 or more, it uses the interquartile spread
      //
      // NOTE-4 (JVY): The CV with TOTAL errors histo is what one should pass 
      //               to MUPlotter for a nice display (not the MUH1D kind of 
      //               thing because the plotter will cast it to TH1D*, thus 
      //               it'll chop off th eband).
      //
      TH1D hcverr = muh1tst->GetCVHistoWithError();
      cnv->cd(nbands+1); // pad id starts at 1, not 0; 
                         // the number of pads is nbands+1, where the last one is for the "grand summary band" 
      muplt->DrawMCWithErrorBand( &hcverr );
      std::string pltname = hname + ".gif";
      cnv->Print( pltname.c_str() );
      delete cnv;

      key = (TKey*)next();
   }
      
   tstfile->Close();
    
   delete muplt;
   return 0;

}
