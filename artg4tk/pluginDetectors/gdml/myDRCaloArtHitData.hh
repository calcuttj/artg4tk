// DR Calorimeter Art Hits  

#ifndef MYDRCALO_ART_HIT_DATA_HH
#define MYDRCALO_ART_HIT_DATA_HH

#include <vector>

namespace artg4tk {

    class myDRCaloArtHitData {
    public: // change later

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

        myDRCaloArtHitData() {
        }

        // Hide the following from Root
#ifndef __GCCXML__

        myDRCaloArtHitData(double edep, double emedep, double nonemdep, int nceren, double xp, double yp, double zp, double t) :
        Edep(edep),
        em_Edep(emedep),
        nonem_Edep(nonemdep),
        Nceren(nceren),
        xpos(xp),
        ypos(yp),
        zpos(zp),
        time(t) {
        }

        double GetEdep() {
            return Edep;
        };
#endif

    };

    typedef std::vector<myDRCaloArtHitData> myDRCaloArtHitDataCollection;
}

#endif
