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
//   user: {
//     ...  
// GDMLDetector : 
//    {
//    category: "world"
//    gdmlFileName_ : "ta_target.gdml"
//    }  
//   }
// }
// </pre>
// Author: Hans Wenzel (Fermilab)
//=============================================================================
// framework includes:
#include "art/Framework/Services/Registry/ServiceMacros.h"
// artg4tk includes: 
#include "artg4tk/pluginDetectors/gdml/GDMLDetector_service.hh"
#include "artg4tk/pluginDetectors/gdml/ColorReader.hh"
#include "artg4tk/pluginDetectors/gdml/CalorimeterSD.hh"
#include "artg4tk/pluginDetectors/gdml/myCaloArtHitData.hh"
#include "artg4tk/pluginDetectors/gdml/DRCalorimeterSD.hh"
#include "artg4tk/pluginDetectors/gdml/myDRCaloArtHitData.hh"
#include "artg4tk/pluginDetectors/gdml/myParticleEContribArtData.hh"
//#include "artg4tk/pluginDetectors/gdml/myParticleNCerenContribArtData.hh"
#include "artg4tk/pluginDetectors/gdml/PhotonSD.hh"
#include "artg4tk/pluginDetectors/gdml/myPhotonArtHitData.hh"
#include "artg4tk/pluginDetectors/gdml/TrackerSD.hh"
#include "artg4tk/pluginDetectors/gdml/myTrackerArtHitData.hh"
#include "artg4tk/pluginDetectors/gdml/InteractionSD.hh"
#include "artg4tk/pluginDetectors/gdml/myInteractionArtHitData.hh"
#include "artg4/services/DetectorHolder_service.hh"
// Geant 4 includes:
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4VUserDetectorConstruction.hh"
#include "Geant4/G4GDMLParser.hh"
#include "Geant4/globals.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4LogicalVolumeStore.hh"
#include "Geant4/G4VPhysicalVolume.hh"
#include "Geant4/G4PhysicalVolumeStore.hh"
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

artg4tk::GDMLDetectorService::GDMLDetectorService(fhicl::ParameterSet const & p, art::ActivityRegistry &)
: artg4::DetectorBase(p,
p.get<string>("name", "GDMLDetectorService"),
p.get<string>("category", "World"),
p.get<string>("mother_category", "")),
gdmlFileName_(p.get<std::string>("gdmlFileName_")),
// Initialize our message logger
logInfo_("GDMLDetectorService") {
}

// Destructor

artg4tk::GDMLDetectorService::~GDMLDetectorService() {
}

std::vector<G4LogicalVolume *> artg4tk::GDMLDetectorService::doBuildLVs() {
    ColorReader* fReader = new ColorReader;
    G4GDMLParser *parser = new G4GDMLParser(fReader);
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
                } else if ((*vit).value == "Interaction") {
                    G4String name = ((*iter).first)->GetName() + "_Interaction";
                    InteractionSD* aInteractionSD = new InteractionSD(name);
                    SDman->AddNewDetector(aInteractionSD);
                    ((*iter).first)->SetSensitiveDetector(aInteractionSD);
                    std::cout << "Attaching sensitive Detector: " << (*vit).value
                            << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
                    DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
                }
            }
        }
    }
    G4cout << *(G4Material::GetMaterialTable());
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
            producer -> produces<myDRCaloArtHitDataCollection>(identifier);
            std::string EdepID = identifier + "Edep";
            producer -> produces<myParticleEContribArtData>(EdepID);
            std::string NCerenID = identifier + "NCeren";
            producer -> produces<myParticleEContribArtData>(NCerenID);
        } else if ((*cii).second == "Calorimeter") {
            std::string identifier = myName() +(*cii).first;
            producer -> produces<myCaloArtHitDataCollection>(identifier);
        } else if ((*cii).second == "PhotonDetector") {
            std::string identifier = myName() + (*cii).first;
            producer -> produces<myPhotonArtHitDataCollection>(identifier);
        } else if ((*cii).second == "Tracker") {
            std::string identifier = myName() + (*cii).first;
            producer -> produces<myTrackerArtHitDataCollection>(identifier);
        } else if ((*cii).second == "Interaction") {
            std::string identifier = myName() + (*cii).first;
            producer -> produces<myInteractionArtHitDataCollection>(identifier);
        }
    }
}

void artg4tk::GDMLDetectorService::doFillEventWithArtHits(G4HCofThisEvent * myHC) {
    std::unique_ptr<myCaloArtHitDataCollection> myArtHits(new myCaloArtHitDataCollection);
    std::unique_ptr<myDRCaloArtHitDataCollection> myDRArtHits(new myDRCaloArtHitDataCollection);
    std::unique_ptr<myPhotonArtHitDataCollection> myPhotonHits(new myPhotonArtHitDataCollection);
    std::unique_ptr<myTrackerArtHitDataCollection> myTrackerHits(new myTrackerArtHitDataCollection);
    std::unique_ptr<myInteractionArtHitDataCollection> myInteractionHits(new myInteractionArtHitDataCollection);
    std::unique_ptr<myParticleEContribArtData> myEdepCon(new myParticleEContribArtData);
    std::unique_ptr<myParticleEContribArtData> myNCerenCon(new myParticleEContribArtData);
    for (int i = 0; i < myHC->GetNumberOfCollections(); i++) {
        G4VHitsCollection* hc = myHC->GetHC(i);
        G4String hcname = hc->GetName();
        std::vector<std::string> y = split(hcname, '_');
        std::string Classname = y[1];
        std::string Volume = y[0];
        std::string SDName = y[0] + "_" + y[1];
        if (Classname == "Calorimeter") {
            G4int NbHits = hc->GetSize();
            for (G4int ii = 0; ii < NbHits; ii++) {
                G4VHit* hit = hc->GetHit(ii);
                CalorimeterHit* Hit = dynamic_cast<CalorimeterHit*> (hit);
                G4ThreeVector Position = Hit->GetPos();
                myCaloArtHitData myhit = myCaloArtHitData(
                        Hit->GetEdep(),
                        Hit->GetEdepEM(),
                        Hit->GetEdepnonEM(),
                        Position.x(),
                        Position.y(),
                        Position.z(),
                        Hit->GetTime()
                        );
                myArtHits->push_back(myhit);
            }
            // Now that we have our collection of artized hits, add them to the event
            art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
            art::Event & e = detectorHolder -> getCurrArtEvent();
            std::string dataname = myName() + Volume;
            e.put(std::move(myArtHits), dataname);
        } else if (Classname == "DRCalorimeter") {
            G4int NbHits = hc->GetSize();
            std::cout << "Number of Hits: " << NbHits << std::endl;
            for (G4int ii = 0; ii < NbHits; ii++) {
                G4VHit* hit = hc->GetHit(ii);
                DRCalorimeterHit* Hit = dynamic_cast<DRCalorimeterHit*> (hit);
                G4ThreeVector Position = Hit->GetPos();
                myDRCaloArtHitData myDRhit = myDRCaloArtHitData(
                        Hit->GetEdep(),
                        Hit->GetEdepEM(),
                        Hit->GetEdepnonEM(),
                        Hit->GetNCeren(),
                        Position.x(),
                        Position.y(),
                        Position.z(),
                        Hit->GetTime()
                        );
                myDRArtHits->push_back(myDRhit);
            }
            // Now that we have our collection of artized hits, add them to the event
            art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
            art::Event & e = detectorHolder -> getCurrArtEvent();
            std::string dataname = myName() + Volume;
            e.put(std::move(myDRArtHits), dataname);
            G4SDManager* fSDM = G4SDManager::GetSDMpointer();
            DRCalorimeterSD* junk = dynamic_cast<DRCalorimeterSD*> (fSDM->FindSensitiveDetector(SDName));
            std::map<std::string, double> EbyParticle = junk->GetEbyParticle();
            double TotalE = junk->GetTotalE();
            myEdepCon -> insert(std::make_pair("ETot", TotalE));
            for (std::map<std::string, double>::iterator it = EbyParticle.begin(); it != EbyParticle.end(); ++it) {
//                std::cout << "Particle: " << it->first << "   " << 100.0 * it->second / TotalE << " % " << std::endl;
                myEdepCon -> insert(std::make_pair(it->first, 100.0 * it->second / TotalE));
            }
            dataname = myName() + Volume + "Edep";
            e.put(std::move(myEdepCon), dataname);
            std::map<std::string, double> NCerenbyParticle = junk->GetNCerenbyParticle();
            double TotalNCeren = junk->GetTotalNCeren();
 //           std::cout << TotalNCeren << std::endl;
            myNCerenCon-> insert(std::make_pair("NCerenTot", TotalNCeren));
            for (std::map<std::string, double>::iterator it = NCerenbyParticle.begin(); it != NCerenbyParticle.end(); ++it) {
                //std::cout << "Particle: " << it->first << "   " << 100.0 * it->second / TotalNCeren << " % " << std::endl;
                myNCerenCon -> insert(std::make_pair(it->first, 100.0 * it->second / TotalNCeren));
            }
//           std::cout << myNCerenCon->size() << std::endl;
            dataname = myName() + Volume + "NCeren";
            e.put(std::move(myNCerenCon), dataname);
        } else if (Classname == "PhotonDetector") {
            G4int NbHits = hc->GetSize();
            for (G4int ii = 0; ii < NbHits; ii++) {
                G4VHit* hit = hc->GetHit(ii);
                PhotonHit* Hit = dynamic_cast<PhotonHit*> (hit);
                G4ThreeVector Position = Hit->GetPos();
                myPhotonArtHitData myPhotonhit = myPhotonArtHitData(
                        Hit->GetProcessID(),
                        Hit->GetEdep(),
                        Hit->GetPos().x(),
                        Hit->GetPos().y(),
                        Hit->GetPos().z(),
                        Hit->GetTime()
                        );
                myPhotonHits->push_back(myPhotonhit);
            }
            // Now that we have our collection of artized hits, add them to the event
            art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
            art::Event & e = detectorHolder -> getCurrArtEvent();
            std::string dataname = myName() + Volume;
            e.put(std::move(myPhotonHits), dataname);
        } else if (Classname == "Tracker") {
            G4int NbHits = hc->GetSize();
            for (G4int ii = 0; ii < NbHits; ii++) {
                G4VHit* hit = hc->GetHit(ii);
                TrackerHit* Hit = dynamic_cast<TrackerHit*> (hit);
                G4ThreeVector Position = Hit->GetPos();
                myTrackerArtHitData myTrackerhit = myTrackerArtHitData(
                        Hit->GetEdep(),
                        Hit->GetPos().x(),
                        Hit->GetPos().y(),
                        Hit->GetPos().z()
                        );
                myTrackerHits->push_back(myTrackerhit);
            }
            // Now that we have our collection of artized hits, add them to the event
            art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
            art::Event & e = detectorHolder -> getCurrArtEvent();
            std::string dataname = myName() + Volume;
            e.put(std::move(myTrackerHits), dataname);
        } else if (Classname == "Interaction") {
            G4int NbHits = hc->GetSize();
            G4cout << "===================    " << NbHits << G4endl;
            for (G4int ii = 0; ii < NbHits; ii++) {
                G4VHit* hit = hc->GetHit(ii);
                InteractionHit* Hit = dynamic_cast<InteractionHit*> (hit);
                Hit->Print();
                myInteractionArtHitData myInteractionhit = myInteractionArtHitData(
                        Hit->GetPname(),
                        Hit->GetPmom(),
                        Hit->GetTheta()
                        );
                myInteractionHits->push_back(myInteractionhit);
            }
            // Now that we have our collection of artized hits, add them to the event
            art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
            art::Event & e = detectorHolder -> getCurrArtEvent();
            std::string dataname = myName() + Volume;
            e.put(std::move(myInteractionHits), dataname);
        } else {
            G4cout << "SD type: " << Classname << " unknown" << G4endl;
        }
    };
}
using artg4tk::GDMLDetectorService;
DEFINE_ART_SERVICE(GDMLDetectorService)
