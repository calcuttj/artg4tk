




#include <iostream>
#include "artg4tk/ExpDataAccess/JSON2Data.hh"
#include "boost/foreach.hpp"
// --> no need as it comes via parser --> #include <boost/property_tree/ptree.hpp>
#include "boost/property_tree/json_parser.hpp"

#include "TAxis.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

MetaData::MetaData()
   : /* fBeamPID(0), fBeamMomentum(0.), */ fBeamLink(-1), fTargetNucleus(0), fSecondaryPID(0),
                                           fObservable(-1),
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
     fObservable(rhs.fObservable),
     fTitle(rhs.fTitle),
     fRefLink(rhs.fRefLink),
     fParNames(rhs.fParNames),
     fParValues(rhs.fParValues)
{
}

MetaData::~MetaData()
{
}

bool MetaData::operator==( const MetaData& rhs ) const
{
   
   if ( /* fBeamPID==rhs.fBeamPID && (fBeamMomentum-rhs.fBeamMomentum)<=1.e-10 && */
        fBeamLink==rhs.fBeamLink &&
        fTargetNucleus==rhs.fTargetNucleus && fSecondaryPID==rhs.fSecondaryPID && fObservable==rhs.fObservable )
   {
      return true;
   }

   return false;
   
}

bool MetaData::IsMatch( const int& beamid, const double& mom,
                        const int& tgt,    const int& secid,   const int& observable ) const
{

   if ( IsBeamTargetMatch( beamid, mom, tgt ) &&
	secid == fSecondaryPID && observable == fObservable )
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
      else if ( fabs(mom-100.) <= 1.e-10  && fRefLink == 56 )
      {
         blnk = 12;
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
      // IAEA/Ishibashi (there should also be mom's corresponding 
      // to Ekin=0.8GeV and EKin=1.5GeV
      else if ( fabs(mom-3.824) <= 1.e-10 && fRefLink == 68 )
      {
         blnk = 107;
      }
   }
   else if (pid == 211 )
   {
      if ( fabs(mom-100.) <= 1.e-10  && fRefLink == 56 )
      {
         blnk = 10;
      }
      else if ( fabs(mom-3.) <= 1.e-10 )
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
   else if ( pid == 321 )
   {
      if ( fabs(mom-100.) <= 1.e-10  && fRefLink == 56 )
      {
         blnk = 11;
      }
   }
   
// --->   std::cout << " beam = " << pid << " momentum = " << mom << " beamlink = " << blnk << std::endl;
   
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
   fMetaData.fObservable = -1;
   fMetaData.fTitle.clear();
   fMetaData.fTitle = "";
   fMetaData.fRefLink = -1;
   fMetaData.fParNames.clear();
   fMetaData.fParValues.clear();

   return;

}

bool JSON2Data::BuildDictionaries( const std::string& type, const std::string& jstr )
{

   if ( type == "Particle" )   return BuildParticlesDict( jstr );
   if ( type == "Material" )   return BuildTargetsDict( jstr );
   if ( type == "Beam" )       return BuildBeamsDict( jstr );
   if ( type == "Observable" ) return BuildObservablesDict( jstr );
   if ( type == "Reference" )  return BuildReferencesDict( jstr );
   if ( type == "Datatypes" )  return BuildDataTypesDict( jstr );
   
   return false;

}

void JSON2Data::ParseMetaData( const std::string& jstr )
{

   assert( !fParticles.empty() );
   assert( !fTargets.empty() );
   assert( !fBeams.empty() ); 
   assert( !fObservables.empty() );
   assert( !fReferences.empty() );
   assert( !fDataTypes.empty() );
   
   ClearMetaData();
   
   std::stringstream ss(jstr.c_str()); 
   
   ptree pt;
   read_json( ss, pt );
   
   std::string kw = "";
   std::map<std::string,int>::iterator itr;
   kw = pt.get<std::string>("beamkw");
   itr = fBeams.find(kw);
   if ( itr != fBeams.end() )
   {
      fMetaData.fBeamLink = itr->second;
   }
   kw = pt.get<std::string>("targetkw");
   itr = fTargets.find(kw);
   if ( itr != fTargets.end() )
   {  
      fMetaData.fTargetNucleus = itr->second; 
   }
   kw = pt.get<std::string>("secondarykw");
   itr = fParticles.find(kw);
   if ( itr != fParticles.end() )
   {
      fMetaData.fSecondaryPID = itr->second;
   }
   kw = pt.get<std::string>("observablekw");
   itr = fObservables.find(kw);
   if ( itr != fObservables.end() )
   {
      fMetaData.fObservable = itr->second;
   }
/*
   fMetaData.fBeamLink      = pt.get<int>("beamlnk");
   fMetaData.fTargetNucleus = pt.get<int>("targetlnk");
   fMetaData.fSecondaryPID  = pt.get<int>("secondarylnk");
   fMetaData.fObservable    = pt.get<int>("observablelnk");
*/
   
   fMetaData.fTitle         = pt.get<std::string>("datatable.title");
   
   kw = pt.get<std::string>("referencekw");
   int inspireid = std::atoi( kw.c_str() );
   if ( fReferences.find(inspireid) != fReferences.end() )
   {
      fMetaData.fRefLink = (fReferences.find(inspireid))->second;
   }
/*
   fMetaData.fRefLink = pt.get<int>("referencelnk");
*/   
   BOOST_FOREACH( ptree::value_type &v, pt.get_child("parnames.") )
   {
      fMetaData.fParNames.push_back( v.second.data() );
      
   }
   BOOST_FOREACH( ptree::value_type &v, pt.get_child("parvalues.") )
   {
      fMetaData.fParValues.push_back( v.second.data() );
      
   }
   
   return;

}

bool JSON2Data::BuildParticlesDict( const std::string& jstr )
{

    std::stringstream ss(jstr.c_str()); 

    ptree pt;
    read_json( ss, pt );
        
    BOOST_FOREACH( ptree::value_type &v, pt )
    {
       int pdgid = (v.second).get<int>("pdgid");
       std::string pname = (v.second).get<std::string>("pname");
       fParticles.insert( std::pair<std::string,int>(pname,pdgid) );
    } 
      
   return ( !fParticles.empty() );

}

bool JSON2Data::BuildTargetsDict( const std::string& jstr )
{

    std::stringstream ss(jstr.c_str()); 

    ptree pt;
    read_json( ss, pt );
        
    BOOST_FOREACH( ptree::value_type &v, pt )
    {
       int z = (v.second).get<int>("z");
       std::string mname = (v.second).get<std::string>("mname");
       fTargets.insert( std::pair<std::string,int>(mname,z) );
    } 
   
   return ( !fTargets.empty() );

}

bool JSON2Data::BuildBeamsDict( const std::string& jstr )
{

    std::stringstream ss(jstr.c_str()); 

    ptree pt;
    read_json( ss, pt );
        
    BOOST_FOREACH( ptree::value_type &v, pt )
    {
       int blnk = (v.second).get<int>("bid");
       std::string bname = (v.second).get<std::string>("Bname");
       fBeams.insert( std::pair<std::string,int>(bname,blnk) );
    } 
      
   return ( !fBeams.empty() );

}

bool JSON2Data::BuildObservablesDict( const std::string& jstr )
{

    std::stringstream ss(jstr.c_str()); 

    ptree pt;
    read_json( ss, pt );
        
    BOOST_FOREACH( ptree::value_type &v, pt )
    {
       int olnk = (v.second).get<int>("oid");
       std::string oname = (v.second).get<std::string>("oname");
       fObservables.insert( std::pair<std::string,int>(oname,olnk) );
    } 
      
   return ( !fObservables.empty() );

}

bool JSON2Data::BuildReferencesDict( const std::string& jstr )
{

    std::stringstream ss(jstr.c_str()); 

    ptree pt;
    read_json( ss, pt );
        
    BOOST_FOREACH( ptree::value_type &v, pt )
    {
       int olnk      = (v.second).get<int>("refid");
       int inspireid = (v.second).get<int>("inspireid");
       fReferences.insert( std::pair<int,int>(inspireid,olnk) );
    } 
      
   return ( !fReferences.empty() );

}

bool JSON2Data::BuildDataTypesDict( const std::string& jstr )
{

    std::stringstream ss(jstr.c_str()); 

    ptree pt;
    read_json( ss, pt );
        
    BOOST_FOREACH( ptree::value_type &v, pt )
    {
       int dtlnk      = (v.second).get<int>("dtype");
       std::string desc = (v.second).get<std::string>("description");
       fDataTypes.insert( std::pair<std::string,int>(desc,dtlnk) );
    } 
      
   return ( !fDataTypes.empty() );

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
//   int dtype = pt.get<int>("datatable.datatypeslnk");
   std::string dtypekw = pt.get<std::string>("datatable.datatypeskw");
   std::map<std::string,int>::iterator itr = fDataTypes.find( dtypekw );
   if ( itr == fDataTypes.end() )
   {
      // FIXME !!! maybe it should also give a warning ?...
      return NULL;
   }
   
   int dtype = itr->second;

//   if ( dtype == "1000" || dtype == "1001" )
   if ( dtype >= 1000 )
   {
     // 1D Datapoint set - can NOT convert to TH1D directly since the binning is unknowns
     // Thus we convert via Graph... which is a bit of cheating but not the end of the world
     // At least, we'll go like this until we get a better idea and/or time to refine the infrastructure
     //
     mf::LogInfo log;
     log << " 1D Datapoints set can be converted into TH1D via TGraphErrors" << std::endl;
     // ---> return NULL;
     fGraph = Convert2Graph( jstr, hname );
     ConvertGraph2Histo( hname );
     return fHisto;
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

TGraphErrors* JSON2Data::Convert2Graph( const std::string& jstr, const char* grname )
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
   // int dtype = pt.get<int>("datatable.datatypeslnk");
   std::string dtypekw = pt.get<std::string>("datatable.datatypeskw");
   std::map<std::string,int>::iterator itr = fDataTypes.find( dtypekw );
   if ( itr == fDataTypes.end() )
   {
      // FIXME !!! maybe it should also give a warning ?...
      return NULL;
   }
   
   int dtype = itr->second;
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
      if ( cnt < nbins )
      {
         xx[cnt] = atof( v.second.data().c_str() );
      }
      else
      {
         cv[cnt-nbins] = atof( v.second.data().c_str() );
      }
      cnt++;
      if ( cnt >= 2*nbins ) break; 
   }
   
   // now get errors
   //
   // stat errors first
   //
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.errStatMinus") )
   {
      if ( cnt < nbins ) 
      {
         cnt++;
	 continue;
      }
      estat[cnt-nbins] = atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= 2*nbins ) break;
   }
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.errStatPlus") )
   {
      if ( cnt < nbins )
      {
         cnt++;
	 continue;
      }
      estat[cnt-nbins] += atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= 2*nbins ) break;
   }
   
   // now sys err
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.errSysMinus") )
   {
      if ( cnt < nbins )
      {
         cnt++;
	 continue;
      } 
      esys[cnt-nbins] = atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= 2*nbins ) break;
   }
   cnt = 0;
   BOOST_FOREACH( ptree::value_type& v, pt.get_child("datatable.errSysPlus") )
   {
      if ( cnt < nbins )
      {
         cnt++;
	 continue;
      }
      esys[cnt-nbins] += atof( v.second.data().c_str() );
      cnt++;
      if ( cnt >= 2*nbins ) break;
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
   std::string name = "GrExpData" + std::string( grname );
   fGraph->SetName( name.c_str() );
   
   delete [] cv;
   delete [] estat;
   delete [] esys;
   delete [] etot;
   delete [] xx;

   return fGraph;   

}

void JSON2Data::ConvertGraph2Histo( const char* hname )
{

   assert(fGraph);
   
   if ( fHisto ) delete fHisto;
   
   int     NPt = fGraph->GetN();
   double* XX  = fGraph->GetX();
   double* YY  = fGraph->GetY();
   double* EYY = fGraph->GetEY();
   
   double* xbins = new double[NPt+1];
   
   double delta = (XX[1]-XX[0])/2.;
   xbins[0] = XX[0] - delta;
   xbins[1] = XX[0] + delta;
   for ( int i=1; i<NPt-2; ++i )
   {
      xbins[i+1] = XX[i] + delta;
      double delta_next = (XX[i+1]-XX[i])/2.;
      if ( fabs(delta_next-delta) > 1.e-10 )
      {
	 delta_next = (XX[i+2]-XX[i+1])/2.;
	 assert( fabs((XX[i+1]-delta_next)-xbins[i+1]) < 1.e-10 );
      }
      delta = delta_next;
   }
   xbins[NPt-1] = XX[NPt-2] + delta;
   // delta = XX[NPt-1] - xbins[NPt-1];
   // xbins[NPt] = XX[NPt-1] + delta;
   xbins[NPt] = 2.*XX[NPt-1] - xbins[NPt-1];
   
   fHisto = new TH1D( hname, fGraph->GetTitle(), NPt, xbins );
   
   for ( int i=0; i<NPt; ++i )
   {
      int ibin = fHisto->FindBin( XX[i] );
      if ( ibin > 0 ) 
      {
	 fHisto->SetBinContent( ibin, YY[i] );
         fHisto->SetBinError( ibin, EYY[i] );	 
      }
   } 
   
   return;

}
