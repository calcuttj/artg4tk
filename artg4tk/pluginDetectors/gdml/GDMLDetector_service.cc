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
// GDMLDetector_service.hh: 
// GDMLDetectorService is the service that constructs the Geant 4 Geometry 
// as specified in a gdml file.
// To use this service, all you need to do is put it in the services section
// of the fcl configuration file, like this (Just change the name of the gdml file):
// 
// <pre>
// services: { 
//   ...
//     ...  
// GDMLDetector : 
//    {
//    category: "world"
//    gdmlFileName_ : "ta_target.gdml"
//    }  
//   }
// </pre>
// Author: Hans Wenzel (Fermilab)
//=============================================================================
// framework includes:
#include "art/Framework/Services/Registry/ServiceMacros.h"
// artg4tk includes: 
#include "artg4tk/pluginDetectors/gdml/GDMLDetector_service.hh"
#include "artg4tk/pluginDetectors/gdml/ColorReader.hh"
#include "artg4tk/pluginDetectors/gdml/CalorimeterSD.hh"
#include "artg4tk/pluginDetectors/gdml/CalorimeterHit.hh"
#include "artg4tk/pluginDetectors/gdml/DRCalorimeterSD.hh"
#include "artg4tk/pluginDetectors/gdml/DRCalorimeterHit.hh"
#include "artg4tk/pluginDetectors/gdml/ByParticle.hh"
#include "artg4tk/pluginDetectors/gdml/PhotonSD.hh"
#include "artg4tk/pluginDetectors/gdml/PhotonHit.hh"
#include "artg4tk/pluginDetectors/gdml/TrackerSD.hh"
#include "artg4tk/pluginDetectors/gdml/TrackerHit.hh"
#include "artg4tk/pluginDetectors/gdml/HadInteractionSD.hh"
#include "artg4tk/pluginDetectors/gdml/HadIntAndEdepTrkSD.hh"
//
// Geant 4 includes:
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4GDMLParser.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4LogicalVolumeStore.hh"
#include "Geant4/G4VPhysicalVolume.hh"
#include "Geant4/G4PhysicalVolumeStore.hh"
#include "Geant4/G4UserLimits.hh"

// C++ includes
#include <vector>
#include <map>

using std::string;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

artg4tk::GDMLDetectorService::GDMLDetectorService(fhicl::ParameterSet const & p)
: artg4tk::DetectorBase(p,
			p.get<string>("name", "GDMLDetectorService"),
			p.get<string>("category", "World"),
			p.get<string>("mother_category", "")),
  gdmlFileName_( p.get<std::string>("gdmlFileName_","")),
  checkoverlaps_( p.get<bool>("CheckOverlaps",false)),
  dumpMP_( p.get<bool>("DumpMaterialProperties",false)),
  logInfo_("GDMLDetectorService") {

}

// Destructor

artg4tk::GDMLDetectorService::~GDMLDetectorService() {
}

std::vector<G4LogicalVolume *> artg4tk::GDMLDetectorService::doBuildLVs() {
    ColorReader* fReader = new ColorReader;
    G4GDMLParser *parser = new G4GDMLParser(fReader);
    parser->SetOverlapCheck(checkoverlaps_);

    parser->Read(gdmlFileName_);
    G4VPhysicalVolume *World = parser->GetWorldVolume();
    std::cout << World->GetTranslation() << std::endl << std::endl;
    std::cout << "Found World:  " << World-> GetName() << std::endl;
    std::cout << "World LV:  " << World->GetLogicalVolume()->GetName() << std::endl;
    G4LogicalVolumeStore *pLVStore = G4LogicalVolumeStore::GetInstance();
    std::cout << "Found " << pLVStore->size()
            << " logical volumes."
            << std::endl << std::endl;
    G4PhysicalVolumeStore *pPVStore = G4PhysicalVolumeStore::GetInstance();
    std::cout << "Found " << pPVStore->size()
            << " physical volumes."
            << std::endl << std::endl;
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    const G4GDMLAuxMapType* auxmap = parser->GetAuxMap();
    std::cout << "Found " << auxmap->size()
            << " volume(s) with auxiliary information."
            << std::endl << std::endl;
    for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin();
            iter != auxmap->end(); iter++) {
        G4cout << "Volume " << ((*iter).first)->GetName()
                << " has the following list of auxiliary information: "
                << G4endl;
        for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
                vit != (*iter).second.end(); vit++) {
            std::cout << "--> Type: " << (*vit).type
                    << " Value: " << (*vit).value << std::endl;
            if ((*vit).type == "StepLimit") {
                G4UserLimits *fStepLimit = new G4UserLimits(atof((*vit).value));
                std::cout << "fStepLimit:  " << atof((*vit).value) << "  " << atof((*vit).value) / CLHEP::cm << " cm" << std::endl;
                ((*iter).first)->SetUserLimits(fStepLimit);
            }
            if ((*vit).type == "SensDet") {
                if ((*vit).value == "DRCalorimeter") {
                    G4String name = ((*iter).first)->GetName() + "_DRCalorimeter";
                    DRCalorimeterSD* aDRCalorimeterSD = new DRCalorimeterSD(name);
                    SDman->AddNewDetector(aDRCalorimeterSD);
                    ((*iter).first)->SetSensitiveDetector(aDRCalorimeterSD);
                    std::cout << "Attaching sensitive Detector: " << (*vit).value
                            << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
                    DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
                } else if ((*vit).value == "Calorimeter") {
                    G4String name = ((*iter).first)->GetName() + "_Calorimeter";
                    CalorimeterSD* aCalorimeterSD = new CalorimeterSD(name);
                    SDman->AddNewDetector(aCalorimeterSD);
                    ((*iter).first)->SetSensitiveDetector(aCalorimeterSD);
                    std::cout << "Attaching sensitive Detector: " << (*vit).value
                            << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
                    DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
                } else if ((*vit).value == "PhotonDetector") {
                    G4String name = ((*iter).first)->GetName() + "_PhotonDetector";
                    PhotonSD* aPhotonSD = new PhotonSD(name);
                    SDman->AddNewDetector(aPhotonSD);
                    ((*iter).first)->SetSensitiveDetector(aPhotonSD);
                    std::cout << "Attaching sensitive Detector: " << (*vit).value
                            << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
                    DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
                } else if ((*vit).value == "Tracker") {
                    G4String name = ((*iter).first)->GetName() + "_Tracker";
                    TrackerSD* aTrackerSD = new TrackerSD(name);
                    SDman->AddNewDetector(aTrackerSD);
                    ((*iter).first)->SetSensitiveDetector(aTrackerSD);
                    std::cout << "Attaching sensitive Detector: " << (*vit).value
                            << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
                    DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
                } else if ((*vit).value == "HadInteraction") {
                    G4String name = ((*iter).first)->GetName() + "_HadInteraction";
                    HadInteractionSD* aHadInteractionSD = new HadInteractionSD(name);
                    // NOTE: This will be done in the HadInteractionSD ctor
                    // SDman->AddNewDetector(aHadInteractionSD);
                    ((*iter).first)->SetSensitiveDetector(aHadInteractionSD);
                    std::cout << "Attaching sensitive Detector: " << (*vit).value
                            << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
                    DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
                } else if ((*vit).value == "HadIntAndEdepTrk") {
                    G4String name = ((*iter).first)->GetName() + "_HadIntAndEdepTrk";
                    HadIntAndEdepTrkSD* aHadIntAndEdepTrkSD = new HadIntAndEdepTrkSD(name);
                    // NOTE: This will be done in the HadIntAndEdepTrkSD ctor
                    // SDman->AddNewDetector(aHadIntAndEdepTrkSD);
                    ((*iter).first)->SetSensitiveDetector(aHadIntAndEdepTrkSD);
                    std::cout << "Attaching sensitive Detector: " << (*vit).value
                            << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
                    DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
                }
            }
        }
    }
    if (dumpMP_)
      {
	G4cout << *(G4Material::GetMaterialTable());
	/*
	G4Region* region = G4RegionStore::GetInstance()->GetRegion("DefaultRegionForTheWorld", false);
	std::vector<G4Material*>::const_iterator mItr = region->GetMaterialIterator();
	size_t nMaterial = region->GetNumberOfMaterials();
	G4cout << nMaterial << G4endl;
	for (size_t iMate = 0; iMate < nMaterial; iMate++) {
	  G4cout << (*mItr)->GetName() << G4endl;
	  G4MaterialPropertiesTable* mt = (*mItr)->GetMaterialPropertiesTable();
	  if (mt != nullptr) {
            mt->DumpTable();
	    //            if (mt->GetProperty("SLOWCOMPONENT", true) != nullptr) {
	    //  mt->GetProperty("SLOWCOMPONENT", true)->SetSpline(true);
            //    std::cout << "Scint " << mt->GetProperty("SLOWCOMPONENT", true)->GetVectorLength()<< std::endl;
            //}
        }
        mItr++;
    }
    G4cout << G4endl;
*/
}
    std::cout << "List SD Tree: " << std::endl;
    SDman->ListTree();
    std::cout << " Collection Capacity:  " << SDman->GetCollectionCapacity() << std::endl;
    G4HCtable* hctable = SDman->GetHCtable();
    for (G4int j = 0; j < SDman->GetCollectionCapacity(); j++) {
        std::cout << "HC Name: " << hctable->GetHCname(j) << "   SD Name:  " << hctable->GetSDname(j) << std::endl;
    }
    std::cout << "==================================================" << std::endl;
    // Return our logical volumes.
    std::vector<G4LogicalVolume *> myLVvec;
    myLVvec.push_back(pLVStore->at(0)); // only need to return the LV of the world
    std::cout << "nr of LV ======================:  " << myLVvec.size() << std::endl;
    return myLVvec;
}

std::vector<G4VPhysicalVolume *> artg4tk::GDMLDetectorService::doPlaceToPVs(std::vector<G4LogicalVolume *>) {
    // Note we don't use our input.
    std::vector<G4VPhysicalVolume *> myPVvec;
    G4PhysicalVolumeStore *pPVStore = G4PhysicalVolumeStore::GetInstance();
    myPVvec.push_back(pPVStore->at(pPVStore->size() - 1)); // only need to return the PV of the world  (last entry in Volume Store)
    return myPVvec;
}

void artg4tk::GDMLDetectorService::doCallArtProduces(art::EDProducer * producer) {
    // Tell Art what we produce, and label the entries
    std::vector<std::pair<std::string, std::string> >::const_iterator cii;
    for (cii = DetectorList.begin(); cii != DetectorList.end(); cii++) {
        if ((*cii).second == "DRCalorimeter") {
            std::string identifier = myName() +(*cii).first;
            producer -> produces<DRCalorimeterHitCollection>(identifier);
            std::string EdepID = identifier + "Edep";
            producer -> produces<ByParticle>(EdepID);
            std::string NCerenID = identifier + "NCeren";
            producer -> produces<ByParticle>(NCerenID);
        } else if ((*cii).second == "Calorimeter") {
            std::string identifier = myName() +(*cii).first;
            producer -> produces<CalorimeterHitCollection>(identifier);
        } else if ((*cii).second == "PhotonDetector") {
            std::string identifier = myName() + (*cii).first;
            producer -> produces<PhotonHitCollection>(identifier);
        } else if ((*cii).second == "Tracker") {
            std::string identifier = myName()+(*cii).first;
            producer -> produces<TrackerHitCollection>(identifier);
        } else if ((*cii).second == "HadInteraction") {
            // std::string identifier = myName() + (*cii).first;
            producer -> produces<ArtG4tkVtx>(); // do NOT use product instance name (for now)
        } else if ((*cii).second == "HadIntAndEdepTrk") {
            producer->produces<ArtG4tkVtx>();
            producer->produces<TrackerHitCollection>();
        }
    }
}

void artg4tk::GDMLDetectorService::doFillEventWithArtHits(G4HCofThisEvent * myHC) {
        //
    // NOTE(JVY): 1st hadronic interaction will be fetched as-is from HadInteractionSD
    //            a copy (via copy ctor) will be placed directly into art::Event
    //
    std::vector<std::pair<std::string, std::string> >::const_iterator cii;
    //std::cout << "****************Detectorlist size:  " << DetectorList.size() << std::endl;
    for (cii = DetectorList.begin(); cii != DetectorList.end(); cii++) {
        std::string sdname = (*cii).first + "_" + (*cii).second;
	//        std::cout << "****************SDNAME:" << sdname << std::endl;
        if ((*cii).second == "HadInteraction") {
            G4SDManager* sdman = G4SDManager::GetSDMpointer();
            HadInteractionSD* hisd = dynamic_cast<HadInteractionSD*> (sdman->FindSensitiveDetector(sdname));
            if (hisd) {
                const ArtG4tkVtx& inter = hisd->Get1stInteraction();
                if (inter.GetNumOutcoming() > 0) {
                    std::unique_ptr<ArtG4tkVtx> firstint(new ArtG4tkVtx(inter));
                    art::ServiceHandle<artg4tk::DetectorHolderService> detectorHolder;
                    art::Event & e = detectorHolder -> getCurrArtEvent();
                    e.put(std::move(firstint)); // note that there's NO product instance name (for now, at least)
                    // (part of) the is that the name is encoded into the "collection"
                    // which is NOT used in this specific case
                }
                hisd->clear(); // clear out after movind info to EDM; no need to clea out in the producer !
            }
        } else if ((*cii).second == "HadIntAndEdepTrk") {
            G4SDManager* sdman = G4SDManager::GetSDMpointer();
            HadIntAndEdepTrkSD* sd = dynamic_cast<HadIntAndEdepTrkSD*> (sdman->FindSensitiveDetector(sdname));
            if (sd) {
                art::ServiceHandle<artg4tk::DetectorHolderService> detectorHolder;
                art::Event & e = detectorHolder -> getCurrArtEvent();
                const ArtG4tkVtx& inter = sd->Get1stInteraction();
                if (inter.GetNumOutcoming() > 0) {
                    std::unique_ptr<ArtG4tkVtx> firstint(new ArtG4tkVtx(inter));
                    e.put(std::move(firstint)); // note that there's NO product instance name (for now, at least)
                    // (part of) the is that the name is encoded into the "collection"
                    // which is NOT used in this specific case
                }
                const TrackerHitCollection& trkhits = sd->GetEdepTrkHits();
                if (!trkhits.empty()) {
                    std::unique_ptr<TrackerHitCollection> hits(new TrackerHitCollection(trkhits));
                    e.put(std::move(hits));
                }
                sd->clear(); // clear out after moving info to EDM; no need to clea out in the producer !
            }
        } else if ((*cii).second == "Tracker") {
            G4SDManager* sdman = G4SDManager::GetSDMpointer();
            TrackerSD* trsd = dynamic_cast<TrackerSD*> (sdman->FindSensitiveDetector(sdname));
            art::ServiceHandle<artg4tk::DetectorHolderService> detectorHolder;
            art::Event & e = detectorHolder -> getCurrArtEvent();
            const TrackerHitCollection& trkhits = trsd->GetHits();
            std::unique_ptr<TrackerHitCollection> hits(new TrackerHitCollection(trkhits));
            std::string identifier = myName()+(*cii).first;
            e.put(std::move(hits), identifier);
        } else if ((*cii).second == "Calorimeter") {
            G4SDManager* sdman = G4SDManager::GetSDMpointer();
            CalorimeterSD* calsd = dynamic_cast<CalorimeterSD*> (sdman->FindSensitiveDetector(sdname));
            art::ServiceHandle<artg4tk::DetectorHolderService> detectorHolder;
            art::Event & e = detectorHolder -> getCurrArtEvent();
            const CalorimeterHitCollection& calhits = calsd->GetHits();
            std::unique_ptr<CalorimeterHitCollection> hits(new CalorimeterHitCollection(calhits));
            std::string identifier = myName()+(*cii).first;
            e.put(std::move(hits), identifier);
        } else if ((*cii).second == "DRCalorimeter") {
            G4SDManager* sdman = G4SDManager::GetSDMpointer();
            DRCalorimeterSD* drcalsd = dynamic_cast<DRCalorimeterSD*> (sdman->FindSensitiveDetector(sdname));
            art::ServiceHandle<artg4tk::DetectorHolderService> detectorHolder;
            art::Event & e = detectorHolder -> getCurrArtEvent();
            const DRCalorimeterHitCollection& drcalhits = drcalsd->GetHits();
            std::unique_ptr<DRCalorimeterHitCollection> hits(new DRCalorimeterHitCollection(drcalhits));
            std::string identifier = myName()+(*cii).first;
            e.put(std::move(hits), identifier);
            //
            const ByParticle& edeps = drcalsd->GetEbyParticle();
            std::unique_ptr<ByParticle> edepsptr(new ByParticle(edeps));
            std::string edidentifier = myName()+(*cii).first + "Edep";
            e.put(std::move(edepsptr), edidentifier);
            //
            const ByParticle& nceren = drcalsd->GetNCerenbyParticle();
            std::unique_ptr<ByParticle> ncerenptr(new ByParticle(nceren));
            std::string ncidentifier = myName()+(*cii).first + "NCeren";
            e.put(std::move(ncerenptr), ncidentifier);
        } else if ((*cii).second == "PhotonDetector") {
            G4SDManager* sdman = G4SDManager::GetSDMpointer();
            PhotonSD* phsd = dynamic_cast<PhotonSD*> (sdman->FindSensitiveDetector(sdname));
            art::ServiceHandle<artg4tk::DetectorHolderService> detectorHolder;
            art::Event & e = detectorHolder -> getCurrArtEvent();
            const PhotonHitCollection& phhits = phsd->GetHits();
            std::unique_ptr<PhotonHitCollection> hits(new PhotonHitCollection(phhits));
            std::string identifier = myName()+(*cii).first;
            e.put(std::move(hits), identifier);
        }
    }
}
using artg4tk::GDMLDetectorService;
DEFINE_ART_SERVICE(GDMLDetectorService)
