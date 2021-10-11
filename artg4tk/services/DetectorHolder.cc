// For more comprehensive documentation, see DetectorHolderService.hh.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: July 2012

// Includes

#include "artg4tk/Core/DetectorBase.hh"
#include "artg4tk/services/DetectorHolder_service.hh"

#include "messagefacility/MessageLogger/MessageLogger.h"

// Save ourselves the trouble of typing 'std::' all the time
using std::endl;
using std::map;
using std::pair;
using std::string;

// Category for this file
static std::string msgctg = "DetectorHolderService";

// PUBLIC METHODS

// Constructor doesn't do anything with either of its passed components.
artg4tk::DetectorHolderService::DetectorHolderService(fhicl::ParameterSet const&)
  : categoryMap_(), worldPV_(nullptr)
{}

// Register a detector object with this service
void
artg4tk::DetectorHolderService::registerDetector(DetectorBase* const db)
{
  mf::LogDebug(msgctg) << "Registering detector named " << db->myName();
  addDBtoCategoryMap(db);
}

// Get the physical volume for the world/lab in the simulation
G4VPhysicalVolume*
artg4tk::DetectorHolderService::worldPhysicalVolume()
{
  // Check if we have a world yet.
  if (0 == worldPV_) {
    // We don't - let's construct all the physical volumes.
    constructAllPVs();

    // Check whether we have a world at this point. If not, we have a problem.
    if (0 == worldPV_) {
      throw cet::exception("DetectorHolderService") << "No world volume "
                                                    << "constructed!\n";
    }
  }
  // If we reach this point, the world volume exists, so let's return it!
  return worldPV_;
}

// Set up all the detectors' LVs
void
artg4tk::DetectorHolderService::constructAllLVs()
{
  // Let's loop over the detectors in the map
  for (auto entry : categoryMap_) {
    mf::LogDebug(msgctg) << "Constructing logical volumes for detector of "
                         << "category " << (entry.second)->category();

    (entry.second)->buildLVs();
  }
}

// Initialize all detectors
void
artg4tk::DetectorHolderService::initialize()
{
  for (auto entry : categoryMap_) {
    mf::LogDebug(msgctg) << "Initializing detector with category " << (entry.second)->category();

    (entry.second)->initialize();
  }
}

// Set up all the detectors' PVs
void
artg4tk::DetectorHolderService::constructAllPVs()
{
  // Let's loop over the detectors in the map
  for (auto entry : categoryMap_) {
    mf::LogDebug(msgctg) << "Constructing physical volumes for detector of "
                         << "category " << (entry.second)->category();

    placeDetector(entry.second);
  }
}

// Get a specific detector, given a category string.
artg4tk::DetectorBase*
artg4tk::DetectorHolderService::getDetectorForCategory(std::string category) const
{

  auto categoryDB = categoryMap_.find(category);

  if (categoryDB != categoryMap_.end()) {
    // We have a detector of that category
    return categoryDB->second;
  }
  throw cet::exception("DetectorHolderService")
    << "No detector found for category " << category << ".LO";
}

// Get the parameter set for a detector given its category string
fhicl::ParameterSet const
artg4tk::DetectorHolderService::getParametersForCategory(std::string category)
{
  return getDetectorForCategory(category)->parameters();
}

// Tell the detectors to tell Art what they produce
void
artg4tk::DetectorHolderService::callArtProduces(art::ProducesCollector& collector)
{
  // Let's loop over the detectors in the map
  for (auto entry : categoryMap_) {
    mf::LogDebug(msgctg) << "Calling art produces for category " << (entry.second)->category();
    (entry.second)->callArtProduces(collector);
  }
}

// Convert geant hits to art hits for all detectors
void
artg4tk::DetectorHolderService::fillEventWithArtHits(G4HCofThisEvent* hc)
{
  // Let's loop over the detectors in the map
  for (auto entry : categoryMap_) {
    mf::LogDebug(msgctg) << "Converting hits for category " << (entry.second)->category();
    (entry.second)->fillEventWithArtHits(hc);
  }
}

// PRIVATE METHODS

// Add a detector base object to our collection of registered detectors
void
artg4tk::DetectorHolderService::addDBtoCategoryMap(DetectorBase* const db)
{
  if (categoryMap_.find(db->category()) != cend(categoryMap_)) {
    // We already have one of these detectors
    throw cet::exception("DetectorHolderService")
      << "Duplicate detector found. "
      << "There is already one detector with the category " << db->category() << ".\n";
  }
  categoryMap_.try_emplace(db->category(), db);
  mf::LogDebug(msgctg) << "Registered detector with category: " << db->category();
}

// Find a detector's mother logical volume and pass it to the detector to
// allow it to create its own physical volume.
void
artg4tk::DetectorHolderService::placeDetector(DetectorBase* const db)
{
  // Check if we're dealing with the world volume first.
  if (db->category() == "world") {
    // The world's mother 'logical volume' is an empty vector.
    worldPV_ = (db->placeToPVs(std::vector<G4LogicalVolume*>()))[0];
    mf::LogDebug(msgctg) << "Just placed detector with category: " << db->category();
    return;
  }

  // Deal with non-world detectors
  auto motherCategoryDB = categoryMap_.find(db->motherCategory());
  if (motherCategoryDB == categoryMap_.end()) {
    throw cet::exception("DetectorHolderService")
      << "No mother volume found for detector with category " << db->category()
      << ", which wanted a mother of category " << db->motherCategory()
      << ". This probably means you are missing a "
      << "detector class (derived from DetectorBase).\n";
  }

  // We have a parent volume - pass the DB its mother volume and call place.
  db->placeToPVs(motherCategoryDB->second->lvs());
  mf::LogDebug(msgctg) << "Just placed detector with category: " << db->category();
}
