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
// myPhotonArtHitData.hh: persistent representation of a  
// optical Photon hit
// 
// Author: Hans Wenzel (Fermilab)
//=============================================================================

#ifndef myPhotonArtHitData_h
#define myPhotonArtHitData_h 1


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {

    class myPhotonArtHitData {
    private:
    public: // change later
        int processID;   // distinguish between Cerenkov and Szintillation photons.  
        double edep;
        double xpos;
        double ypos;
        double zpos;
        double time;

    public:

        myPhotonArtHitData(){};
        // Hide the following from Root
#ifndef __GCCXML__
        myPhotonArtHitData(int id, double e, double x, double y, double z, double t) :
        processID(id),
        edep(e),
        xpos(x),
        ypos(y),
        zpos(z),
        time(t) {
        };
      //      ~myPhotonArtHitData();
      //  myPhotonArtHitData(const myPhotonArtHitData&);
      //  const myPhotonArtHitData& operator=(const myPhotonArtHitData&);
      //  int operator==(const myPhotonArtHitData&) const;

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

    typedef std::vector<myPhotonArtHitData> myPhotonArtHitDataCollection;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
