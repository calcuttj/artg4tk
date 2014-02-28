// Physical Volume Store implementation

#include "artg4tk/pluginActions/physicalVolumeStore/physicalVolumeStore_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"

#include <iostream>

artg4tk::PhysicalVolumeStoreService::PhysicalVolumeStoreService(fhicl::ParameterSet const & p,
                                                art::ActivityRegistry& )
  : RunActionBase(p.get<std::string>("name", "physicalVolumeStore")),
    pvs_( new artg4tk::PhysicalVolumeStoreData ),
    logInfo_("PhysicalVolumeStore")
{}

artg4tk::PhysicalVolumeStoreService::~PhysicalVolumeStoreService()
{ }


void artg4tk::PhysicalVolumeStoreService::callArtProduces(art::EDProducer* producer) {
  producer->produces< artg4tk::PhysicalVolumeStoreData, art::InRun>( myName() );
}

unsigned int artg4tk::PhysicalVolumeStoreService::idGivenPhysicalVolume(const G4VPhysicalVolume* pvptr ) {
  
  // Determine the id
  return pvs_->idGivenString( pvptr->GetName() );
}

void artg4tk::PhysicalVolumeStoreService::fillRunEndWithArtStuff(art::Run& r) {
  
  // Debug
  mf::LogDebug("PhysicalVolumeStore") << "Storing " << pvs_->size() << " items in the physical volume store";
  
  // Put our map into the run
  r.put( std::move(pvs_), myName() );
  
  // pvs_ should now be invalid and set to nullptr. But in fact
  // due to https://cdcvs.fnal.gov/redmine/issues/3601 this does not happen.
  // So need to do a release to avoid a segfault
  pvs_.release();
  
  // Point to a new valid collection
  pvs_.reset( new artg4tk::PhysicalVolumeStoreData );
    
}

using artg4tk::PhysicalVolumeStoreService;
DEFINE_ART_SERVICE(PhysicalVolumeStoreService)