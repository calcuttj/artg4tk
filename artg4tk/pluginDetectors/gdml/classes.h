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
// classes.h: Class which want to make persistent
// Author: Hans Wenzel (Fermilab)
//=============================================================================

//#include <vector>
//#include <map>


#include "art/Persistency/Common/Wrapper.h"
#include "artg4tk/pluginDetectors/gdml/myCaloArtHitData.hh"
#include "artg4tk/pluginDetectors/gdml/myDRCaloArtHitData.hh"
#include "artg4tk/pluginDetectors/gdml/myPhotonArtHitData.hh"
#include "artg4tk/pluginDetectors/gdml/myTrackerArtHitData.hh"
#include "artg4tk/pluginDetectors/gdml/myInteractionArtHitData.hh"
#include "artg4tk/pluginDetectors/gdml/myParticleEContribArtData.hh"
//#include "artg4tk/pluginDetectors/gdml/myParticleNCerenContribArtData.hh"
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< artg4tk::myCaloArtHitDataCollection >;
template class art::Wrapper< artg4tk::myDRCaloArtHitDataCollection >;
template class art::Wrapper< artg4tk::myPhotonArtHitDataCollection >;
template class art::Wrapper< artg4tk::myTrackerArtHitDataCollection >;
template class art::Wrapper< artg4tk::myInteractionArtHitDataCollection >;
template class std::pair<  std::string,double >; 
template class std::map<  std::string,double >;
template class art::Wrapper< artg4tk::myParticleEContribArtData >;
//template class art::Wrapper< artg4tk::myParticleNCerenContribArtData >;
