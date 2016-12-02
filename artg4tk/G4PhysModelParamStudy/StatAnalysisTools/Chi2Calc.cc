
#include "artg4tk/G4PhysModelParamStudy/StatAnalysisTools/Chi2Calc.hh"
// #include <sstream>

#include "TH1D.h"
#include "TGraphErrors.h"

double Chi2Calc::Chi2DataMC( const TH1* data, 
                             const TH1* mc, 
			     int& ndf, 
			     const double mcscale /* = 1. */ )
{

   fChi2 = 0.;
   ndf   = 0;

// NOTE: Clone() creates a **new** copy, 
//       so it'll need to be deleted afterwards
//       As an alternative, one can use boots::shared_ptr
//       but since we're considering to backport it to RooMUHisto... 
//       which we want to incorporate into GENIE/Val... given that
//       some people on GENIE don't like boost... let's make this
//       implementation as "plain" as possible
//
   TH1D* tmpData = (TH1D*)data->Clone("tmp_data_chi2");
   TH1D* tmpMC   = (TH1D*)mc->Clone("tmp_mc_chi2");

   if( tmpData->GetSumw2N() == 0 ) tmpData->Sumw2();
   if( tmpMC->GetSumw2N() == 0 )   tmpMC->Sumw2();

   tmpMC->Scale(mcscale);
   
   const int x1Data = tmpData->GetXaxis()->GetFirst();
   const int x2Data = tmpData->GetXaxis()->GetLast();
   const int x1MC   = tmpMC->GetXaxis()->GetFirst();
   const int x2MC   = tmpMC->GetXaxis()->GetLast();

// NOTE: Data and MC are NOT required to have the same number of bins/points
//       (this is the difference from Root's standard Chi2 utilities).
//       Technically speaking, this is not entirely safe because bin width 
//       can differ... but we'll face similar issue with the data graph !...
//       So let's assume that the data are properly normalized, and so is the MC 

   for( int imc=x1MC; imc<=x2MC; ++imc )
   {
      double xmc  = tmpMC->GetBinLowEdge(imc);
      double dxmc = tmpMC->GetBinWidth(imc);      
      for ( int id=x1Data; id<=x2Data; ++id )
      {
         double xdc  = tmpData->GetBinCenter(id);	 
	 if ( xdc > xmc && xdc < (xmc+dxmc) )
	 { 
            double yd   = tmpData->GetBinContent(id);
	    double eyd  = tmpData->GetBinError(id);
	    double ymc  = tmpMC->GetBinContent(imc);
	    double eymc = tmpMC->GetBinError(imc);
	    double dd   = eyd*eyd + eymc*eymc;
	    if ( dd > 0. )
	    { 
	       fChi2 += ( (yd-ymc)*(yd-ymc) ) / dd ;
	       ++ndf;
            }
            break;
	 }
      }
   }
  
   delete tmpData;
   delete tmpMC;
  
   return fChi2;

}

double Chi2Calc::Chi2DataMC( const TGraphErrors* data, 
                             const TH1* mc, 
			     int& ndf, 
			     const double mcscale /* = 1. */ )
{

   fChi2 = 0.;
   ndf   = 0;

// NOTE: Clone() creates a **new** copy,
//       so it'll need to be deleted afterwards
//
   TH1D* tmpMC   = (TH1D*)mc->Clone("tmp_mc_chi2");

   if( tmpMC->GetSumw2N() == 0 ) tmpMC->Sumw2();

   tmpMC->Scale(mcscale);
   
   const int nxd = data->GetN();
   double*   xd  = data->GetX();
   double*   yd  = data->GetY();
   double*   eyd = data->GetEY();
   const int x1MC   = tmpMC->GetXaxis()->GetFirst();
   const int x2MC   = tmpMC->GetXaxis()->GetLast();
   
   for ( int imc=x1MC; imc<=x2MC; ++imc )
   {
      double xmc  = tmpMC->GetBinLowEdge(imc);
      double dxmc = tmpMC->GetBinWidth(imc);
      for ( int id=0; id<nxd; ++id )
      {
         if ( xd[id] > xmc && xd[id] < xmc+dxmc )
	 {
	    double ymc  = tmpMC->GetBinContent(imc);
	    double eymc = tmpMC->GetBinError(imc);
	    double dd   = eymc*eymc + eyd[id]*eyd[id];
            if ( dd > 0. )
	    {
	       fChi2 += ( (yd[id]-ymc)*(yd[id]-ymc) ) / dd ;
	       ++ndf;
	    }	    
	    break;
	 }
      }
   }
   
   delete tmpMC;

   return fChi2;

}
