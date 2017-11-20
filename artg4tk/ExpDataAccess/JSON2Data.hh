#ifndef JSON2Data_H
#define JSON2Data_H 1

#include <string>

#include "TH1.h"
#include "TGraphErrors.h"

class MetaData
{

   public:
   
      MetaData();
      MetaData( const MetaData& );
      ~MetaData();
      
      bool operator==( const MetaData& ) const;
      
      bool IsMatch( const int&, const double&, 
                    const int&,    const int&, const int& ) const;
      
      bool IsBeamTargetMatch( const int&, const double&, const int& ) const;
      
      // tmp stuff until we'll have a service to extract beam params by beamlink
      //
      int EmulateBeamLink( const int&, const double& ) const; 
      
      // int                      fBeamPID;
      // double                   fBeamMomentum;
      int                      fBeamLink;
      int                      fTargetNucleus;
      int                      fSecondaryPID;
      int                      fObservable;
      std::string              fTitle;
      int                      fRefLink;
      std::vector<std::string> fParNames;
      std::vector<std::string> fParValues;

};

class JSON2Data
{

   public:
   
      // ctor & dtor
      //
      JSON2Data() : fHisto(0), fGraph(0) {}
      ~JSON2Data() { if ( fHisto ) delete fHisto; if ( fGraph ) delete fGraph; }
      
      void ClearMetaData();
      void ParseMetaData( const std::string& );
      const MetaData& GetMetaData() const { return fMetaData; }
      TH1D*           Convert2Histo( const std::string&, const char* );
      TGraphErrors*   Convert2Graph( const std::string&, const char* );
   
   private:
        
     void ConvertGraph2Histo( const char* );
     
     // data members
     //
     MetaData                    fMetaData;
     TH1D*                       fHisto;
     TGraphErrors*               fGraph;
     

};

#endif
