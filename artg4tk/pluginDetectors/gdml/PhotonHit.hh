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
// PhotonHit.hh: persistent representation of a
// optical Photon hit
//
// Author: Hans Wenzel (Fermilab)
//=============================================================================

#ifndef artg4tk_pluginDetectors_gdml_PhotonHit_hh
#define artg4tk_pluginDetectors_gdml_PhotonHit_hh
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {

  class PhotonHit {
  private:
    int ID;        // copy number of Photodetector
    int processID; // distinguish between Cerenkov and Szintillation photons.
    double edep;
    double xpos;
    double ypos;
    double zpos;
    double time;

  public:
    PhotonHit(){};
    // Hide the following from Root
#ifndef __GCCXML__
    PhotonHit(int id, int pid, double e, double x, double y, double z, double t)
      : ID(id), processID(pid), edep(e), xpos(x), ypos(y), zpos(z), time(t)
    {}

    void
    SetTime(double time)
    {
      this->time = time;
    }

    double
    GetTime() const
    {
      return time;
    }

    void
    SetZpos(double zpos)
    {
      this->zpos = zpos;
    }

    double
    GetZpos() const
    {
      return zpos;
    }

    void
    SetYpos(double ypos)
    {
      this->ypos = ypos;
    }

    double
    GetYpos() const
    {
      return ypos;
    }

    void
    SetXpos(double xpos)
    {
      this->xpos = xpos;
    }

    double
    GetXpos() const
    {
      return xpos;
    }

    void
    SetEdep(double edep)
    {
      this->edep = edep;
    }

    double
    GetEdep() const
    {
      return edep;
    }

    void
    SetProcessID(int processID)
    {
      this->processID = processID;
    }

    int
    GetProcessID() const
    {
      return processID;
    }

    void
    SetID(int ID)
    {
      this->ID = ID;
    }

    int
    GetID() const
    {
      return ID;
    };

#endif
  };

  typedef std::vector<PhotonHit> PhotonHitCollection;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /* artg4tk_pluginDetectors_gdml_PhotonHit_hh */
