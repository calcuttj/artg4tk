//
// Authors: Julia Yarba

// Date: May 2015

#include <string>
#include <vector>
#include <map>

// Art includes
//
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"

// Services
//
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Optional/TFileService.h"
#
#include "messagefacility/MessageLogger/MessageLogger.h" 

// Root 
//
#include "TFile.h"
#include "TDirectory.h"
#include "TH1D.h"
#include "TClass.h"

// RooMUHistos 
//
#include "PlotUtils/MUH1D.h"
#include "PlotUtils/MUApplication.h"

#include <iostream>

namespace artg4tk {

  class RooMUHistSummary : public art::EDAnalyzer 
  {
  
  public:

    // Constructor
    explicit RooMUHistSummary( fhicl::ParameterSet const& );

    // Destructor
    virtual ~RooMUHistSummary();

    virtual void analyze( art::Event const& ) override;
    virtual void beginJob()                   override;
    virtual void beginRun( art::Run const& )  override;
    virtual void endRun( art::Run const& )    override;
    virtual void endJob()                     override;

  private:
  
    std::string                                       fG4DefaultDir;    
    std::map< std::string, std::vector<std::string> > fBand2Dirs;
    
    // Diagnostics 
    //
    int                      fVerbosity;
    mf::LogInfo              fLogInfo;
    
  };
}

artg4tk::RooMUHistSummary::RooMUHistSummary(fhicl::ParameterSet const& p )
  : art::EDAnalyzer(p),
    fLogInfo("RooMUHistSummary")
{
      
   fG4DefaultDir  = p.get<std::string>("G4DefaultDirectory"); 
   
   fhicl::ParameterSet bands = p.get<fhicl::ParameterSet>("G4GroupsOfVariants");
   std::vector<std::string> keys = bands.get_keys();
   for ( unsigned int ik=0; ik<keys.size(); ++ik )
   {
      std::vector<std::string> dirs = bands.get<std::vector<std::string> >(keys[ik]);
      fBand2Dirs.insert( std::pair< std::string, std::vector<std::string> > ( keys[ik], dirs ) );
   }
      
   fVerbosity = p.get<int>("Verbosity",0);

   TH1::SetDefaultSumw2();
   
   // look in the PSet, module label would be there, most likely module_label data member
     
}

artg4tk::RooMUHistSummary::~RooMUHistSummary()
{

}

// At begin job
//
void artg4tk::RooMUHistSummary::beginJob()
{
   
   return;

}

void artg4tk::RooMUHistSummary::endJob()
{
    
   art::ServiceHandle<art::TFileService> tfs;

   TFile& hfile = tfs->file();
   
   // Check if the Default dir is valid
   // (protection againts error(s) in the config
   //
   if ( !hfile.GetDirectory( fG4DefaultDir.c_str() ) )
   {
      fLogInfo << " Can NOT access specified default directory " << fG4DefaultDir << ", bail out";
      return;
   } 

   // Now handle all directories with variants
   //
   
   std::map< std::string, std::vector<std::string> >::iterator itr = fBand2Dirs.begin();
   
   // Check if variants directories are valid
   //
   for ( ; itr!=fBand2Dirs.end(); ++itr )
   {     
      for ( unsigned int i=0; i<(itr->second.size()); ++i )
      {
         if ( !hfile.GetDirectory( (itr->second)[i].c_str() ) )
         {
            fLogInfo << " Can NOT access specified directory for a variant " << (itr->second)[i] << ", bail out";
            return;
         }
      }
   }
         
   // NOTE(JVY): at this point, it'll point at the directory that corresponds
   //            to the label of the last module that called TFileService
   // const TString oldDir = gDirectory->GetPath();
      
   // NOTE(JVY): While in memory, those are TObject(s) (OBJ), not TKey(s) (KEY) !!!
   //
   TIter  next( hfile.GetDirectory( fG4DefaultDir.c_str() )->GetList() );
   
   // FIXME/NOTE(JVY): NEED TO FIGURE OUT IF AT THIS POINT TObjArray with the config info
   //                  EXISTS AS TKey (written out already) OR AS A TObject (in memory) !!!
            
   TObject* obj = next();
   TH1D*     h = 0;
   
   while ( obj )
   {

      if ( !(TClass::GetClass(obj->ClassName())->InheritsFrom(TH1::Class())) ) 
      {
	 obj = next();
	 continue;
      }
      h = (TH1D*)obj;
      const char* hname = h->GetName();            

      PlotUtils::MUH1D* muh1 = tfs->make<PlotUtils::MUH1D>( *h, 1. );

      // NOTE(JVY): and now, gDirectory->GetPath() will point at this module's subdir !
      //
      muh1->SetTitle( h->GetTitle() );

      // Here loop over **groups** of variants (for multiple bands, if needs be)
      //
      // Reset iterator
      //
      itr = fBand2Dirs.begin();
      //
      // Do the loop over groups of variants
      //       
      for ( ; itr!=fBand2Dirs.end(); ++itr )
      {
         std::vector<TH1D*> hvariants;
         hvariants.reserve( (itr->second).size() );
         hvariants.clear();
         for ( unsigned int i=0; i<(itr->second).size(); ++i )
         {
	 std::string hvarname = (itr->second)[i] + "/" + std::string( hname );
	 hvariants.push_back( (TH1D*)(hfile.Get(hvarname.c_str())) );
         }
         std::string muh1vband = itr->first + "_vband_" + std::string( hname );
         muh1->AddVertErrorBand( muh1vband, hvariants );      
      } // end loop over groups of variants (multiple bands)

      // not necessary... as it produces all the same complains about missing dictionaries
      // for pair<string,PlotUtils::MULatErrorBand*>, pair<string,TMatrixT<double>*>, pair<string,TH1D*>
      // muh1->Write();  

      // NOTE(JVY): and after this, gDirectory->GetPath() will point at fG4DefaultDir !
      //
      hfile.cd( fG4DefaultDir.c_str() );

      obj = next();

   }
      
   return;

}

// At begin run
//
void artg4tk::RooMUHistSummary::beginRun( art::Run const& r )
{  
      
   return;

}

//
void artg4tk::RooMUHistSummary::analyze( art::Event const& e )
{
      
   if ( fVerbosity > 0 )  std::cout << "RooMUSummary: at event " << e.id().event() << "\n" << std::endl;

   return;

}

// At end run
//
void artg4tk::RooMUHistSummary::endRun( art::Run const& r)
{

   return;

}

using artg4tk::RooMUHistSummary;
DEFINE_ART_MODULE(RooMUHistSummary)
