#ifndef JSON2Data_H
#define JSON2Data_H 1

#include <map>
#include <string>

#include "TGraphErrors.h"
#include "TH1.h"

class MetaData {

public:
  MetaData();
  MetaData(const MetaData&);
  ~MetaData();

  bool operator==(const MetaData&) const;

  bool IsMatch(const int&, const double&, const int&, const int&, const int&) const;

  bool IsBeamTargetMatch(const int&, const double&, const int&) const;

  // tmp stuff until we'll have a service to extract beam params by beamlink
  //
  int EmulateBeamLink(const int&, const double&) const;

  // int                      fBeamPID;
  // double                   fBeamMomentum;
  int fBeamLink;
  int fTargetNucleus;
  int fSecondaryPID;
  int fObservable;
  std::string fTitle;
  int fRefLink;
  std::vector<std::string> fParNames;
  std::vector<std::string> fParValues;
};

class JSON2Data {

public:
  // ctor & dtor
  //
  JSON2Data() : fHisto(0), fGraph(0)
  {
    fParticles.clear();
    fTargets.clear();
    fBeams.clear();
    fObservables.clear();
    fReferences.clear(), fDataTypes.clear();
  }
  ~JSON2Data()
  {
    if (fHisto)
      delete fHisto;
    if (fGraph)
      delete fGraph;
  }

  void ClearMetaData();
  bool BuildDictionaries(const std::string&, const std::string&);
  void ParseMetaData(const std::string&);
  const MetaData&
  GetMetaData() const
  {
    return fMetaData;
  }
  TH1D* Convert2Histo(const std::string&, const char*);
  TGraphErrors* Convert2Graph(const std::string&, const char*);

private:
  bool BuildParticlesDict(const std::string&);
  bool BuildTargetsDict(const std::string&);
  bool BuildBeamsDict(const std::string&);
  bool BuildObservablesDict(const std::string&);
  bool BuildReferencesDict(const std::string&);
  bool BuildDataTypesDict(const std::string&);
  void ConvertGraph2Histo(const char*);

  // data members
  //
  MetaData fMetaData;
  std::map<std::string, int> fParticles;
  std::map<std::string, int> fTargets;
  std::map<std::string, int> fBeams;
  std::map<std::string, int> fObservables;
  std::map<int, int> fReferences;
  std::map<std::string, int> fDataTypes;
  TH1D* fHisto;
  TGraphErrors* fGraph;
};

#endif
