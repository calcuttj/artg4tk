#ifndef VDBConnect_H
#define VDBConnect_H 1

#include <iostream>
#include <string>

#include <curl/curl.h> 
// #include "/usr/local/Anaconda3-5.0.1/include/curl/curl.h"

class VDBConnect
{

   public:
   
      // ctor & dtor
      //
      VDBConnect() : fCurl(0), fHTTP(""), fResponse(""), fInitialized(false) {}
      ~VDBConnect();
   
      bool Init();
      bool IsInitialized() { return fInitialized; }
      
      std::string& GetDictionary( const std::string& );

      std::string& GetHTTPResponse( const int );
      
   private:
   
      // private member function(s)
      //
      static size_t  Write2String( void*, size_t, size_t, void* );
        
      // private data members
      //
      CURL*       fCurl;
      std::string fHTTP;
      std::string fResponse;
      bool        fInitialized;
      
      
};

#endif
