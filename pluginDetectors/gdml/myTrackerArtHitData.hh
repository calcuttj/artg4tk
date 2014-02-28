// Calorimeter Art Hits  

#ifndef MYTRACKER_ART_HIT_DATA_HH
#define MYTRACKER_ART_HIT_DATA_HH

#include <vector>

namespace artg4tk {

    class myTrackerArtHitData {
    public: // change later

        double Edep;
        double xpos;
        double ypos;
        double zpos;
//        double time;

        // Default constructor
    public:

        myTrackerArtHitData() {}
        // Hide the following from Root
#ifndef __GCCXML__

        myTrackerArtHitData(double edep, double xp, double yp, double zp) :
        Edep(edep),
        xpos(xp),
        ypos(yp),
        zpos(zp){
        }

        double GetEdep() {
            return Edep;
        };
#endif

    };

    typedef std::vector<myTrackerArtHitData> myTrackerArtHitDataCollection;
}

#endif
