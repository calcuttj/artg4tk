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
// myInteractionArtHitData.hh: persistent representation of a simple 
// InteractionHit
// by Geant4
// Author: Hans Wenzel (Fermilab)
//=============================================================================// 

#ifndef MYINTERACTION_ART_HIT_DATA_HH
#define MYINTERACTION_ART_HIT_DATA_HH
#include <string>
#include <vector>

namespace artg4tk {

    class myInteractionArtHitData {
    public: // change later
        std::string pname; // name of secondary particle
        double pmom; // momentum of secondary particle
        double theta; // theta of secondary particle  

        // Default constructor
    public:

        myInteractionArtHitData() {
        }

        // Hide the following from Root
#ifndef __GCCXML__

        myInteractionArtHitData(std::string pn, double pm, double th):
        pname(pn),
        pmom(pm),
        theta(th) {
        }

        std::string GetPname() {
            return pname;
        };

        double GetPmom() {
            return pmom;
        };

        double GetTheta() {
            return theta;
        };
#endif

    };

    typedef std::vector<myInteractionArtHitData> myInteractionArtHitDataCollection;
}

#endif
