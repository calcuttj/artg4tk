// DR Calorimeter Art Hits

#ifndef artg4tk_pluginDetectors_gdml_DRCalorimeterHit_hh
#define artg4tk_pluginDetectors_gdml_DRCalorimeterHit_hh

#include <vector>

namespace artg4tk {

  class DRCalorimeterHit {
  private:
    unsigned int ID;
    double Edep;
    double em_Edep;
    double nonem_Edep;
    int Nceren;
    double xpos;
    double ypos;
    double zpos;
    double time;

    // Default constructor
  public:
    DRCalorimeterHit() {}

    // Hide the following from Root
#ifndef __GCCXML__

    DRCalorimeterHit(int id,
                     double edep,
                     double emedep,
                     double nonemdep,
                     int nceren,
                     double xp,
                     double yp,
                     double zp,
                     double t)
      : ID(id)
      , Edep(edep)
      , em_Edep(emedep)
      , nonem_Edep(nonemdep)
      , Nceren(nceren)
      , xpos(xp)
      , ypos(yp)
      , zpos(zp)
      , time(t)
    {}

    double
    GetEdep()
    {
      return Edep;
    };
    void
    SetID(unsigned int id)
    {
      ID = id;
    };
    void
    SetEdep(double de)
    {
      Edep = de;
    };
    void
    Setem_Edep(double de)
    {
      em_Edep = de;
    };
    void
    Setnonem_Edep(double de)
    {
      nonem_Edep = de;
    };
    void
    SetNceren(int nc)
    {
      Nceren = nc;
    };
    void
    SetXpos(double x)
    {
      xpos = x;
    };
    void
    SetYpos(double y)
    {
      xpos = y;
    };
    void
    SetZpos(double z)
    {
      xpos = z;
    };
    void
    SetTime(double t)
    {
      time = t;
    };

    unsigned int
    GetID() const
    {
      return ID;
    };
    double
    GetEdep() const
    {
      return Edep;
    };
    double
    GetEdepEM() const
    {
      return em_Edep;
    };
    double
    GetEdepnonEM() const
    {
      return nonem_Edep;
    };
    int
    GetNceren() const
    {
      return Nceren;
    };
    double
    GetXpos() const
    {
      return xpos;
    };
    double
    GetYpos() const
    {
      return ypos;
    };
    double
    GetZpos() const
    {
      return zpos;
    };
    double
    GetTime() const
    {
      return time;
    };

#endif
  };

  typedef std::vector<DRCalorimeterHit> DRCalorimeterHitCollection;
}

#endif /* artg4tk_pluginDetectors_gdml_DRCalorimeterHit_hh */
