#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "TFile.h"
#include "TDirectory.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TKey.h"
#include "TClass.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TMatrixD.h"

#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/BeamThinTargetConfig.hh"
#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/RecordChi2.hh"

#include "Cintex/Cintex.h"


using namespace std;

int main( int argc, char** argv )
{

   string fname = argv[1];
   
   ROOT::Cintex::Cintex::Enable();

   vector<string> SecPartName;
   SecPartName.push_back("piplus");
   SecPartName.push_back("piminus");
   SecPartName.push_back("proton");
   SecPartName.push_back("neutron");
   
   vector< pair< TObjArray, vector< pair< string, pair<double,double> > > > > config_vs_results;
   
   map< string, vector< pair< string, pair<double,double> > > > defaultchi2;
   defaultchi2.insert( pair< string, 
                             vector< pair< string, pair<double,double> > > >( "Bertini", 
				                                              vector< pair< string, pair<double,double> > >() ) );
   map< string, vector< pair< string, pair<double,double> > > >::iterator idefchi2 = defaultchi2.find( "Bertini" );
   // in general, need to check if idefchi2 is valid (not at the end())

   // open the input file
   //
   TFile* hfile = TFile::Open( fname.c_str(), "UPDATE" );
   // first of all, remove parameters analysis directory, if exists
   //
   if ( hfile->GetDirectory( "ParametersCovMtx" ) )
   {
      hfile->Delete( "ParametersCovMtx;*" );
   }

   if ( hfile->GetDirectory( "ParametersAnalysis" ) )
   {
      hfile->Delete( "ParametersAnalysis;*" );
   }

   // NOTE(JVY): While in memory, those are TObject(s) (OBJ), not TKey(s) (KEY) !!!
   //
   // --> TIter next( hfile.GetList() );
   // --> TObject* obj = next();
   //
   TIter next( hfile->GetListOfKeys() );
   TKey*     key = (TKey*)next();

   // 1st loop over directories, to find out parameters and their ranges/limits
   //
   while (key )
   {

      // ---> if ( !(TClass::GetClass(obj->ClassName())->InheritsFrom(TDirectory::Class())) ) 
      if ( !(TClass::GetClass(key->GetClassName())->InheritsFrom(TDirectory::Class())) ) 
      {
	 key = (TKey*)next();
	 continue;
      }

      TObject* obj = key->ReadObj();
      string objname = obj->GetName();

      RecordChi2* rchi2 = 0;

      if ( objname.find( "Default" ) != string::npos )
      {
	 for ( size_t ip=0; ip<SecPartName.size(); ++ip )
	 {
            string rchi2name = objname + "/" + SecPartName[ip] + "_RecordChi2";
	    rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str() );
	    if ( !rchi2 ) 
	    { 
	       cout << " can not find RecordChi2: " << rchi2name << endl;
	       continue;
	    }
	    (idefchi2->second).push_back( make_pair( SecPartName[ip], make_pair(rchi2->GetSumChi2(),rchi2->GetSumNDF()) )  );
         }
	 
         key = (TKey*)next();
	 continue;
      } 

      TObjArray* objarr = (TObjArray*)hfile->Get( (objname+"/Geant4ModelConfig").c_str() );
      if ( !objarr ) 
      {
         key = (TKey*)next();
	 continue;
      } 
      
      bool conf_exist = false;
      size_t icr = 0;
      for ( int iobj=0; iobj<objarr->GetEntries(); ++iobj )
      {
         TObjString* curstr = (TObjString*)objarr->At(iobj);
	 icr = 0;
	 for (  ; icr<config_vs_results.size(); ++icr )
	 {
	    conf_exist = true;
	    TObjString* cfgstr = (TObjString*)(config_vs_results[icr].first)[iobj];
	    if ( ! curstr->IsEqual( cfgstr ) )
	    {
		  conf_exist = false;
	    }
	    if ( conf_exist ) break;
	 }	 
      }

      if ( ! conf_exist )
      {
         config_vs_results.push_back( make_pair( TObjArray(), vector< pair< string, pair<double,double> > >() ) );
	 // config_vs_results.back().first.SetOwner(kTRUE);
	 for ( int iobj=0; iobj<objarr->GetEntries(); ++iobj )
	 {
	    
	    TObjString* ss = (TObjString*)objarr->At(iobj);
	    config_vs_results.back().first.Add( new TObjString( *ss ) );
	 }
	 icr = config_vs_results.size() - 1;
      }
	 
      for ( size_t ip=0; ip<SecPartName.size(); ++ip )
      {
         string rchi2name = objname + "/" + SecPartName[ip] + "_RecordChi2";
	 rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str() );
	    
	 if ( !rchi2 ) 
	 { 
	       // --> cout << " can not find RecordChi2: " << rchi2name << endl;
	       continue;
	 }
	 
	 config_vs_results[icr].second.push_back(  make_pair( SecPartName[ip], make_pair( rchi2->GetSumChi2(), rchi2->GetSumNDF() ) ) );
      
      }      
      
      key = (TKey*)next();

   }

//   hfile->mkdir( "ParametersCovMtx" );
//   hfile->cd( "ParametersCovMtx" );
   
   map< string, vector< pair<string,double> > > pdefaults;
   pdefaults.insert( pair< string, 
                     vector< pair<string,double> > >( "Bertini", vector< pair<string,double> >() ) ); 
   map< string, vector< pair<string,double> > >::iterator imod = pdefaults.find( "Bertini" );
   if ( imod != pdefaults.end() )
   {
      (imod->second).push_back( pair<string,double>( "RadiusScale", 2.82 ) );
      (imod->second).push_back( pair<string,double>( "XSecScale", 1.0 ) );
      (imod->second).push_back( pair<string,double>( "FermiScale", 0.685 ) );
      (imod->second).push_back( pair<string,double>( "TrailingRadius", 0.0 ) );
   }    

   int np = (imod->second).size();
      
   TH1D* hdefault = new TH1D( "Defaults", "Default Parameters", np, 0., (double)np );

   double defwt = 1.; // fair enough if we do this: wt=epx(-0.5*(totchi2-deftotchi2))
   for ( int ip=0; ip<np; ++ip )
   {
      hdefault->SetBinContent( ip+1, (imod->second)[ip].second );
   }

   double deftotchi2 = 0.;
   double deftotndf = 0.;
   for ( size_t ip=0; ip<(idefchi2->second).size(); ++ip )
   {
      deftotchi2 += ((idefchi2->second)[ip].second).first;
      deftotchi2 += ((idefchi2->second)[ip].second).second;   
   }

   map< string, vector< pair<TH1D*,double> > > variants;
   variants.insert( pair< string, vector< pair<TH1D*,double> > >( "Bertini", vector< pair<TH1D*,double> >() ) );
   map< string, vector< pair<TH1D*,double> > >::iterator imov = variants.find("Bertini");

   double maxchi2 = -1.e-10;
   double minchi2 =  1.e+25;
   int    imin    = -1 ;
   
   map<double,int> themap;
   
   for (size_t ic=0; ic<config_vs_results.size(); ++ic )
   {
      bool mod_exist = false;
      for ( int ip=0; ip<config_vs_results[ic].first.GetEntries(); ++ip )
      {
         TObjString* ss = (TObjString*)config_vs_results[ic].first.At(ip);
	 if ( ss->GetString().Contains( (imov->first).c_str() ) ) 
	 {
	    mod_exist = true;
	    break;
	 }	 
      }
      if ( ! mod_exist ) continue;
      string hname = (imov->first) + "_Var" + to_string(ic);
      double totchi2 = 0.;
      double totndf = 0.;
      for ( size_t isec=0; isec<config_vs_results[ic].second.size(); ++isec )
      {
         double chi2 = config_vs_results[ic].second[isec].second.first;
	 totchi2 += chi2;
	 double ndf = config_vs_results[ic].second[isec].second.second;
	 totndf += ndf;	 
      }
      if ( totchi2 > maxchi2 ) maxchi2 = totchi2;
      if ( totchi2 < minchi2 ) 
      {
         minchi2 = totchi2;
	 imin = ic;
      } 
      
      themap.insert( pair<double,int>(totchi2,ic) );
      
      (imov->second).push_back( make_pair( new TH1D( hname.c_str(), "", np, 0., (double)np ), totchi2 ) );
      for ( int icp=0; icp<config_vs_results[ic].first.GetEntries(); ++icp )
      {
         TObjString* ss = (TObjString*)config_vs_results[ic].first.At(icp);
	 string str = ss->GetString().Data();
	 size_t idx1 = str.find(":");
	 string model = str.substr(0,idx1);
	 size_t idx2 = str.find_first_not_of(" ", idx1+1);
	 size_t idx3 = str.find("=");
	 string pname = str.substr(idx2,(idx3-idx2));
	 string sval = str.substr( str.find("=")+1 );
	 double val = stod(sval); 
	 for ( int ip=0; ip<np; ++ip )
	 {
	    if ( (imod->second)[ip].first == pname )
	    {
	       (imov->second).back().first->SetBinContent( ip+1, val );
	    }
	 }
	      
      }
   }
   
   cout << " ================================ " << endl;

   map<double,int>::iterator theitr=themap.begin();
   for ( ; theitr!=themap.end(); ++theitr )
   {
      if ( (theitr->first-minchi2) > 1400. ) break;
      cout << " totchi2 = " << theitr->first << " pos = " << theitr->second << endl;
      config_vs_results[theitr->second].first.Print();
   }


   cout << " ================================ " << endl;
   
   cout << " Minimal chi2 = " << minchi2 << " for configuration" << endl;
   config_vs_results[imin].first.Print();

   // OK, prep work is done - for Bertini
   // now can calculate covariance matrix
   //
   
   TMatrixD covmtx( np, np );   
   
   TH1D hmean = TH1D( *hdefault );
   hmean.Reset();
   
   double totwt = 0.;
   vector<double> weights;
   
   int nh = (imov->second).size();
   
   for ( int ih=0; ih<nh; ++ih)
   {
      double chi2 = (imov->second)[ih].second;
      double wt   = exp( -0.5*(chi2-minchi2) );
/*
      double wt   = 1.; 
      if ( fabs(chi2-deftotchi2) >= 1400 )
      {
         wt = 0.;
      } 
      else
      {
         wt = exp( -0.5*(chi2-deftotchi2) );
      }
*/
      weights.push_back( wt );
      hmean.Add( ((imov->second)[ih].first), wt );
      totwt += wt;
   }
   
   if ( totwt > 0. )
   {
      hmean.Scale( 1./totwt );
   }
   
   for ( int ih=0; ih<nh; ++ih )
   {
       if ( weights[ih] > 0. )
       {
          for ( int ib1=0; ib1<np; ++ib1 )
          {
             double xmean1 = hmean.GetBinContent(ib1+1);
	     double xib1   = (imov->second)[ih].first->GetBinContent(ib1+1);
	     for ( int ib2=ib1; ib2<np; ++ib2 )
	     {
	        double xmean2 = hmean.GetBinContent(ib2+1);
	        double xib2   = (imov->second)[ih].first->GetBinContent(ib2+1);
	        covmtx[ib1][ib2] += weights[ih] * (xib1-xmean1) * (xib2-xmean2); 
	     }
          }
       }  
   }

   for ( int ib1=0; ib1<np; ++ib1 )
   {
      for ( int ib2=ib1; ib2<np; ++ib2 )
      {
         covmtx[ib1][ib2] /= totwt;
	 covmtx[ib2][ib1] = covmtx[ib1][ib2];
      }
   }
   
   covmtx.Print();

   cout << " ================================ " << endl;

   for ( int ih=0; ih<nh; ++ih)
   {
      double chi2 = (imov->second)[ih].second;
      double wt   = exp( -0.5*(chi2-minchi2) );
      cout << " RadiusScale = " << (imov->second)[ih].first->GetBinContent(1) << "    chi2 = " << chi2 << "    wt = " << wt << endl;
   }

   cout << " ================================ " << endl;

   for ( int ih=0; ih<nh; ++ih)
   {
      double chi2 = (imov->second)[ih].second;
      double wt   = exp( -0.5*(chi2-minchi2) );
      cout << " XSecScale = " << (imov->second)[ih].first->GetBinContent(2) << "    chi2 = " << chi2 << "    wt = " << wt << endl;
   }

   cout << " ================================ " << endl;

   for ( int ih=0; ih<nh; ++ih)
   {
      double chi2 = (imov->second)[ih].second;
      double wt   = exp( -0.5*(chi2-minchi2) );
      cout << " FermiScale = " << (imov->second)[ih].first->GetBinContent(3) << "    chi2 = " << chi2 << "    wt = " << wt << endl;
   }

   cout << " ================================ " << endl;

   for ( int ih=0; ih<nh; ++ih)
   {
      double chi2 = (imov->second)[ih].second;
      double wt   = exp( -0.5*(chi2-minchi2) );
      cout << " TrailingRadius = " << (imov->second)[ih].first->GetBinContent(3) << "    chi2 = " << chi2 << "    wt = " << wt << endl;
   }
   


   return 0;

}

