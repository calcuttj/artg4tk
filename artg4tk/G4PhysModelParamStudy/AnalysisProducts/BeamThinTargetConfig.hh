#ifndef artg4tk_G4PhysModelParamStudy_AnalysisProducts_BeamThinTargetConfig_hh
#define artg4tk_G4PhysModelParamStudy_AnalysisProducts_BeamThinTargetConfig_hh

#include <TNamed.h>
#include <string>

// NO namespece !!!
//
// namespace artg4tk {

class BeamThinTargetConfig : public TNamed {

public:
  BeamThinTargetConfig() : TNamed(), fBeamPartID(0), fBeamMomentum(-1.), fTargetID(0) {}
  BeamThinTargetConfig(const int, const double, const int);
  BeamThinTargetConfig(const BeamThinTargetConfig&);
  BeamThinTargetConfig(TRootIOCtor*);

  ~BeamThinTargetConfig() {}

  void
  SetBeamPartID(const int pid)
  {
    fBeamPartID = pid;
    return;
  }
  void
  SetBeamMomentum(const double m)
  {
    fBeamMomentum = m;
    return;
  }
  void
  SetTargetID(const int tid)
  {
    fTargetID = tid;
    return;
  }

  int
  GetBeamPartID() const
  {
    return fBeamPartID;
  }
  double
  GetBeamMomentum() const
  {
    return fBeamMomentum;
  }
  int
  GetTargetID() const
  {
    return fTargetID;
  }

  virtual void Print(Option_t* opt = "") const;

  bool operator==(const BeamThinTargetConfig&) const;

private:
  int fBeamPartID;
  double fBeamMomentum;
  int fTargetID;

  // this is mandatory because it derives from TObject (via TNamed)
  //
  ClassDef(BeamThinTargetConfig, 10)
};

// }

#endif /* artg4tk_G4PhysModelParamStudy_AnalysisProducts_BeamThinTargetConfig_hh */
