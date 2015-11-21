

// --> #include <iostream>

#include "artg4tk/DataProducts/G4ModelConfig/ArtG4tkModelConfig.hh"

#include "fhiclcpp/ParameterSet.h"

#include <assert.h>

void artg4tk::ArtG4tkModelConfig::Fill( const fhicl::ParameterSet& pset )
{

   // NOTE(JVY): To an extend, this kind of replicate the code in PhysModelConfig service.
   //              However, opted for this way because otherwise one would need to introduce
   //              MULTIPLE RunProducts (configs) in service which would be quite a complication !!!
   //

   std::vector<std::string> keys = pset.get_keys();
   std::vector<std::string> modkeys;
   for ( unsigned int i=0; i<keys.size(); ++i )
   {      
      if ( keys[i] == "DefaultPhysics" )
      {
         fDefaultPhysics = pset.get<bool>("DefaultPhysics",true);
	 continue;
      }
      if ( fDefaultPhysics ) return;
      fhicl::ParameterSet mpset = pset.get<fhicl::ParameterSet>(keys[i]);
      modkeys.clear();
      modkeys = mpset.get_keys();
      for ( unsigned i1=0; i1<modkeys.size(); ++i1 )
      {
         // FIXME !!!
	 // Treat everything as doubles for now
	 // Doubles can be converted to int(s), and those map fine vs bool(s)
	 //
	 double value = mpset.get<double>(modkeys[i1]);
	 Insert( keys[i], modkeys[i1], value );
      }
   }
   
   return;
   
}


void artg4tk::ArtG4tkModelConfig::Insert( const std::string& model, const std::string& param, const double& value )
{

   // Check if "Default Physics" (no changes to G4's defaults whatsoever)
   // and store config(s) only if NOT
   //
   //
   if ( fDefaultPhysics ) return; 
   
   if ( fModelConfig.find(model) == fModelConfig.end() )
   {
      fModelConfig.insert( std::pair< std::string, std::vector< std::pair<std::string,double> > >( model,
                                                                                                   std::vector< std::pair<std::string,double> >() ) );
   }
   
   std::map< std::string, std::vector< std::pair<std::string,double> > >::iterator itr=fModelConfig.find(model);
   
   (itr->second).push_back( std::pair<std::string,double>(param,value) );

   return;

}

std::vector<std::string> artg4tk::ArtG4tkModelConfig::GetConfiguredModels() const
{

   std::vector<std::string> models;

   std::map< std::string, std::vector< std::pair<std::string,double> > >::const_iterator itr=fModelConfig.begin();
   for ( ; itr!=fModelConfig.end(); ++itr )
   {
      models.push_back( itr->first );
   }
   
   return models;

}

const std::vector< std::pair<std::string,double> >* artg4tk::ArtG4tkModelConfig::GetConfig( const std::string& model ) const
{

   std::map< std::string, std::vector< std::pair<std::string,double> > >::const_iterator itr=fModelConfig.find(model);
   
   if ( itr == fModelConfig.end() )
   {
      // FIXME !! 
      // Needs to give a WARNING !!!
      //
      return NULL;
   }   

   return &(itr->second);
   
}
