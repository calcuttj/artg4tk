// ArtG4DetectorConstruction.cc
// Author: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// See the header file for documentation


#include "ArtG4DetectorConstruction.hh"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4tk/services/DetectorHolder_service.hh"

artg4tk::ArtG4DetectorConstruction::ArtG4DetectorConstruction()
{
  art::ServiceHandle<artg4tk::DetectorHolderService> dh;
  world_ = dh -> worldPhysicalVolume();
}

artg4tk::ArtG4DetectorConstruction::ArtG4DetectorConstruction(G4VPhysicalVolume * world)
  : world_(world)
{
  // nothing else to do!
}

G4VPhysicalVolume * artg4tk::ArtG4DetectorConstruction::Construct()
{
  return world_;
}
