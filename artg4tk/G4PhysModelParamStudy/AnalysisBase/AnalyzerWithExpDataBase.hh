#ifndef artg4tk_G4PhysModelParamStudy_AnalysisBase_AnalyzerWithExpDataBase_hh
#define artg4tk_G4PhysModelParamStudy_AnalysisBase_AnalyzerWithExpDataBase_hh

#include "art/Framework/Core/EDAnalyzer.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "artg4tk/ExpDataAccess/JSON2Data.hh"
#include "artg4tk/ExpDataAccess/VDBConnect.hh"

#include <map>
#include <string>
#include <vector>

namespace fhicl {
  class ParameterSet;
}

class TH1;

namespace artg4tk {

  class AnalyzerWithExpDataBase : public art::EDAnalyzer {

  public:
    explicit AnalyzerWithExpDataBase(const fhicl::ParameterSet&);
    virtual ~AnalyzerWithExpDataBase();

    void endJob() override;

  protected:
    bool matchVDBRec2MC(const int&, const double&, const int&);
    bool findExpDataByBeamTarget(const int&, const double&, const int&);
    virtual TH1*
    matchExpSpectrum2MC(const int&, const std::vector<std::string>&, const int&)
    {
      return NULL;
    }

    void rebinMC2Data(const std::string&);

    void overlayDataMC();
    /*
          template <class T>
          bool matchExpData2MC( const int&, const std::vector<T>& );
    */
    /*
          template<typename ... ARGS>
          bool matchExpData2MC( const int& secid, ARGS ... args );
    */
    /*
          template<typename T, typename... Args>
          bool matchExpData2MC(T t, Args... args);
    */
    /*
    template <class T>
    void func2( std::initializer_list<T> list )
    {
        for( auto elem : list )
        {
            std::cout << elem << std::endl ;
        }
    }
    */

    bool fIncludeExpData;
    std::vector<int> fVDBRecordID;
    VDBConnect* fVDBConnect;
    JSON2Data* fJSON2Data;
    //
    std::map<int, std::string> fJSONRecords;
    std::vector<std::pair<int, TH1*>> fVDBRecID2MC;

    // diagnostics output
    //
    mf::LogInfo fLogInfo;
  };

}

#endif /* artg4tk_G4PhysModelParamStudy_AnalysisBase_AnalyzerWithExpDataBase_hh */
