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

#ifndef PhotonHit_h
#define PhotonHit_h 1
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {

    class PhotonHit {
    private:
    public: // change later
        int processID;   // distinguish between Cerenkov and Szintillation photons.  
        double edep;
        double xpos;
        double ypos;
        double zpos;
        double time;

    public:

        PhotonHit(){};
        // Hide the following from Root
#ifndef __GCCXML__
        PhotonHit(int id, double e, double x, double y, double z, double t) :
        processID(id),
        edep(e),
        xpos(x),
        ypos(y),
        zpos(z),
        time(t) {
        };
      //      ~PhotonHit();
      //  PhotonHit(const PhotonHit&);
      //  const PhotonHit& operator=(const PhotonHit&);
      //  int operator==(const PhotonHit&) const;

    public:

                /*
                void SetProcessID(int track) {
                    processID = track;
                };

                void SetEdep(double de) {
                    edep = de;
                };

                void SetPos(G4ThreeVector xyz) {
                    pos = xyz;
                };

                void SetTime(double de) {
                    time = de;
                };
        int GetProcessID() {
            return processID;
        };

        double GetEdep() {
            return edep;
        };

        double GetXPos() {
            return xpos;
        };

        double GetYPos() {
            return ypos;
        };

        double GetZPos() {
            return zpos;
        };

        double GetTime() {
            return time;
        };
*/
#endif
    };

    typedef std::vector<PhotonHit> PhotonHitCollection;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
