
#include <iostream>
#include "artg4tk/ExpDataAccess/JSON2Data.hh"
#include "boost/foreach.hpp"
// --> no need as it comes via parser --> #include <boost/property_tree/ptree.hpp>
#include "boost/property_tree/json_parser.hpp"

#include "TAxis.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

MetaData::MetaData()
   : /* fBeamPID(0), fBeamMomentum(0.), */ fBeamLink(-1), fTargetNucleus(0), fSecondaryPID(0),
                                           fTitle(""), 
					   fRefLink(-1)
{
}

MetaData::MetaData( const MetaData& rhs )
   : // fBeamPID(rhs.fBeamPID),
     // fBeamMomentum(rhs.fBeamMomentum),
     fBeamLink(rhs.fBeamLink),
     fTargetNucleus(rhs.fTargetNucleus),
     fSecondaryPID(rhs.fSecondaryPID),
     fTitle(rhs.fTitle),
     fRefLink(rhs.fRefLink),
     fParameters(rhs.fParameters)
{
}

MetaData::~MetaData()
{
}

bool MetaData::operator==( const MetaData& rhs ) const
{
   
   if ( /* fBeamPID==rhs.fBeamPID && (fBeamMomentum-rhs.fBeamMomentum)<=1.e-10 && */
        fBeamLink==rhs.fBeamLink &&
        fTargetNucleus==rhs.fTargetNucleus && fSecondaryPID==rhs.fSecondaryPID )
   {
      return true;
   }

   return false;
   
}

bool MetaData::IsMatch( const int& beamid, const double& mom,
                        const int& tgt,    const int& secid ) const
{

   if ( IsBeamTargetMatch( beamid, mom, tgt ) &&
	secid == fSecondaryPID )
   {
      return true;
   }
   
   return false;

}

bool MetaData::IsBeamTargetMatch( const int& beamid, const double& mom, 
                                  const int& tgt ) const
{

   if ( EmulateBeamLink( beamid, mom ) == fBeamLink &&
        tgt == fTargetNucleus )
   {
      return true;
   }
   
   return false;

}

int MetaData::EmulateBeamLink( const int& pid, const double& mom ) const
{

   int blnk = -1;
   
   // HARP-like beam defs 
   // (some overlaps with ITEP771, e.g. 5GeV/c pions) 
   //
   if ( pid == 2212 )
   {
      if ( fabs(mom-158.) <= 1.e-10 )
      {
         blnk = 7;
      }
      else if ( fabs(mom-3.) <= 1.e-10 )
      {
         blnk = 37;
      }
      else if ( fabs(mom-5.) <= 1.e-10 )
      {
         if ( fRefLink == 17 )
	 {
	    blnk = 70;
	 }
	 else
	 {
	    blnk = 38;
	 }
      }
      else if ( fabs(mom-8.) <= 1.e-10 )
      {
         blnk = 39;
      }
      else if ( fabs(mom-8.9) <= 1.e-10 )
      {
         blnk = 40;
      }
      else if ( fabs(mom-12.) <= 1.e-10 )
      {
         blnk = 41;
      }
   }
   else if (pid == 211 )
   {
      if ( fabs(mom-3.) <= 1.e-10 )
      {
         if ( fRefLink == 17 )
	 {
	    blnk = 71;
	 }
	 else
	 {
	    blnk = 42;
	 }
      }
      else if ( fabs(mom-5.) <= 1.e-10 )
      {
         if ( fRefLink == 17 )
	 {
	    blnk = 72;
	 }
	 else
	 {
            blnk = 43;
	 }
      }
      else if ( fabs(mom-8.) <= 1.e-10 )
      {
         blnk = 44;
      }
      else if ( fabs(mom-12.) <= 1.e-10 )
      {
         blnk = 45;
      }
   }
   else if ( pid == -211 )
   {
      if ( fabs(mom-3.) <= 1.e-10 )
      {
         blnk = 46;
      }
      else if ( fabs(mom-5.) <= 1.e-10 )
      {
         if ( fRefLink == 17 )
	 {
	    blnk = 73;
	 }
	 else
	 {
            blnk = 47;
	 }
      }
      else if ( fabs(mom-8.) <= 1.e-10 )
      {
         blnk = 48;
      }
      else if( fabs(mom-12.) <= 1.e-10 )
      {
         blnk = 49;
      }
   }
   
//   std::cout << " beam = " << pid << " momentum = " << mom << " beamlink = " << blnk << std::endl;
   
   return blnk;

}


using namespace boost;
using namespace boost::property_tree;

// #include "rapidjson/document.h"
// #include "rapidjson/error/en.h"

void JSON2Data::ClearMetaData()
{

   fMetaData.fBeamLink = -1;
   fMetaData.fTargetNucleus = 0;
   fMetaData.fSecondaryPID = 0;
   fMetaData.fTitle.clear();
   fMetaData.fTitle = "";
   fMetaData.fRefLink = -1;
   fMetaData.fParameters.clear();

   return;

}

void JSON2Data::ParseMetaData( const std::string& jstr )
{

   ClearMetaData();
   
   std::stringstream ss(jstr.c_str()); 

   ptree pt;
   read_json( ss, pt );
   
   fMetaData.fBeamLink      = pt.get<int>("beamlnk");
   fMetaData.fTargetNucleus = pt.get<int>("targetlnk");
   fMetaData.fSecondaryPID  = pt.get<int>("secondarylnk");
   
   fMetaData.fTitle         = pt.get<std::string>("datatable.title");
   
   fMetaData.fRefLink = pt.get<int>("referencelnk");
   
/*
   BOOST_FOREACH( ptree::value_type &v, pt.get_child("parnames") )
   {
      //std::cout << " parnames: " << v.second.data() << std::endl;
      fParameters.push_back( v.second.data() );
   }
*/
   return;

}

TH1D* JSON2Data::Convert2Histo( const std::string& jstr, const char* hname )
{

   if ( fHisto )
   {
      delete fHisto;
      fHisto = 0;
   }
   
   ParseMetaData( jstr );
   
   std::stringstream ss(jstr.c_str()); 
   
   ptree pt;
   read_json( ss, pt );
   
//   std::string dtype = pt.get<std::string>("datatable.datatypeslnk");
   int dtype = pt.get<int>("datatable.datatypeslnk");
//   if ( dtype == "1000" || dtype == "1001" )
   if ( dtype >= 1000 )
   {
     // 1D Datapoint set - can NOT convert to TH1D since the binning is unknowns
     //
     mf::LogInfo log;
     log << " Can NOT convert 1D Datapoints set into TH1D since the binning is unknown" << std::endl;
     return NULL;
   }   
   
   int     nbins = 0;
   double* cv = 0;
   double* estat = 0;
   double* esys = 0;
   double* xx = 0;
   
   std::vector<int> nb;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.nbins") )
   {
         nb.push_back( atoi( v.second.data().c_str() ) );
   }
   nbins = nb[0];

   cv = new double[ nbins ];
   estat = new double[ nbins ];
   esys = new double[ nbins ];
   xx = new double[ nbins+1 ];
   for ( int i=0; i<nbins; ++i )
   {
      cv[i] = 0.;
      estat[i] = 0.;
      esys[i] = 0.;
      xx[i] = 0.;
   }
   xx[nbins] = 0.;

   // get "central values"
   //
   int cnt = 0;
   BOOST_FOREACH( ptree::value_type &v, pt.get_child("datatable.val") )
   {
       // std::cout << " datatable.val: " << v.second.data() << std::endl;
       //cv.push_back( atof( v.second.data().c_str() ) );
      cv[cnt] = atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= nbins ) break; 
   }
   
   // get binning (x-axis, etc...)
   //
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.binMin") )
   {
      xx[cnt] = atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= nbins ) break;
   }
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.binMax") )
   {
      // override the upper bin edge
      //
      xx[cnt+1] = atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= nbins ) break;
   }
      
   // now get errors
   //
   // stat errors first
   //
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.errStatMinus") )
   {
      estat[cnt] = atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= nbins ) break;
   }
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.errStatPlus") )
   {
      estat[cnt] += atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= nbins ) break;
   }
   
   // now sys err
   //
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.errSysMinus") )
   {
      esys[cnt] = atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= nbins ) break;
   }
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.errSysPlus") )
   {
      esys[cnt] += atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= nbins ) break;
   }

   // calculate total error
   //
   double* etot = new double[ nbins ];
   
   for ( int i=0; i<nbins; ++i )
   {
      etot[i] = sqrt( estat[i]*estat[i] + esys[i]*esys[i] );
   }

   std::string gtitle = pt.get<std::string>( "datatable.title" );
   
   std::vector<std::string> axtitle;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.axisTitle" ) )
   {
      axtitle.push_back( v.second.data() );
   }

   fHisto = new TH1D( hname, gtitle.c_str(), nbins, xx );
   fHisto->GetXaxis()->SetTitle( axtitle[0].c_str() );
   fHisto->GetYaxis()->SetTitle( axtitle[1].c_str() );
   
   for ( int i=1; i<=nbins; ++i )
   {
      fHisto->SetBinContent( i, cv[i-1] );
      fHisto->SetBinError( i, etot[i-1] );
   } 

   delete [] cv;
   delete [] estat;
   delete [] esys;
   delete [] etot;
   delete [] xx;
   
   return fHisto;

}

TGraphErrors* JSON2Data::Convert2Graph( const std::string& jstr )
{


   // FIXME !!!
   // Needs cleanup and certain "generalization"
   //
   // NOTE: Histo-like exp.datasets can be presented as a Graph
   //       Graph-like datasets can NOT be presented as a Histo since binning is unknown
   //       TGraph(Errors)::GetHistohram() only "books" a histo but  a does NOT populate it
   //       with graph's contents (values, errors,...) !!!... so it's kind of useless...
   

   if ( fGraph ) 
   {
      delete fGraph;
      fGraph = 0;
   }
   
/*   
   rapidjson::Document jdoc;
   if (jdoc.Parse(jstr.c_str()).HasParseError()) 
   {
      std::cout << " Can NOT parse input " << jstr << std::endl;
   }
*/

   ParseMetaData( jstr );

   std::stringstream ss(jstr.c_str()); 

   ptree pt;
   read_json( ss, pt );
   
   int     nbins = 0;
   double* cv = 0;
   double* estat = 0;
   double* esys = 0;
   double* xx = 0;
   
   // First of all, find oud what kind of record it is
   //
   // std::string dtype = pt.get<std::string>("datatable.datatypeslnk");
   int dtype = pt.get<int>("datatable.datatypeslnk");
//   if ( dtype == "1000" || dtype == "1001" )
   if ( dtype >= 1000 )
   {
     // 1D Datapoint set - related to datatable.npoints
     //
     nbins = pt.get<int>("datatable.npoints");
   }
   else
   {
      std::vector<int> nb;
      BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.nbins") )
      {
         nb.push_back( atoi( v.second.data().c_str() ) );
      }
      nbins = nb[0];
   }
   
   // assert ( (nbins > 0) && cv && estat && esys && xx ); 

   cv = new double[ nbins ];
   estat = new double[ nbins ];
   esys = new double[ nbins ];
   xx = new double[ nbins ];
   for ( int i=0; i<nbins; ++i )
   {
      cv[i] = 0.;
      estat[i] = 0.;
      esys[i] = 0.;
      xx[i] = 0.;
   }
   
   // get "central values"
   //
   int cnt = 0;
   BOOST_FOREACH( ptree::value_type &v, pt.get_child("datatable.val") )
   {
       //std::cout << " datatable.val: " << v.second.data() << std::endl;
       //cv.push_back( atof( v.second.data().c_str() ) );
      cv[cnt] = atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= nbins ) break; 
   }
   
   // get binning (x-axis, etc...)
   //
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.binMin") )
   {
      xx[cnt] = atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= nbins ) break;
   }
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.binMax") )
   {
      xx[cnt] += atof( v.second.data().c_str() );
      xx[cnt] /= 2.;
      cnt++;
      if ( cnt >= nbins ) break;
   }
   
   // now get errors
   //
   // stat errors first
   //
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.errStatMinus") )
   {
      estat[cnt] = atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= nbins ) break;
   }
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.errStatPlus") )
   {
      estat[cnt] += atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= nbins ) break;
   }
   
   // now sys err
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.errSysMinus") )
   {
      esys[cnt] = atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= nbins ) break;
   }
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.errSysPlus") )
   {
      esys[cnt] += atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= nbins ) break;
   }

   double* etot = new double[ nbins ];
   
   for ( int i=0; i<nbins; ++i )
   {
      etot[i] = sqrt( estat[i]*estat[i] + esys[i]*esys[i] );
   }
   
   fGraph = new TGraphErrors( nbins, xx, cv, 0, etot );
   
   std::string gtitle = pt.get<std::string>( "datatable.title" );
   fGraph->SetTitle( gtitle.c_str() );
   
   std::vector<std::string> axtitle;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.axisTitle" ) )
   {
      axtitle.push_back( v.second.data() );
   }
   fGraph->GetXaxis()->SetTitle( axtitle[0].c_str() );
   fGraph->GetYaxis()->SetTitle( axtitle[1].c_str() );
   fGraph->SetName( "GrExpData" );
   
   delete [] cv;
   delete [] estat;
   delete [] esys;
   delete [] etot;
   delete [] xx;

   return fGraph;   

}
