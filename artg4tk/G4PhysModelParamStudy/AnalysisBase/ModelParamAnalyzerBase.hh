#ifndef artg4tk_MPAnalyzerBase_H
#define artg4tk_MPAnalyzerBase_H 1


#include "art/Framework/Core/EDAnalyzer.h"
// --> --> #include "art/Framework/Principal/Event.h" // no need so far...
#include "art/Framework/Principal/Run.h"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "messagefacility/MessageLogger/MessageLogger.h" 

// Run/Eevent data products
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkParticle.hh"   // Event data product
#include "artg4tk/DataProducts/G4ModelConfig/ArtG4tkModelConfig.hh" // Run data product


// Root-specific headers
#include "TFile.h"
#include "TDirectory.h"
#include "TObjArray.h"
// --> --> #include "TObjString.h" // will be included with the source code

#include "artg4tk/ExpDataAccess/VDBConnect.hh"
#include "artg4tk/ExpDataAccess/JSON2Data.hh"

namespace artg4tk {

   class ModelParamAnalyzerBase : public art::EDAnalyzer {
   
   public:
   
      explicit ModelParamAnalyzerBase( const fhicl::ParameterSet& );
      virtual ~ModelParamAnalyzerBase();
      
      virtual void beginRun( const art::Run&  )       override;

      virtual void endJob()                           override;
         
   protected:
   
      void prepareG4PTable();
      void initXSecOnTarget( const std::string&, const ArtG4tkParticle& );

      // These two data members below can go into a base class for
      // this group of analysis modules
      //
      std::string fProdLabel;
      TObjArray*  fModelConfig;
            
      // NOTE: Will also need XSec on TARGET !!!!!
      //
      double fXSecOnTarget; 
      bool   fXSecInit;
            
      bool             fIncludeExpData;
      std::vector<int> fVDBRecordID;
      VDBConnect*      fVDBConnect;
      JSON2Data*       fJSON2Data;
      
      // diagnostics output
      //
      mf::LogInfo fLogInfo;
        
   };

}

#endif
