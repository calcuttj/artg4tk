//
//               __        __ __  __  __
//   ____ ______/ /_____ _/ // / / /_/ /__
//  / __ `/ ___/ __/ __ `/ // /_/ __/ //_/
// / /_/ / /  / /_/ /_/ /__  __/ /_/ ,<
// \__,_/_/   \__/\__, /  /_/  \__/_/|_|
//               /____/
//
// artg4tk: art based Geant 4 Toolkit
//
//=============================================================================
// ParticleChange.hh: checks if particle changes its identity
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef artg4tk_pluginDetectors_gdml_ParticleChange_hh
#define artg4tk_pluginDetectors_gdml_ParticleChange_hh

#include "Geant4/G4VParticleChange.hh"

class ParticleChange : public G4VParticleChange {

public:
  ParticleChange() : fIsFirstInter(false) {}
  ParticleChange(bool isFirst) : fIsFirstInter(isFirst) {}
  virtual ~ParticleChange() {}

  bool
  IsFisrtInteraction() const
  {
    return fIsFirstInter;
  }

private:
  bool fIsFirstInter;
};

#endif /* artg4tk_pluginDetectors_gdml_ParticleChange_hh */
