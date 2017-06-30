#include "artg4tk/G4PhysModelParamStudy/G4Components/ModelConfigMapper.hh"

// Geant4
#include "Geant4/G4PhysicalConstants.hh"
#include "Geant4/G4SystemOfUnits.hh"
//
// Bertini
#include "Geant4/G4CascadeParameters.hh"
// INCLXX Parameters ???
// Preco
#include "Geant4/G4NuclearLevelData.hh"
#include "Geant4/G4DeexPrecoParameters.hh"
//
#include "Geant4/G4RunManager.hh"
#include "Geant4/G4UImanager.hh"
#include "Geant4/G4StateManager.hh"

// system/general
#include <iostream>
#include <cctype>

// Bertini NOTES: coming later...

// INCLXX NOTE: coming later...

// PreCompound NOTES:
//
// Source code:
// /geant4/source/processes/hadronic/models/de_excitation/management
// /geant4/source/processes/hadronic/models/pre_equilibrium/exciton_model
//
// G4DeexPrecoParameters - is NOT a singleton but...
//                         ...it's a part of G4NuclearLevelData which IS a singleton
//
// Vladimir I. said at the G4 Wrkshop-2016 that "the Set methods are only active
// if the master thread is in the G4State_PreInit state", although I have't noticed
//
// Environment variable G4LEVELGAMMADATA must be set 
//
// All PreCo parameters must be in BEFORE BuildPhysicsTable() is called
// Technically, this happens later than the ctor is invoked (e.g. via physics list) 
// but it is still part of run initialization
//

ModelConfigMapper::ModelConfigMapper()
{

   // models that have G4UI as of geant4.10.3-series
   //
   fNameConvention.insert( std::pair<std::string,std::string>("bertini",    "cascade") );
   fNameConvention.insert( std::pair<std::string,std::string>("inclxx",     "inclxx" ) );
   fBaseCommand = "/process/had/";
   //
   // model(s) that do not have G4UI but a C++ interface instead
   //
   fNameConvention.insert( std::pair<std::string,std::string>("precompound","precompound" ) );
   
   FillBertiniDefaults();
   FillINCLXXDefaults();
   FillPreCompoundDefaults();
   
   FillConfigParamMapBertini();

}

ModelConfigMapper::~ModelConfigMapper()
{

   // Need to clean up here !

}

void ModelConfigMapper::PrintDefaults( const std::string& model ) // const
{
     
   std::string mod = model;
   mod = ToLower(mod);
   
   std::map< std::string, std::map<std::string,std::string> >::iterator itr=fDEFAULTS.find(mod);
   
   if ( itr == fDEFAULTS.end() )
   {
      G4cout << " Model " << model << "(" << mod <<") NOT found " << G4endl;
      return;   
   }

   std::map<std::string,std::string>::iterator itr1;
   
   std::map< std::string, std::map<std::string,std::string> >::iterator itr2=fConfigParameters.find(mod);
   std::map<std::string,std::string>::iterator itr3; 
   
   
   itr1 = fNameConvention.find(mod);
   if ( itr1 == fNameConvention.end() )
   {
      G4cout << " Can NOT find Geant4 internal name for model " << model << "(" << mod <<") " << G4endl;
      return;   
   }
   
   std::string command = "";
   if ( model == "bertini" || model == "inclxx" )
   {
      command += fBaseCommand; 
   }
   command += itr1->second;
   
   G4cout << " ***** Default settings for Geant4 model " << model << G4endl;
   
   itr1 = (itr->second).begin();
   for ( ; itr1!=(itr->second).end(); ++itr1 )
   {
      // --> G4cout << " * " << command << itr1->first << " " << itr1->second << G4endl; 
      itr3 = (itr2->second).find(itr1->first);
      if ( itr3 == (itr2->second).end() )
      {
         G4cout << " Can NOT find command for parameter " << itr1->first << G4endl;
      }
      else
      {
         G4cout << " * " << command << itr3->second << " " << itr1->second << G4endl; 
      }
   }
   
   G4cout << " ***** End of default settings for Geant4 model " << model << G4endl;
   
   return;

}

void ModelConfigMapper::PrintCurrentSettings()
{

   PrintBertiniSettings();
   PrintINCLXXSettings();
   PrintPreCompoundSettings();
   
   // etc. in the future...
   
   return;
   
}

void ModelConfigMapper::SetVerbosity( const std::string& model, const bool& verb )
{
         
   double value = 0;
   if ( verb ) value = 1;
   ChangeParameter( model, "verbosity", value, verb ); 
   
   return;

}

void ModelConfigMapper::ChangeParameter( const std::string& model, const std::string& param, const double& value, bool verb )
{
   
   // First of all, make sure physics list is NOT setup to the RunManager
   //
   if ( G4RunManager::GetRunManager() ) // check if it exists at all
   {
      if ( G4RunManager::GetRunManager()->GetUserPhysicsList() )
      {
         // bail out with a warning
	 // it's USELESS to change anything after a physics list is set to the run manager
	 // because everything happens at the stage of physics list init (models ctor's)
	 G4cout << " Physics list is already assigned to Run Manager " << G4endl;
	 G4cout << " It is useless to change any model parameters since the changes will not propagate " << G4endl;
	 return;
      }
   
   }
   
   // FIXME !!!
   // Technically speaking, one needs to check if a(ny) physics list is already created
   // because one may create a physics list and assign it to the run manager later...
   // but in such case changing parameters wont make any sense either because they'll
   // NOT propagate to the guts of the model
   // Although we might want to check if it's the physics list ctor or its init...
   

   std::string mod = model;
   mod = ToLower(mod);

   if ( mod == "bertini" || mod == "inclcxx" )
   {
      ChangeParameterViaG4UI( model, param, value, verb );
   }
   else if ( mod == "precompound" )
   {
      ChangeParameterPreCo( param, value, verb );
   }
   else if ( mod.find( "ftf" ) != std::string::npos )
   {
   }
   
   return;

}

void ModelConfigMapper::ChangeParameter( const std::string& model, const std::string& param, const float& value, bool verb )
{

   double v1 = value;
   ChangeParameter( model, param, v1, verb );
   
   return;

}

void ModelConfigMapper::ChangeParameter( const std::string& model, const std::string& param, const int& value, bool verb )
{

   double v1 = (double)value;
   ChangeParameter( model, param, v1, verb );
   return;

}

void ModelConfigMapper::ChangeParameterByRatio( const std::string& model, const std::string& param, const double& ratio, bool verb )
{

   std::string mod = model;
   mod = ToLower(mod);   
   
   std::map< std::string, std::map<std::string,std::string> >::iterator itr = fConfigParameters.find(mod);
   if ( itr == fConfigParameters.end() )
   {
      // bail out with a warning
      //
      G4cout << " Model " << model << "(" << mod <<") NOT found " << G4endl;
      return;
   }

   std::string par = param;
   par = ToLower(par);
   size_t found = par.find("use");
   
   if ( found != std::string::npos )
   {
      G4cout << " Parameter " << param << " is a switch, it can NOT be changed by ratio" << G4endl;
      return;
   }
   

   found = par.find("byratio");
   if ( found != std::string::npos )
   {
      // remove "byratio" part from the parameter's name/key
      par.erase( found ); // will go to the end of the string since the D for len=std::string::npos 
   }
   
   std::map<std::string,std::string>::iterator itr1 = (itr->second).find(par);
   
   if ( itr1 == (itr->second).end() )
   {
      // bail out with a warning
      //
      G4cout << " Parameter " << param << "(" << par << ") for model " << model << " NOT found " << G4endl;
      return;
   }
   
   std::map< std::string, std::map<std::string,std::string> >::iterator itr2=fDEFAULTS.find(mod);
   if ( itr2 == fDEFAULTS.end() )
   {
      G4cout << " Can NOT find defaults for model " << model << "(" << mod <<")" << G4endl;
      return;
   }
   
   
   std::map<std::string,std::string>::iterator itr3=(itr2->second).find(itr1->first);
   
   if ( itr3 == (itr2->second).end() )
   {
      std::cout << " can NOT find parameter " << itr1->second << " in the list of defaults" << std::endl;
   }
      
   double dvalue = std::atof( (itr3->second).c_str() ); 
   double newvalue = dvalue*ratio;
   
   if ( verb )
   {
      G4cout << " Changing " << model << " parameter " << param << " (" << par << ")" 
             << " by Ratio=" << ratio << ", from Default=" << dvalue << G4endl;
   }
      
   ChangeParameter( model, par, newvalue, verb );
   
   return;

}

void ModelConfigMapper::ChangeParameterByRatio( const std::string& model, const std::string& param, const float& ratio, bool verb )
{

   double v1 = ratio;
   ChangeParameterByRatio( model, param, v1, verb );
   return;

}

void ModelConfigMapper::RestoreAllDefaults()
{

   std::map< std::string, std::map<std::string,std::string> >::iterator itr=fDEFAULTS.begin();
   for ( ; itr!=fDEFAULTS.end(); ++itr )
   {
      RestoreDefaults( itr->first );
   }
      
   return;

}

void ModelConfigMapper::RestoreDefaults( const std::string& model ) 
{
      
   std::string mod = model;
   mod = ToLower(mod);

   std::map< std::string, std::map<std::string,std::string> >::iterator itr=fDEFAULTS.find(mod);
   
   if ( itr == fDEFAULTS.end() )
   {
      G4cout << " Model " << model << "(" << mod <<") NOT found " << G4endl;
      return;   
   }
   
   std::map<std::string,std::string>::iterator itr1 = (itr->second).begin();
   for ( ; itr1!=(itr->second).end(); ++itr1 )
   {
      ChangeParameter( itr->first, itr1->first, std::stod((itr1->second).c_str()), false );
   }
   
   return;

}

std::string ModelConfigMapper::ToLower( std::string input )
{

  for( unsigned int i=0; i<input.size(); ++i ) input[i] = std::tolower(input[i]);
  return input;

}

void ModelConfigMapper::FillBertiniDefaults()
{
   
   fDEFAULTS.insert( std::pair< std::string, std::map<std::string,std::string> >( "bertini", 
                                                                                  std::map<std::string,std::string>() ) );

   std::map< std::string, std::map<std::string,std::string> >::iterator itr2=fDEFAULTS.find("bertini");

   std::ostringstream cmd;

   // general purpose verbosity switch
   // has nothing to do with the model params/configuration
   // but is useful for some debugging purposes
   //
   cmd << G4CascadeParameters::verbose();
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/verbose",cmd.str()) ); 
   (itr2->second).insert( std::pair<std::string,std::string>("verbosity",cmd.str()) ); 
   cmd.str( "" );
   cmd.clear();
   
   cmd << G4CascadeParameters::doCoalescence();
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/doCoalescence",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("docoalescence",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

// these params/methods are NOT available in G4.9.6-series, but only starting 10.1-ref03
//   
   cmd << G4CascadeParameters::piNAbsorption();
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/piNAbsorption",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("pinabsorption",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   cmd << G4CascadeParameters::use3BodyMom();
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/use3BodyMom",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("use3bodymom",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   cmd << G4CascadeParameters::usePhaseSpace();
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/usePhaseSpace",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("usephasespace",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

// technically speaking, these parameters are available in 9.6-series, together with their G4UI,  
// but in practice in 9.6 they can only be changed via env.variables, due to some implementation details 
//
   cmd << G4CascadeParameters::useTwoParam();
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/useTwoParamNuclearRadius",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("use2paramnucradius",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   cmd << G4CascadeParameters::radiusScale();
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/nuclearRadiusScale",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("radiusscale",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   cmd << (G4CascadeParameters::radiusSmall()/G4CascadeParameters::radiusScale()); // due to specifics of Bertini implementation
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/smallNucleusRadius",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("smallnucradius",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   cmd << G4CascadeParameters::radiusAlpha();
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/alphaRadiusScale",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("alpharadiusscale",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
//   cmd << G4CascadeParameters::radiusTrailing(); 
   cmd << (G4CascadeParameters::radiusTrailing()/G4CascadeParameters::radiusScale()); 
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/shadowningRadius",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("trailingradius",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   cmd << (G4CascadeParameters::fermiScale()/G4CascadeParameters::radiusScale()); // due to specifics of Bertini implementation
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/fermiScale",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("fermiscale",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   cmd << G4CascadeParameters::xsecScale();
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/crossSectionScale",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("xsecscale",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   cmd << G4CascadeParameters::gammaQDScale();
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/gammaQuasiDeutScale",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("gammaqdscale",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   cmd << G4CascadeParameters::dpMaxDoublet();
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/cluster2DPmax",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("cluster2dpmax",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   cmd << G4CascadeParameters::dpMaxTriplet();
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/cluster3DPmax",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("cluster3dpmax",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   cmd << G4CascadeParameters::dpMaxAlpha();
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/cluster4DPmax",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("cluster4dpmax",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   cmd << G4CascadeParameters::usePreCompound() ; // false/0 by default
   // --> (itr2->second).insert( std::pair<std::string,std::string>("/usePreCompound",cmd.str()) );
   (itr2->second).insert( std::pair<std::string,std::string>("useprecompound",cmd.str()) );
   cmd.str("");
   cmd.clear();

// !!!   (itr2->second).insert( std::pair<std::string,std::string>("/useBestNuclearModel","false") ); // no corresponding access method; unclear D !!!
                                                                                                // From G4CascadeParameters.cc: BEST_PAR = (0!=G4NUCMODEL_USE_BEST);
				                                                                // probably means that if env.var. is NOT set, this option isn't in use

   return;

}

void ModelConfigMapper::FillINCLXXDefaults()
{

   return;

}

void ModelConfigMapper::FillPreCompoundDefaults()
{
 
   G4DeexPrecoParameters* precoparams = G4NuclearLevelData::GetInstance()->GetParameters();
   precoparams->SetDefaults();

   fDEFAULTS.insert( std::pair< std::string, std::map<std::string,std::string> >( "precompound", 
                                                                                  std::map<std::string,std::string>() ) );

   std::map< std::string, std::map<std::string,std::string> >::iterator itr2=fDEFAULTS.find("precompound");
   
   std::ostringstream cmd;

   cmd << precoparams->GetLevelDensity()*CLHEP::MeV; 
   (itr2->second).insert( std::pair<std::string,std::string>("LevelDensity",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->GetR0()/CLHEP::fermi;
   (itr2->second).insert( std::pair<std::string,std::string>("R0",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->GetTransitionsR0()/CLHEP::fermi;
   (itr2->second).insert( std::pair<std::string,std::string>("TransitionsR0",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   
   cmd << precoparams->GetFermiEnergy()/CLHEP::MeV;
   (itr2->second).insert( std::pair<std::string,std::string>("FermiEnergy",cmd.str()) );
   cmd.str( "" );
   cmd.clear();
   
   cmd << precoparams->GetPrecoLowEnergy()/CLHEP::MeV;
   (itr2->second).insert( std::pair<std::string,std::string>("PrecoLowEnergy",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->GetPhenoFactor();
   (itr2->second).insert( std::pair<std::string,std::string>("PhenoFactor",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->GetMinExcitation()/CLHEP::eV;
   (itr2->second).insert( std::pair<std::string,std::string>("MinExcitation",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->GetMaxLifeTime()/CLHEP::microsecond;
   (itr2->second).insert( std::pair<std::string,std::string>("MaxLifeTime",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   // cmd << precoparams->GetMinExPerNucleonForMF()/CLHEP::GeV;
   cmd << precoparams->GetMinExPerNucleounForMF()/CLHEP::GeV;
   (itr2->second).insert( std::pair<std::string,std::string>("MinExPerNucleonForMF",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->GetMinZForPreco();
   (itr2->second).insert( std::pair<std::string,std::string>("MinZForPreco",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->GetMinAForPreco();
   (itr2->second).insert( std::pair<std::string,std::string>("MinAForPreco",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->GetPrecoModelType();
   (itr2->second).insert( std::pair<std::string,std::string>("PrecoModelType",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->GetDeexModelType();
   (itr2->second).insert( std::pair<std::string,std::string>("DeexModelType",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->NeverGoBack();
   (itr2->second).insert( std::pair<std::string,std::string>("NeverGoBack",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->UseSoftCutoff();
   (itr2->second).insert( std::pair<std::string,std::string>("UseSoftCutoff",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->UseCEM();
   (itr2->second).insert( std::pair<std::string,std::string>("UseCEM",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->UseGNASH();
   (itr2->second).insert( std::pair<std::string,std::string>("UseGNASH",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->UseHETC();
   (itr2->second).insert( std::pair<std::string,std::string>("UseHETC",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

   cmd << precoparams->UseAngularGen();
   (itr2->second).insert( std::pair<std::string,std::string>("UseAngularGen",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

// FIXME (JVY) !!! What is this ???
//
//   cmd << precoparams->UsrFilesNEW();
//   (itr2->second).insert( std::pair<std::string,std::string>("UseFilesNEW",cmd.str()) );
//   cmd.str( "" );
//   cmd.clear();

   cmd << precoparams->CorrelatedGamma();
   (itr2->second).insert( std::pair<std::string,std::string>("CorrelatedGamma",cmd.str()) );
   cmd.str( "" );
   cmd.clear();

// FIXME (JVY) !!! What is this ???
//
//   cmd << precoparams->StoreAllLevels();
//   (itr2->second).insert( std::pair<std::string,std::string>("StoreAllLevels",cmd.str()) );
//   cmd.str( "" );
//   cmd.clear();

// FIXME/NOTE (JVY) !!!
// Need to see how to (better) handle this case...
//
// There's also a method G4DeexChannelType GetDeexChannelsType() const
// The G4DeexChannelType is enum:
// enum G4DeexChannelType
// {
//   fEvaporation = 0,
//   fGEM,
//   fCombined
// };
//
// The current setting is "fEvaporation" 
// 

   return;

}

void ModelConfigMapper::FillConfigParamMapBertini()
{

   fConfigParameters.insert( std::pair< std::string, std::map<std::string,std::string> >( "bertini", std::map<std::string,std::string>() ) );
   
   std::map< std::string, std::map<std::string,std::string> >::iterator itr=fConfigParameters.find("bertini");
   
   (itr->second).insert( std::pair<std::string,std::string>("radiusscale","/nuclearRadiusScale") );
   (itr->second).insert( std::pair<std::string,std::string>("xsecscale","/crossSectionScale") );
   (itr->second).insert( std::pair<std::string,std::string>("fermiscale","/fermiScale") );
   (itr->second).insert( std::pair<std::string,std::string>("trailingradius","/shadowningRadius") );
   (itr->second).insert( std::pair<std::string,std::string>("gammaqdscale","/gammaQuasiDeutScale") );
   (itr->second).insert( std::pair<std::string,std::string>("useprecompound", "/usePreCompound") );
   (itr->second).insert( std::pair<std::string,std::string>("docoalescence", "/doCoalescence") );

   (itr->second).insert( std::pair<std::string,std::string>("pinabsorption","/piNAbsorption") );  
   (itr->second).insert( std::pair<std::string,std::string>("use3bodymom","/use3BodyMom") );  
   (itr->second).insert( std::pair<std::string,std::string>("usephasespace","/usePhaseSpace") );  
   (itr->second).insert( std::pair<std::string,std::string>("use2paramnucradius","/useTwoParamNuclearRadius") );  
   (itr->second).insert( std::pair<std::string,std::string>("smallnucradius","/smallNucleusRadius") );  
   (itr->second).insert( std::pair<std::string,std::string>("alpharadiusscale","/alphaRadiusScale") );  
   (itr->second).insert( std::pair<std::string,std::string>("cluster2dpmax","/cluster2DPmax") );  
   (itr->second).insert( std::pair<std::string,std::string>("cluster3dpmax","/cluster3DPmax") );  
   (itr->second).insert( std::pair<std::string,std::string>("cluster4dpmax","/cluster4DPmax") );  

   (itr->second).insert( std::pair<std::string,std::string>("verbosity","/verbose") );  

   return;

}

void ModelConfigMapper::FillConfigParamMapPreCo()
{

   fConfigParameters.insert( std::pair< std::string, std::map<std::string,std::string> >( "precompound", std::map<std::string,std::string>() ) );
   
   std::map< std::string, std::map<std::string,std::string> >::iterator itr=fConfigParameters.find("precompound");

   (itr->second).insert( std::pair<std::string,std::string>("leveldensity","LevelDensity") );
   (itr->second).insert( std::pair<std::string,std::string>("r0","R0") );
   (itr->second).insert( std::pair<std::string,std::string>("transitionsr0","TransitionsR0") );
   (itr->second).insert( std::pair<std::string,std::string>("precolowenergy","PrecoLowEnergy") );
   (itr->second).insert( std::pair<std::string,std::string>("phenofactor","PhenoFactor") );
   (itr->second).insert( std::pair<std::string,std::string>("minexcitation","MinExcitation") );
   (itr->second).insert( std::pair<std::string,std::string>("maxlifetime","MaxLifeTime") );
   (itr->second).insert( std::pair<std::string,std::string>("minexpernucleonformf","MinExPerNucleonForMF") );
   (itr->second).insert( std::pair<std::string,std::string>("minzforpreco","MinZForPreco") );
   (itr->second).insert( std::pair<std::string,std::string>("minaforpreco","MinAForPreco") );
   (itr->second).insert( std::pair<std::string,std::string>("precomodeltype","PrecoModelType") );
   (itr->second).insert( std::pair<std::string,std::string>("deexmodeltype","DeexModelType") );
   (itr->second).insert( std::pair<std::string,std::string>("nevergoback","NeverGoBack") );
   (itr->second).insert( std::pair<std::string,std::string>("usesoftcutoff","UseSoftCutoff") );
   (itr->second).insert( std::pair<std::string,std::string>("usecem","UseCEM") );
   (itr->second).insert( std::pair<std::string,std::string>("usegnash","UseGNASH") );
   (itr->second).insert( std::pair<std::string,std::string>("usehetc","UseHETC") );
   (itr->second).insert( std::pair<std::string,std::string>("useangulargen","UseAngularGen") );
   (itr->second).insert( std::pair<std::string,std::string>("correlatedgamma","CorrelatedGamma") );

   return;

}
void ModelConfigMapper::PrintBertiniSettings()
{

   G4cout << " *** BERTINI CASCADE *** CURRENT SETTINGS ARE THE FOLLOWING: " << G4endl; 
   G4cout << " =========================================================== " << G4endl;
   
   // general purpose verbosity switch
   // has nothing to do with the model params/configuration
   // but is useful for some debugging purposes
   //
   G4cout << fBaseCommand << "cascade/verbosity " <<  G4CascadeParameters::verbose() << G4endl;
   
   G4cout << fBaseCommand << "cascade/doCoalescence " << G4CascadeParameters::doCoalescence() << G4endl;

// these params/methods are NOT available in G4.9.6-series, but only starting 10.1-ref03
//   
   G4cout << fBaseCommand << "cascade/piNAbsorption " << G4CascadeParameters::piNAbsorption() << G4endl;

   G4cout << fBaseCommand << "cascade/use3BodyMom " << G4CascadeParameters::use3BodyMom() << G4endl;

   G4cout << fBaseCommand << "cascade/usePhaseSpace "  << G4CascadeParameters::usePhaseSpace() << G4endl;

// technically speaking, these parameters are available in 9.6-series, together with their G4UI,  
// but in practice they can only be changed via env.variables, due to some implementation details 
//
   G4cout << fBaseCommand << "cascade/useTwoParamNuclearRadius " << G4CascadeParameters::useTwoParam() << G4endl;

   G4cout << fBaseCommand << "cascade/nuclearRadiusScale " << G4CascadeParameters::radiusScale() << G4endl;

   G4cout << fBaseCommand << "cascade/smallNuclearRadius " << (G4CascadeParameters::radiusSmall()/G4CascadeParameters::radiusScale()) 
                          << "*radiusScale(" << G4CascadeParameters::radiusScale() << ") = " << G4CascadeParameters::radiusSmall() << G4endl;
   
   G4cout << fBaseCommand << "cascade/alphaRadiusScale " << G4CascadeParameters::radiusAlpha() << G4endl;
   
   G4cout << fBaseCommand << "cascade/shadowingRadius " << (G4CascadeParameters::radiusTrailing()/G4CascadeParameters::radiusScale()) 
                          << "*radiusScale(" << G4CascadeParameters::radiusScale() << ") = " << G4CascadeParameters::radiusTrailing() << G4endl; 
   
   G4cout << fBaseCommand << "cascade/fermiScale " << (G4CascadeParameters::fermiScale()/G4CascadeParameters::radiusScale())
                          << "*radiusScale(" << G4CascadeParameters::radiusScale() << ") = " << G4CascadeParameters::fermiScale() << G4endl;
   
   G4cout << fBaseCommand << "cascade/crossSectionScale " << G4CascadeParameters::xsecScale() << G4endl;
   
   G4cout << fBaseCommand << "cascade/gammaQuasiDeutScale " << G4CascadeParameters::gammaQDScale() << G4endl;
   
   G4cout << fBaseCommand << "cascade/cluster2DPmax " << G4CascadeParameters::dpMaxDoublet() << G4endl;
   
   G4cout << fBaseCommand << "cascade/cluster3DPmax " << G4CascadeParameters::dpMaxTriplet() << G4endl;
   
   G4cout << fBaseCommand << "cascade/cluster4DPmax " << G4CascadeParameters::dpMaxAlpha() << G4endl;
   
   G4cout << fBaseCommand << "cascade/usePreCompound " << G4CascadeParameters::usePreCompound() << G4endl; // false/0 by default

// --> !!!   (itr2->second).insert( std::pair<std::string,std::string>("/useBestNuclearModel","false") ); // no corresponding access method; unclear D !!!
                                                                                                // From G4CascadeParameters.cc: BEST_PAR = (0!=G4NUCMODEL_USE_BEST);
				                                                                // probably means that if env.var. is NOT set, this option isn't in use
// --> !!!   (itr2->second).insert( std::pair<std::string,std::string>("/useBestNuclearModel","0") ); // no corresponding access method; unclear D !!!

   G4cout << " =========================================================== " << G4endl;
   
   return;

}

void ModelConfigMapper::PrintINCLXXSettings()
{

   return;

}

void ModelConfigMapper::PrintPreCompoundSettings()
{

   G4cout << " *** PRECOMPOUND *** CURRENT SETTINGS ARE THE FOLLOWING: " << G4endl; 
   G4cout << " =========================================================== " << G4endl;
   
   G4DeexPrecoParameters* precoparams = G4NuclearLevelData::GetInstance()->GetParameters();

   G4cout << "LevelDensity " <<  precoparams->GetLevelDensity() << " (in MeV)" << G4endl;
   G4cout << "R0 " << precoparams->GetR0() << " (in fermi)" << G4endl;
   G4cout << "TransitionsR0 " << precoparams->GetTransitionsR0() << " (in fermi)" << G4endl;
   G4cout << "FermiEnergy " << precoparams->GetFermiEnergy() << " (inMeV)" << G4endl;
   G4cout << "PrecoLowEnergy " << precoparams->GetPrecoLowEnergy() << " (in MeV)" << G4endl;
   G4cout << "PhenoFactor " << precoparams->GetPhenoFactor() << G4endl;
   G4cout << "MinExcitation " << precoparams->GetMinExcitation() << " (in eV)" << G4endl;
   G4cout << "MaxLifeTime " << precoparams->GetMaxLifeTime() << " (in microseconds)" << G4endl;
   G4cout << "MinExPerNucleounForMF " << precoparams->GetMinExPerNucleounForMF() << " (in GeV)" << G4endl;
   G4cout << "MinZForPreco " << precoparams->GetMinZForPreco() << G4endl;
   G4cout << "MinAForPreco " << precoparams->GetMinAForPreco() << G4endl;
   G4cout << "PrecoModelType " << precoparams->GetPrecoModelType() << G4endl;
   G4cout << "GetDeexModelType " << precoparams->GetDeexModelType() << G4endl;
   G4cout << "NeverGoBack " << precoparams->NeverGoBack() << G4endl;
   G4cout << "UseSoftCutoff " << precoparams->UseSoftCutoff() << G4endl;
   G4cout << "UseCEM " << precoparams->UseCEM() << G4endl;
   G4cout << "UseGNASH " << precoparams->UseGNASH() << G4endl;
   G4cout << "UseHETC " << precoparams->UseHETC() << G4endl;
   G4cout << "UseAngularGen " << precoparams->UseAngularGen() << G4endl; 
   // --> FIXME !! What is this ?? --> G4cout << "UseFilesNEW " << precoparams->UseFilesNEW() << G4endl;
   G4cout << "CorrelatedGamma " << precoparams->CorrelatedGamma() << G4endl;
   // --> FIXME !! What is this ?? --> G4cout << "StoreAllLevels " << precoparams->StoreAllLevels() << G4endl;
   // Figure out how to handle --> G4cout << "DedxChannelsType " << precoparams->GetDeexChannelsType() << G4endl;

   return;

}

void ModelConfigMapper::ChangeParameterViaG4UI( const std::string& model, const std::string& param, const double& value, bool verb )
{

   std::string mod = model;
   mod = ToLower(mod);
   
   std::map< std::string, std::map<std::string,std::string> >::iterator itr = fConfigParameters.find(mod);
   if ( itr == fConfigParameters.end() )
   {
      // bail out with a warning
      G4cout << " Can NOT find model " << model << " (" << mod << ")" << G4endl;
      return;
   }

   std::string par = param;
   par = ToLower(par);
   
   std::map<std::string,std::string>::iterator itr1 = (itr->second).find(par);
   
   if ( itr1 == (itr->second).end() )
   {
      // bail out with a warning
      G4cout << " Can NOT find parameter " << param << " (" << par << ")" << G4endl;
      return;
   }
   
   G4ApplicationState currentstate = G4StateManager::GetStateManager()->GetCurrentState();
   
   // changes propagate through G4UI only if the system is in certain states
   // (Idle or PreInit)
   //
   // bool ok = G4StateManager::GetStateManager()->SetNewState(G4State_Idle);
   bool ok = G4StateManager::GetStateManager()->SetNewState(G4State_PreInit);
   if ( !ok )
   {
     // G4cout << " G4StateManager PROBLEM: can NOT change state to G4State_Idle !" << G4endl;
     G4cout << " G4StateManager PROBLEM: can NOT change state to G4State_PreInit !" << G4endl;
     return;
   }            
   
   // compose command to apply change
   //
   std::string command = fBaseCommand + (fNameConvention.find(mod))->second;
   command += ( itr1->second + " " );
   std::ostringstream cmd;
   cmd << value;   
   command += cmd.str();
   cmd.str("");
   cmd.clear();
   
   if ( verb )
   {
      G4cout << " New value of parameter " << param << "=" << value << G4endl;
      G4cout << " Applying G4UI command: " << command << G4endl;
  }

   G4UImanager* uim = G4UImanager::GetUIpointer();   
   uim->ApplyCommand( command.c_str() );
   
   if ( verb )
   {
      G4cout << " Current Settings: " << std::endl;
      PrintCurrentSettings();
   }   

//      G4cout << " Cross-check  usePreCompound = " << G4CascadeParameters::usePreCompound() << G4endl;
//      G4cout << " Cross-check radiusScale = " << G4CascadeParameters::radiusScale() << G4endl;
//      G4cout << " Cross-check xsecScale = = " << G4CascadeParameters::xsecScale() << G4endl;

   // restore previous state
   //  
   ok = G4StateManager::GetStateManager()->SetNewState(currentstate);

   return;

}

void ModelConfigMapper::ChangeParameterPreCo( const std::string& param, const double& value, bool verb )
{

   std::string par = param;
   par = ToLower(par);
   
   G4DeexPrecoParameters* precoparams = G4NuclearLevelData::GetInstance()->GetParameters();
   
   bool value_asbool = false;
   if ( value > 0. ) value_asbool = true;

   if ( par == "leveldensity" )
   {
      precoparams->SetLevelDensity( value );
   }
   else if ( par == "r0" )
   {
      precoparams->SetR0( value );
   }
   else if ( par == "transitionsr0" )
   {
      precoparams->SetTransitionsR0( value );
   }
   else if ( par == "fermienergy" )
   {
      precoparams->SetFermiEnergy( value );
   }
   else if ( par == "precolowenergy" )
   {
      precoparams->SetPrecoLowEnergy( value );
   }
   else if ( par == "phenofactor" )
   {
      precoparams->SetPhenoFactor( value );
   }
   else if ( par == "minexcitation" )
   {
      precoparams->SetMinExcitation( value );
   }
   else if (  par == "maxlifetime" )
   {
      precoparams->SetMaxLifeTime( value );
   }
   else if ( par == "minexpernucleonformf" )
   {
      precoparams->SetMinExPerNucleounForMF( value );
   }
   else if ( par == "minzforpreco" )
   {
      precoparams->SetMinZForPreco( (int)value );
   }
   else if ( par == "minaforpreco" )
   {
      precoparams->SetMinAForPreco( (int)value );
   }
   else if ( par == "precomodeltype" )
   {
      precoparams->SetPrecoModelType( (int)value );
   }
   else if ( par == "deexmodeltype" )
   {
      precoparams->SetDeexModelType( (int)value );
   }
   else if ( par == "nevergoback" )
   {
      precoparams->SetNeverGoBack( value_asbool );
   }
   else if ( par == "usesoftcutoff" )
   {
      precoparams->SetUseSoftCutoff( value_asbool );
   }
   else if ( par == "usecem" )
   {
      precoparams->SetUseCEM( value_asbool );
   }
   else if ( par == "usegnash" )
   {
      precoparams->SetUseGNASH( value_asbool );
   }
   else if ( par == "usehetc" )
   {
      precoparams->SetUseHETC( value_asbool );
   }
   else if ( par == "useangulargen" )
   {
      precoparams->SetUseAngularGen( value_asbool );
   }
   else if ( par == "CorrelatedGamma" )
   {
      precoparams->SetCorrelatedGamma( value_asbool );
   }
   
   if ( verb )
   {
      G4cout << " PreCompound: new value of parameter " << param << "=" << value << G4endl;
   }

   return;

}
