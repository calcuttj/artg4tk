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

#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/BeamThinTargetConfig.hh"
#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/RecordChi2.hh"

// --> NOT in Root6 #include "Cintex/Cintex.h"

int main( int argc, char** argv )
{

   std::string fname = argv[1];
   
// --> NOT in Root6   ROOT::Cintex::Cintex::Enable();
   
   std::vector<std::string> SecPartName;
   SecPartName.push_back("piplus");
   SecPartName.push_back("piminus");
   SecPartName.push_back("proton");
   SecPartName.push_back("neutron");
   
   double chi2thr = 1383.5; // 5GeV/c proton beam
   // double chi2thr = 7385.14; // 5GeV/c pi- beam
/*
   double chi2thr = 2250.;
   if ( fname.find("piminus") != std::string::npos || fname.find("piplus") != std::string::npos ) chi2thr = 4500.;  
*/  
      
   std::map< std::string, std::map< std::string, std::vector<double> > > mparams;
   
   std::map< std::string, std::map< std::string, std::map< double, std::vector< std::pair<std::string,std::pair<double,double> > > > > > parvschi2;
      
   TFile* hfile = TFile::Open( fname.c_str(), "UPDATE" );

   // first of all, remove parameters analysis directory, if exists
   //
   if ( hfile->GetDirectory( "ParametersAnalysis" ) )
   {
      hfile->Delete( "ParametersAnalysis;*" );
   }
   if ( hfile->GetDirectory( "ParametersCovMtx" ) )
   {
      hfile->Delete( "ParametersCovMtx;*" );
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

      std::string objname = obj->GetName();
      if ( objname.find( "Default" ) != std::string::npos )
      {
         key = (TKey*)next();
	 continue;
      } 
       
      TObjArray* objarr = (TObjArray*)hfile->Get( (objname+"/Geant4ModelConfig").c_str() );
      if ( !objarr ) continue;
      
      // objarr->Print();
      
      for ( int i=0; i<objarr->GetEntries(); ++i )
      {
         TObjString* sobj = (TObjString*)objarr->At(i);
	 std::string str = sobj->GetString().Data();
	 size_t idx1 = str.find(":");
	 std::string model = str.substr(0,idx1);
	 size_t idx2 = str.find_first_not_of(" ", idx1+1);
	 size_t idx3 = str.find("=");
	 std::string pname = str.substr(idx2,(idx3-idx2));
	 std::string sval = str.substr( str.find("=")+1 );
	 double val = std::stod(sval);
         std::map< std::string, std::map< std::string,std::vector<double> > >::iterator itr1=mparams.find(model);
	 if ( itr1 == mparams.end() )
	 {
	    mparams.insert( std::pair< std::string, std::map< std::string, std::vector<double> > >( model, std::map<std::string, std::vector<double> >() ) );
	    itr1=mparams.find(model);
	 }
	 std::map< std::string, std::vector<double> >::iterator itr2=(itr1->second).find(pname);
	 if ( itr2 == (itr1->second).end() )
	 {
	    (itr1->second).insert( std::pair< std::string, std::vector<double> >( pname, std::vector<double>() ) );
	    itr2=(itr1->second).find(pname);
	 }
	 bool pexists = false;
	 for ( size_t ip=0; ip<(itr2->second).size(); ++ip )
	 {
	    if ( std::fabs( (itr2->second)[ip] - val ) < 1.e-10 )
	    {
	       pexists = true;
	       break;
	    } 
	 }
	 if ( !pexists ) (itr2->second).push_back(val);
	 
         std::map< std::string, std::map< std::string, std::map< double, std::vector< std::pair<std::string,std::pair<double,double> > > > > >::iterator ichi2_1=parvschi2.find(model);
	 if ( ichi2_1 == parvschi2.end() )
	 {
	    parvschi2.insert( std::pair< std::string, 
	                                 std::map< std::string, 
					           std::map< double, 
						             std::vector< std::pair<std::string,
							                            std::pair<double,double> > > > > >( model, 
										                                        std::map< std::string, 
						                                                                                  std::map< double, 
															          std::vector< std::pair<std::string,std::pair<double,double>> > > >() ) );
	    ichi2_1=parvschi2.find(model);
	 }
	 std::map< std::string, std::map< double, std::vector< std::pair<std::string,std::pair<double,double> > > > >::iterator ichi2_2=(ichi2_1->second).find(pname);
	 if ( ichi2_2 == (ichi2_1->second).end() )
	 {
	    (ichi2_1->second).insert( std::pair< std::string, 
	                                         std::map< double, 
						           std::vector< std::pair<std::string,std::pair<double,double> > > > >( pname, 
							                                                                        std::map< double, 
							                                                                                  std::vector< std::pair<std::string,std::pair<double,double> > > >() ) );
	    ichi2_2=(ichi2_1->second).find(pname);
	 }
	 pexists = false;
	 if ( (ichi2_2->second).find(val) != (ichi2_2->second).end() )
	 {
	    pexists = true;
	 }
	 else
	 {
	    (ichi2_2->second).insert( std::pair< double, 
	                                         std::vector< std::pair<std::string,std::pair<double,double> > > >(  val, 
						                                                                     std::vector< std::pair<std::string,std::pair<double,double> > >() ) );
	 }
      }
           
      key = (TKey*)next();
   }

   // now book histograms
   //
   std::map< std::string, std::vector<TProfile*> > histos;

   std::map< std::string, std::map< std::string,std::vector<double> > >::iterator itr1=mparams.begin();
   for ( ; itr1!=mparams.end(); ++itr1 )
   {
      std::map< std::string, std::vector<double> >::iterator itr2=(itr1->second).begin();
      for ( ; itr2!=(itr1->second).end(); ++itr2 )
      {
         double pmin = 1.e25;
         double pmax = -1.e25;
	 std::string hname = itr1->first + itr2->first;
	 for ( size_t i=0; i<(itr2->second).size(); ++i )
	 {
	    if ( (itr2->second)[i] < pmin ) pmin = (itr2->second)[i];
	    if ( (itr2->second)[i] > pmax ) pmax = (itr2->second)[i];
	 }
         double fractpart, intpart;
         fractpart = modf( pmin, &intpart );
	 // double hmin = std::min( 0., intpart-1.) ;
	 double hmin = intpart;
	 fractpart = modf( pmax, &intpart );
	 double hmax = intpart+1.;
         for ( size_t i=0; i<SecPartName.size(); ++i )
         {
	    histos.insert( std::pair< std::string, std::vector<TProfile*> >( SecPartName[i], std::vector<TProfile*>() )  );
	    std::map< std::string, std::vector<TProfile*> >::iterator ih=histos.find( SecPartName[i] );
	    (ih->second).push_back( new TProfile( ( hname + "_Chi2_" + SecPartName[i] ).c_str(), "", 25, hmin, hmax, 0., 50000. ) );
	    (ih->second).back()->GetXaxis()->SetTitle( ( itr1->first + " " + itr2->first ).c_str() );
	    (ih->second).back()->GetYaxis()->SetTitle( ( "Chi2" ) );
	    (ih->second).push_back( new TProfile( ( hname + "_Chi2NDF_" + SecPartName[i] ).c_str(), "", 25, hmin, hmax, 0., 50000. ) );
	    (ih->second).back()->GetXaxis()->SetTitle( ( itr1->first + " " + itr2->first ).c_str() );
	    (ih->second).back()->GetYaxis()->SetTitle( ( "Chi2/NDF" ) );
	    (ih->second).push_back( new TProfile( ( hname + "_" + SecPartName[i] ).c_str(), "", 25, hmin, hmax, 0., 50000. ) );
	    (ih->second).back()->GetXaxis()->SetTitle( ( itr1->first + " " + itr2->first ).c_str() );
	    (ih->second).back()->GetYaxis()->SetTitle( ( "Number of entries" ) );
	    (ih->second).push_back( new TProfile( ( hname + "_Wt_" + SecPartName[i] ).c_str(), "", 25, hmin, hmax, 0., 50000. ) );
	    (ih->second).back()->GetXaxis()->SetTitle( ( itr1->first + " " + itr2->first ).c_str() );
	    (ih->second).back()->GetYaxis()->SetTitle( ( "exp(-0.5*chi2)" ) );
         }
	 histos.insert( std::pair< std::string, std::vector<TProfile*> >( "total", std::vector<TProfile*>() )  );
	 std::map< std::string, std::vector<TProfile*> >::iterator ih=histos.find( "total" );
	 (ih->second).push_back( new TProfile( ( hname + "_total" ).c_str(), "", 25, hmin, hmax, 0., 50000. ) );
	 (ih->second).back()->GetXaxis()->SetTitle( ( itr1->first + " " + itr2->first ).c_str() );
	 (ih->second).back()->GetYaxis()->SetTitle( ( "Number of entries" ) );
	 (ih->second).push_back( new TProfile( ( hname + "_Chi2_total" ).c_str(), "", 25, hmin, hmax, 0., 50000. ) );
	 (ih->second).back()->GetXaxis()->SetTitle( ( itr1->first + " " + itr2->first ).c_str() );
	 (ih->second).back()->GetYaxis()->SetTitle( ( "Chi2" ) );
	 (ih->second).push_back( new TProfile( ( hname + "_Chi2NDF_total" ).c_str(), "", 25, hmin, hmax, 0., 50000. ) );
	 (ih->second).back()->GetXaxis()->SetTitle( ( itr1->first + " " + itr2->first ).c_str() );
	 (ih->second).back()->GetYaxis()->SetTitle( ( "Chi2/NDF" ) );
	 (ih->second).push_back( new TProfile( ( hname + "_Wt_total" ).c_str(), "", 25, hmin, hmax, 0., 50000. ) );
	 (ih->second).back()->GetXaxis()->SetTitle( ( itr1->first + " " + itr2->first ).c_str() );
	 (ih->second).back()->GetYaxis()->SetTitle( ( "exp(-0.5*chi2)" ) );
	 
      }
   }

   // second loop over directories, to extract the chi2, etc.
   //
   next.Reset();
   key = (TKey*)next();

   while (key )
   {

      // ---> if ( !(TClass::GetClass(obj->ClassName())->InheritsFrom(TDirectory::Class())) ) 
      if ( !(TClass::GetClass(key->GetClassName())->InheritsFrom(TDirectory::Class())) ) 
      {
	 key = (TKey*)next();
	 continue;
      }
      
      TObject* obj = key->ReadObj();

      std::string objname = obj->GetName();
      if ( objname.find( "Default" ) != std::string::npos )
      {
         key = (TKey*)next();
	 continue;
      } 
       
      RecordChi2* rchi2 = 0;
      double chi2 = 0.;
      double ndf  = 1;
      double pval  = 0.;
      std::string model = "";
      std::string pname = "";
      
      // fish out parameters (once again)
      //
      TObjArray* objarr = (TObjArray*)hfile->Get( (objname+"/Geant4ModelConfig").c_str() );
      if ( !objarr ) 
      {
         key = (TKey*)next();
	 continue;
      }
      
      // loop over parameters, and populate histograms for different secondaries and total
      //
      for ( int ipr=0; ipr<objarr->GetEntries(); ++ipr )
      {
         TObjString* sobj = (TObjString*)objarr->At(ipr);
	 std::string str = sobj->GetString().Data();
	 size_t idx1 = str.find(":");
	 model = str.substr(0,idx1);
	 size_t idx2 = str.find_first_not_of(" ", idx1+1);
	 size_t idx3 = str.find("=");
	 pname = str.substr(idx2,(idx3-idx2));
	 std::string sval = str.substr( str.find("=")+1 );
	 pval = std::stod(sval); 
	 
	 for ( size_t i1=0; i1<SecPartName.size(); ++i1 )
	 {
            std::string rchi2name = objname + "/" + SecPartName[i1] + "_RecordChi2";
	    rchi2 = (RecordChi2*)hfile->Get( rchi2name.c_str() );
	    if ( !rchi2 ) 
	    { 
	       // --> UNCOMMENT !!! std::cout << " can not find RecordChi2: " << rchi2name << std::endl;
	       continue;
	    }
	    // ---> rchi2->Print();
            chi2 = rchi2->GetSumChi2();
            ndf  = rchi2->GetSumNDF();
	    
            std::map< std::string, std::map< std::string, std::map< double, std::vector< std::pair<std::string,std::pair<double,double> > > > > >::iterator ichi2_1=parvschi2.find(model);
	    std::map< std::string, std::map< double, std::vector< std::pair<std::string,std::pair<double,double> > > > >::iterator ichi2_2=(ichi2_1->second).find(pname);
	    std::map< double, std::vector<std::pair<std::string,std::pair<double,double> > > >::iterator ichi2_3=(ichi2_2->second).find(pval);
	    (ichi2_3->second).push_back( std::make_pair( SecPartName[i1], std::make_pair(chi2,ndf) )  );
            
	    std::map< std::string, std::vector<TProfile*> >::iterator ih=histos.find( SecPartName[i1] );
	    for ( size_t i2=0; i2<(ih->second).size(); ++i2 )
	    {
	       std::string hname = (ih->second)[i2]->GetName();
	       if ( hname.find( model ) != std::string::npos && hname.find( pname ) != std::string::npos )
	       {
	          if ( hname.find( "_Chi2NDF_" ) != std::string::npos )
		  {
		     (ih->second)[i2]->Fill( pval, chi2/ndf );
		  }
		  else if ( hname.find( "_Chi2_" ) != std::string::npos )
		  {
		     (ih->second)[i2]->Fill( pval, chi2 ); 
		  }
		  else if ( hname.find( "_Wt_" ) != std::string::npos )
		  {
		     (ih->second)[i2]->Fill( pval, exp(-0.5*chi2) ); 
		  }
		  else
		  {
		     (ih->second)[i2]->Fill( pval, 1. );
		  }
	       } 
	    }
	 } // end loop over secondary particles
      }      

      key = (TKey*)next();

   }
   
   std::map< std::string, 
             std::map< std::string, 
	               std::map< double, std::vector< std::pair<std::string,std::pair<double,double> > > > > >::iterator itr=parvschi2.begin();
   
   double totchi2 = 0.;
   double totndf = 0.;
   // std::vector< std::pair<std::string,double> > chi2min;
   std::map<std::string,double> chi2min;
   std::map<std::string,double>::iterator itr_chi2min;
   
   // find min chi2 to use for normalization
   //
   
   for ( ; itr!=parvschi2.end(); ++itr )
   {
      std::map< std::string, std::map< double, std::vector< std::pair<std::string,std::pair<double,double> > > > >::iterator itr_1=(itr->second).begin();
      for ( ; itr_1!=(itr->second).end(); ++itr_1 )
      {
         std::string pname = itr_1->first;
	 chi2min.insert( std::pair<std::string,double>( pname, 1.e+25 ) );
	 itr_chi2min = chi2min.find(pname);
	 std::map< double, std::vector< std::pair<std::string,std::pair<double,double> > > >::iterator itr_2=(itr_1->second).begin();
	 for ( ; itr_2!=(itr_1->second).end(); ++itr_2 )
	 {
	    double pval = itr_2->first;
	    totchi2 = 0.;
	    totndf  = 0.;
	    for ( size_t irec=0; irec<(itr_2->second).size(); ++irec )
	    {
	       totchi2 += ((itr_2->second)[irec].second).first;
	       totndf  += ((itr_2->second)[irec].second).second;
	    }
	    // if  ( chi2min.back().second > totchi2 ) chi2min.back().second = totchi2; 
	    if ( itr_chi2min->second > totchi2 ) itr_chi2min->second = totchi2;
         }
      }      
   }
   
   
/*
   for ( itr_chi2min=chi2min.begin(); itr_chi2min!=chi2min.end(); ++itr_chi2min )
   {
      // std::cout << " parameter = " << chi2min[i].first << " --> chi2min = " << chi2min[i].second << std::endl;
      std::cout << " parameter = " << itr_chi2min->first << " --> chi2min = " << itr_chi2min->second << std::endl;
   }
*/   
   
   for ( itr=parvschi2.begin(); itr!=parvschi2.end(); ++itr )
   {
      std::string model = itr->first;
      std::map< std::string, std::map< double, std::vector< std::pair<std::string,std::pair<double,double> > > > >::iterator itr_1=(itr->second).begin();
      for ( ; itr_1!=(itr->second).end(); ++itr_1 )
      {
         std::string pname = itr_1->first;
	 std::map< double, std::vector< std::pair<std::string,std::pair<double,double> > > >::iterator itr_2=(itr_1->second).begin();
	 for ( ; itr_2!=(itr_1->second).end(); ++itr_2 )
	 {
	    double pval = itr_2->first;
	    totchi2 = 0.;
	    totndf  = 0.;
	    for ( size_t irec=0; irec<(itr_2->second).size(); ++irec )
	    {
	       totchi2 += ((itr_2->second)[irec].second).first;
	       totndf  += ((itr_2->second)[irec].second).second;
	    }
            std::map< std::string, std::vector<TProfile*> >::iterator ih=histos.find( "total" );
            // std::cout << " pname = " << pname << " totchi2 = " << totchi2 << " minchi2 = " << chi2min[pname] << std::endl;
	    for ( size_t i3=0; i3<(ih->second).size(); ++i3 )
	    {
	       std::string hname = (ih->second)[i3]->GetName();
	       // std::cout << " hname = " << hname << std::endl;
	       if ( hname.find( model ) != std::string::npos && hname.find( pname ) != std::string::npos )
	       {
	          if ( hname.find( "_Chi2_" ) != std::string::npos )
	          {
		     (ih->second)[i3]->Fill( pval, totchi2 );
		     continue;
	          }
	          if ( hname.find( "_Chi2NDF_" ) != std::string::npos )
	          {
		     if ( totndf > 0. ) (ih->second)[i3]->Fill( pval, totchi2/totndf );
		     continue;
	          }
	          else if ( hname.find( "_Wt_" ) != std::string::npos )
	          {
		     // double wt = exp( -0.5*(totchi2-chi2thr) );
		     double wt = exp( -0.5 * (totchi2-chi2min[pname]*0.99) );
		     (ih->second)[i3]->Fill( pval, wt ); 
		     // ---> (ih->second)[i3]->Fill( pval, exp(-0.5*(totchi2-chi2thr)) ); 
		     continue;
	          }
	          else
	          {
		     (ih->second)[i3]->Fill( pval, 1. );
	          }
	       }	       
	    }
	 }	 
      }
   }
         
   hfile->mkdir( "ParametersAnalysis" );
   hfile->cd( "ParametersAnalysis" );
   
   std::map< std::string, std::vector<TProfile*> >::iterator ih=histos.begin();
   for ( ; ih!=histos.end(); ++ih )
   {
      for ( size_t i=0; i<(ih->second).size(); ++i )
      {
         std::string hname = (ih->second)[i]->GetName();
	 std::string sname = "_Wt_";
	 size_t pos = hname.find( sname );
	 if ( pos != std::string::npos )
	 {
	    hname.erase( pos, sname.length()-1 );
	    for ( size_t i1=0; i1<(ih->second).size(); ++i1 )
	    {
	       if ( hname == ((ih->second)[i1]->GetName()) )
	       {
	          double Integral = (ih->second)[i1]->Integral();
		  double Integral_Wt = (ih->second)[i]->Integral();
		  double scale = 1. ;
		  if ( Integral_Wt > 1.e-307) scale = Integral/Integral_Wt;
		  TProfile* hwtnorm = new TProfile( *((ih->second)[i]) );
		  std::string tname = (ih->second)[i]->GetName();
		  pos = tname.find(sname );
		  tname.insert ( pos+sname.length(), "norm_" ); 
		  hwtnorm->SetName( tname.c_str() );
		  hwtnorm->Scale( scale );
		  hwtnorm->Write();
		  break;
	       }
	    } 
	 }
	 
	 (ih->second)[i]->Write();
      }
   }

   hfile->Close();

   return 0;

}
