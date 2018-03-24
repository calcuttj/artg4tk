// noble Gas TPC Hits  

#ifndef NOBLEGASTPCHIT_HH
#define NOBLEGASTPCHIT_HH

#include <vector>

namespace artg4tk {

    class nobleGasTPCHit {
    public: // change later

        double Edep;
        double xpos;
        double ypos;
        double zpos;
//        double time;

        // Default constructor
    public:

        nobleGasTPCHit() {}
        // Hide the following from Root
#ifndef __GCCXML__

        nobleGasTPCHit(double edep, double xp, double yp, double zp) :
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

    typedef std::vector<nobleGasTPCHit> nobleGasTPCHitCollection;
}

#endif
