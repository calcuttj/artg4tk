// Calorimeter Art Hits  

#ifndef MYCALO_ART_HIT_DATA_HH
#define MYCALO_ART_HIT_DATA_HH

// Hide the following from Root
#ifndef __GCCXML__
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4RotationMatrix.hh"
#endif

#include <vector>

namespace artg4tk {

    class myCaloArtHitData {
    public:                // change later

        double Edep;
        double em_Edep;
        double nonem_Edep;
        double xpos;
        double ypos;
        double zpos;
        double time;

        // Default constructor
    public:

        myCaloArtHitData() {
        }

        // Hide the following from Root
#ifndef __GCCXML__

        //        myCaloArtHitData(G4ThreeVector const & v, G4RotationMatrix const & r, double edep) :
        //        energy_deposition(edep),
        //        x(v.x()), y(v.y()), z(v.z()), phi(r.getPhi()), theta(r.getTheta()), psi(r.getPsi()) {
        //        }

        myCaloArtHitData(double edep, double emedep, double nonemdep, double xp, double yp, double zp, double t) :
        Edep(edep),
        em_Edep(emedep),
        nonem_Edep(nonemdep),
        xpos(xp),
        ypos(yp),
        zpos(zp),
        time(t) {
        }

        G4double GetEdep() {
            return Edep;
        };
#endif
        //    myCaloArtHitData(double  edep, double xp, double yp , double zp, double phip, double thetap, double psip) :
        //  energy_deposition(edep),x(xp),y(yp),z(zp),phi(phip),theta(thetap),psi(psip)
        //  {}
    };

    typedef std::vector<myCaloArtHitData> myCaloArtHitDataCollection;
}

#endif
