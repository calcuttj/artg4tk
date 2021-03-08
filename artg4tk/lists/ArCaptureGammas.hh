
///////////////////////////////////////////////////////////////////////////////
//                   Spectrum of radiative neutron capture by Gadolinium
//                                    version 1.0.0
//                                    (Sep.09.2005)

//                Author : karim.zbiri@subatech.in2p3.fr

//This file contains the gammas spectrum produced in radiative capture of
//neutrons by gadolinium.
//This work is adapted from earlier work in geant3 for chooz 1.

//First version by Karim Zbiri, April, 2005
///////////////////////////////////////////////////////////////////////////////



#ifndef ArCaptureGammas_hh
#define ArCaptureGammas_hh

#include "Geant4/G4ReactionProductVector.hh"
#include <vector>

using namespace std;

class ArCaptureGammas
{
 public:

  ArCaptureGammas();
  ~ArCaptureGammas();
  G4ReactionProductVector * GetGammas ();
  vector<double>  Initialize ();
  vector<double> CapAr40();
  vector<double> continuum();


 public:
  double Elevel;
  double  xint[4][750];

};
#endif
