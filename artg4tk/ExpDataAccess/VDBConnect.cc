
#include "artg4tk/ExpDataAccess/VDBConnect.hh"
#include <sstream>

VDBConnect::~VDBConnect()
{

  if (fCurl) curl_easy_cleanup(fCurl); 

}

bool VDBConnect::Init()
{

   // Don't do it more than once
   //
   if ( fInitialized ) return fInitialized;
   
   fCurl = curl_easy_init();
   if ( !fCurl ) 
   {
      fInitialized = false; // just in case...
      return fInitialized;
   }
   
//   fHTTP = "http://g4devel.fnal.gov:8080/ValidationWebAPI/webresources/validationWebapi/json/result/";
//   fHTTP = "http://g4devel.fnal.gov:8080/DoSSiER_WS/json/result/";
   fHTTP = "http://g4devel.fnal.gov:8080/DoSSiER/WebAPI/get?format=json&record=";
   
   curl_easy_setopt( fCurl, CURLOPT_WRITEFUNCTION, &Write2String );
//   curl_easy_setopt( fCurl, CURLOPT_WRITEFUNCTION, Write2String ); // By address or by value ???
   curl_easy_setopt( fCurl, CURLOPT_WRITEDATA,     &fResponse);

   fInitialized = true;
   
   return fInitialized;

}

std::string& VDBConnect::GetHTTPResponse( const int id )
{

   std::ostringstream os;
   os << id;
    
   std::string url = fHTTP + os.str();
   
   // reset (clear out) fResponse
   // otherwise each call will *append* the string record to fResponse... 
   //
   fResponse.clear(); // or one can do: fResponse = "";

   // The URL should be (re)set for each specific record
   // Other options can be set just once (see Init())
   //
   curl_easy_setopt( fCurl, CURLOPT_URL, url.c_str() );
   
   // Now just do it !
   //
   CURLcode status = curl_easy_perform( fCurl ); 
   if ( status != 0 )
   {
      std::cout << " BAD CURLcode status = " << status << std::endl;
   }
   
   // The fResponse variable should now contain the contents of the HTTP response
   
   return fResponse;
    
}

size_t VDBConnect::Write2String( void* ptr, size_t size, size_t count, void* stream )
{

   ((std::string*)stream)->append((char*)ptr, 0, size*count);
   
   return size*count;

}

