#ifndef JSON2Data_H
#define JSON2Data_H 1

#include <string>

#include "TH1.h"
#include "TGraphErrors.h"

class JSON2Data
{

   public:
   
      // ctor & dtor
      //
      JSON2Data() : fHisto(0), fGraph(0) {}
      ~JSON2Data() { if ( fHisto ) delete fHisto; if ( fGraph ) delete fGraph; }
      
      TH1D*         Convert2Histo( const std::string&, const char* );
      TGraphErrors* Convert2Graph( const std::string& );
   
   private:
   
     // data members
     //
     TH1D*                       fHisto;
     TGraphErrors*               fGraph;
     

};

#endif
