
#include "artg4tk/ExpDataAccess/VDBConnect.hh"
#include <sstream>

VDBConnect::~VDBConnect()
{

  if (fCurl)
    curl_easy_cleanup(fCurl);
}

bool
VDBConnect::Init()
{

  // Don't do it more than once
  //
  if (fInitialized)
    return fInitialized;

  fCurl = curl_easy_init();
  if (!fCurl) {
    fInitialized = false; // just in case...
    return fInitialized;
  }

  curl_easy_setopt(fCurl, CURLOPT_WRITEFUNCTION, &Write2String);
  //   curl_easy_setopt( fCurl, CURLOPT_WRITEFUNCTION, Write2String ); // By address or by value ???
  curl_easy_setopt(fCurl, CURLOPT_WRITEDATA, &fResponse);

  //
  // to disable "secure" access (e.g. via port 8181) set to false both (!!!) CURLOPT_SSL_VERIFYPEER
  // and CURLOPT_SSL_VERIFYHOST NOTE: the "PROXY" ones will NOT do anything (i.e.
  // CURLOPT_PROXY_SSL_VERIFYPEER & CURLOPT_PROXY_SSL_VERIFYHOST don't matter) available starting
  // curl v7.52.0
  //
  // Interactive use on tev.fnal.gov:
  //     /usr/local/Anaconda3-5.0.1/bin/curl -k
  //     "http://g4devel.fnal.gov:8181/WebAPI/get?format=json&record=2" (with quotation marks)
  //
  // curl_easy_setopt( fCurl, CURLOPT_SSL_VERIFYPEER, 0L);
  // curl_easy_setopt( fCurl, CURLOPT_SSL_VERIFYHOST, 0L );

  fHTTP = "http://g4validation.fnal.gov:8080/DoSSiER/WebAPI";
  // fHTTP = "https://g4devel.fnal.gov:8181/WebAPI";

  fInitialized = true;

  return fInitialized;
}

std::string&
VDBConnect::GetDictionary(const std::string& type)
{

  fResponse.clear();

  std::string url = fHTTP + "/dictionary?name=" + type;

  curl_easy_setopt(fCurl, CURLOPT_URL, url.c_str());

  CURLcode status = curl_easy_perform(fCurl);
  if (status != 0) {
    std::cout << " BAD CURLcode status = " << status << std::endl;
  }

  return fResponse;
}

std::string&
VDBConnect::GetHTTPResponse(const int id)
{

  std::ostringstream os;
  os << id;

  std::string url = fHTTP + "/get?format=json&record=" + os.str();

  // reset (clear out) fResponse
  // otherwise each call will *append* the string record to fResponse...
  //
  fResponse.clear(); // or one can do: fResponse = "";

  // The URL should be (re)set for each specific record
  // Other options can be set just once (see Init())
  //
  curl_easy_setopt(fCurl, CURLOPT_URL, url.c_str());

  // Now just do it !
  //
  CURLcode status = curl_easy_perform(fCurl);
  if (status != 0) {
    std::cout << " BAD CURLcode status = " << status << std::endl;
  }

  // The fResponse variable should now contain the contents of the HTTP response

  return fResponse;
}

size_t
VDBConnect::Write2String(void* ptr, size_t size, size_t count, void* stream)
{

  ((std::string*)stream)->append((char*)ptr, 0, size * count);

  return size * count;
}
