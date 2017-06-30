
// Authors: Julia Yarba

// Date: Oct. 2015

// Art includes
//
#include "art/Framework/Core/ModuleMacros.h"

// Services
#include "art/Framework/Services/Registry/ServiceHandle.h"

#include "artg4tk/G4PhysModelParamStudy/G4Services/PhysModelConfig_service.hh"
#include "artg4tk/G4PhysModelParamStudy/G4Components/ModelConfigMapper.hh"


artg4tk::PhysModelConfigService::PhysModelConfigService( const fhicl::ParameterSet& pset, 
				                         art::ActivityRegistry& )
{

   fConfigMapper = new ModelConfigMapper();
   fVerbosity = pset.get<bool>("Verbosity",false);
   if ( fVerbosity )
   {
      // FIXME !!!
      // Need to find out a more "automated" way to handle it...
      //
      fConfigMapper->PrintDefaults("bertini");
// --> too much output this way; better do only if model's verbosty is requested fConfigMapper->SetVerbosity("bertini",fVerbosity);
      // ---> for future --> fConfigMapper->PrintDefaults("inclxx");
   }
          
}

artg4tk::PhysModelConfigService::~PhysModelConfigService()
{

   delete fConfigMapper;
 
} 

void artg4tk::PhysModelConfigService::PrintDefaultSettings()
{

   fConfigMapper->PrintDefaults("bertini");
   return;

}

void artg4tk::PhysModelConfigService::ConfigureModel( const std::string& mname, 
                                                      const fhicl::ParameterSet& mpset )
{

   // NOTE (JVY): This logic assumes that the model's verbosity key with within the model config.
   //             This is the case when using a physics list where multiple models can be configured.
   //             But NOT the case when running at the "process level" where the verbosity key is
   //             OUTSIDE of the model configuration specs and rather applies to the whole module.
   //             FIXME !!!
   //             Need to find a transparent solution !!!
   //             Using the verbosity key at the level of CENTRAL config service will work but it'll
   //             make the models (or at least Bertini) way to noisy. 
   
   std::string mod = mname;
   mod = fConfigMapper->ToLower(mod);
   if ( mod == "bertini" )
   {
      // this is a "hack" and kind of "violation" of the PSet philosophy but...
      //
      std::vector<std::string> keys = mpset.get_names(); // no loger: get_keys()
      for ( unsigned int i=0; i<keys.size(); ++i )
      {
          std::string thekey = keys[i];
	  thekey = fConfigMapper->ToLower(thekey);
/*
	  if ( thekey.find("use") != std::string::npos ) continue; // those are int or bool !
	                                                           // will add/treat them later
*/
	  // NOTE(JVY): there's a difference between "general" verbosity and
	  //            specific Bertini verbosity (which is "too verbose");
	  //            hence, the 2 different keys...
	  //
	  if ( thekey == "verbosity" )
	  {
	     fConfigMapper->SetVerbosity( "bertini", mpset.get<bool>(keys[i],false) );
	     continue;
	  }	
	  //
	  // Bertini itself treats parameters that contain "use" as integers (not bools);
	  // they can be changed only "by value"
	  //	  
	  if ( thekey.find("use") != std::string::npos )
	  {
	     int value = mpset.get<int>(keys[i],0) ;
	     fConfigMapper->ChangeParameter( "bertini", thekey, value, fVerbosity );
	  }
	  // 
	  // Parameters that do NOT contain "use" can treated as doubles (doesn't matter
	  // what Bertini "thinks" internally),cand can be changed "by value" or "by ratio"
	  //
	  double value = mpset.get<double>(keys[i]);
	  if ( thekey.find("byratio") != std::string::npos )
	  {
	     //  change by ratio
	     fConfigMapper->ChangeParameterByRatio( "bertini", thekey, value, fVerbosity );
	  }
	  else
	  {
	    // change by value
	    fConfigMapper->ChangeParameter( "bertini", thekey, value, fVerbosity );
	  }
      }
   }

   return;

}

void artg4tk::PhysModelConfigService::Reset()
{

   fConfigMapper->RestoreAllDefaults();
   return;

}

void artg4tk::PhysModelConfigService::PrintCurrentSettings()
{

   fConfigMapper->PrintCurrentSettings();
   return;

}

using artg4tk::PhysModelConfigService;

DEFINE_ART_SERVICE(PhysModelConfigService)

