// Calorimeter Art Hits

#ifndef artg4tk_pluginDetectors_gdml_TrackerHit_hh
#define artg4tk_pluginDetectors_gdml_TrackerHit_hh

#include <vector>

namespace artg4tk {

  class TrackerHit {
  public: // change later
    double Edep;
    double xpos;
    double ypos;
    double zpos;
    double time;

    // Default constructor
  public:
    TrackerHit() {}
    // Hide the following from Root
#ifndef __GCCXML__

    TrackerHit(double edep, double xp, double yp, double zp, double ti)
      : Edep(edep), xpos(xp), ypos(yp), zpos(zp), time(ti)
    {}

    double
    GetEdep()
    {
      return Edep;
    };
#endif
  };

  typedef std::vector<TrackerHit> TrackerHitCollection;
}

#endif /* artg4tk_pluginDetectors_gdml_TrackerHit_hh */
