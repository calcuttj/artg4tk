// ArtG4DetectorConstruction.cc
// Author: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// See the header file for documentation

#include "ArtG4DetectorConstruction.hh"

artg4tk::ArtG4DetectorConstruction::ArtG4DetectorConstruction(G4VPhysicalVolume* world)
  : world_{world}
{}

G4VPhysicalVolume*
artg4tk::ArtG4DetectorConstruction::Construct()
{
  return world_;
}
