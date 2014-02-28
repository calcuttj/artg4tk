/* ------------------------------------------------------------------------
            |\___/|       
            )     (    
           =\     /=
             )===(
            /     \         CaTS: DRCalorimeter and Tracker Simulation
            |     |         Author: Hans Wenzel (Fermilab)
           /       \
           \       /
            \__  _/
              ( (
               ) )
              (_(
-------------------------------------------------------------------------*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "artg4tk/pluginDetectors/gdml/DRCalorimeterSD.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Poisson.hh"
#include<iomanip>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace artg4tk {
DRCalorimeterSD::DRCalorimeterSD(G4String name)
: G4VSensitiveDetector(name) {
    G4String HCname = name + "_HC";
    collectionName.insert(HCname);
    CerenGenerator = new(Cerenkov);
    G4cout << collectionName.size() << "   artg4tk::DRCalorimeterSD name:  " << name << " collection Name: " << HCname << G4endl;
    HCID = -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DRCalorimeterSD::~DRCalorimeterSD() {

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DRCalorimeterSD::Initialize(G4HCofThisEvent* HCE) {

    calorimeterCollection = new DRCalorimeterHitsCollection(SensitiveDetectorName, collectionName[0]);
    if (HCID < 0) {
        G4cout << "artg4tk::DRCalorimeterSD::Initialize:  " << SensitiveDetectorName << "   " << collectionName[0] << G4endl;
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    }
    HCE->AddHitsCollection(HCID, calorimeterCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool DRCalorimeterSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    G4double edep = aStep->GetTotalEnergyDeposit() / MeV;
    if (edep == 0.) return false;
    const G4double time = aStep->GetPreStepPoint()->GetGlobalTime() / ns;
    const G4VTouchable* touch = aStep->GetPreStepPoint()->GetTouchable();
    G4String thematerial = touch->GetVolume()->GetLogicalVolume()->GetMaterial()->GetName();
    //const G4ThreeVector cellpos = touch->GetTranslation();
    G4int NCerenPhotons = 0;
    G4Track* theTrack = aStep->GetTrack();
    const G4double charge = theTrack->GetDefinition()->GetPDGCharge();
    G4String particleType = theTrack->GetDefinition()->GetParticleName();

    G4StepPoint* pPreStepPoint = aStep->GetPreStepPoint();
    G4StepPoint* pPostStepPoint = aStep->GetPostStepPoint();
    G4double beta = 0.5 * (pPreStepPoint ->GetBeta() + pPostStepPoint->GetBeta());
    G4double MeanNumberOfPhotons = CerenGenerator->GetAverageNumberOfPhotons(charge, beta, thematerial);
    if (MeanNumberOfPhotons > 0.0) {
        G4double step_length = aStep->GetStepLength();
        MeanNumberOfPhotons = MeanNumberOfPhotons * step_length;
        NCerenPhotons = (G4int) G4Poisson(MeanNumberOfPhotons);
    } else {
        NCerenPhotons = 0;
    }
    const G4ThreeVector cellpos = touch->GetTranslation();
    for (G4int j = 0; j < calorimeterCollection->entries(); j++) {
        DRCalorimeterHit* aPreviousHit = (*calorimeterCollection)[j];
        if (cellpos == aPreviousHit->GetPos()) {
            aPreviousHit->SetEdep(aStep->GetTotalEnergyDeposit() + aPreviousHit->GetEdep());
            aPreviousHit->SetNCeren(NCerenPhotons + aPreviousHit->GetNCeren());
            if ((particleType == "e+") || (particleType == "gamma") || (particleType == "e-")) {
                aPreviousHit->SetEdepEM(edep + aPreviousHit->GetEdepEM());
            } else {
                aPreviousHit->SetEdepnonEM(edep + aPreviousHit->GetEdepnonEM());
            }
            return true;
        }
    }
    DRCalorimeterHit* newHit = new DRCalorimeterHit();
    newHit->SetEdep(edep);
    newHit->SetNCeren(NCerenPhotons);
    newHit->SetPos(cellpos);
    newHit->SetTime(time);
    if ((particleType == "e+") || (particleType == "gamma") || (particleType == "e-")) {
        newHit->SetEdepEM(edep);
        newHit->SetEdepnonEM(0.0);
    } else {
        newHit->SetEdepnonEM(edep);
        newHit->SetEdepEM(0.0);
    }

    calorimeterCollection->insert(newHit);
    return true;
}
Cerenkov::Cerenkov() {
  Initialize();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Cerenkov::~Cerenkov() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Cerenkov::Initialize() {
    //
    // now get the Cerenkov Angle Integrals for all materials that have the refraction index defined
    //
    const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
    G4int numOfMaterials = G4Material::GetNumberOfMaterials();
    for (G4int i = 0; i < numOfMaterials; i++) {
        // Retrieve vector of refraction indices for the material
        // from the material's optical properties table
        G4Material* aMaterial = (*theMaterialTable)[i];
        G4MaterialPropertiesTable* aMaterialPropertiesTable = aMaterial->GetMaterialPropertiesTable();
        G4PhysicsOrderedFreeVector* CerAngleIntegrals = new G4PhysicsOrderedFreeVector();
        if (aMaterialPropertiesTable) {
            G4MaterialPropertyVector* theRefractionIndexVector = aMaterialPropertiesTable->GetProperty("RINDEX");
            if (theRefractionIndexVector) {
                // Retrieve the first refraction index in vector
                // of (photon energy, refraction index) pairs
                G4double currentRI = (*theRefractionIndexVector)[0];
                if (currentRI > 1.0) {
                    // Create first (photon energy, Cerenkov Integral) pair
                    G4double currentPM = theRefractionIndexVector->Energy(0);
                    G4double currentCAI = 0.0;
                    CerAngleIntegrals-> InsertValues(currentPM, currentCAI);
                    // Set previous values to current ones prior to loop
                    G4double prevPM = currentPM;
                    G4double prevCAI = currentCAI;
                    G4double prevRI = currentRI;
                    // loop over all (photon energy, refraction index)
                    // pairs stored for this material
                    for (size_t ii = 1; ii < theRefractionIndexVector->GetVectorLength(); ii++) {
                        currentRI = (*theRefractionIndexVector)[ii];
                        currentPM = theRefractionIndexVector->Energy(ii);
                        currentCAI = 0.5 * (1.0 / (prevRI * prevRI) + 1.0 / (currentRI * currentRI));
                        currentCAI = prevCAI + (currentPM - prevPM) * currentCAI;
                        CerAngleIntegrals ->InsertValues(currentPM, currentCAI);
                        prevPM = currentPM;
                        prevCAI = currentCAI;
                        prevRI = currentRI;
                    }
                }
                G4cout << "Material:  " << aMaterial->GetName() << G4endl;
                G4cout << "Refraction Index: " << G4endl;
                G4cout << "=================" << G4endl;
                theRefractionIndexVector->DumpValues();
                G4cout << "Cerenkov angle Integrals: " << G4endl;
                G4cout << "=========================" << G4endl;
                CerAngleIntegrals ->DumpValues();
                CAI.push_back(aMaterial->GetName());
                CerenkovAngleIntegrals.push_back(CerAngleIntegrals);
                RefractionIndeces.push_back(theRefractionIndexVector);
            }
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// This routine computes the number of Cerenkov photons produced per
// GEANT-unit (millimeter) in the current medium.
//             ^^^^^^^^^^

G4double Cerenkov::GetAverageNumberOfPhotons(const G4double charge, const G4double beta, const G4String Material) const {
    G4bool Ceren = false;
    G4int MaterialIndex = -1;
    //
    // check if optical properties are defined for this material:
    //
    for (unsigned int ii = 0; ii < CAI.size(); ii++) {
        if (CAI[ii] == Material) {
            MaterialIndex = ii;
            Ceren = true;
            break;
        }
    }
    if (!Ceren)return 0.0;
    const G4double Rfact = 369.81 / (CLHEP::eV * CLHEP::cm);
    if (beta <= 0.0)return 0.0;
    if (abs(charge) == 0.0) return 0.0;

    G4double BetaInverse = 1. / beta;
    // Min and Max photon energies
    G4double Pmin = RefractionIndeces[MaterialIndex]->GetMinLowEdgeEnergy();
    G4double Pmax = RefractionIndeces[MaterialIndex]->GetMaxLowEdgeEnergy();

    // Min and Max Refraction Indices
    G4double nMin = RefractionIndeces[MaterialIndex]->GetMinValue();
    G4double nMax = RefractionIndeces[MaterialIndex]->GetMaxValue();

    // Max Cerenkov Angle Integral
    G4double CAImax = CerenkovAngleIntegrals[MaterialIndex]->GetMaxValue();
    G4double dp, ge;

    // If n(Pmax) < 1/Beta -- no photons generated

    if (nMax < BetaInverse) {
        dp = 0;
        ge = 0;
    }// otherwise if n(Pmin) >= 1/Beta -- photons generated

    else if (nMin > BetaInverse) {
        dp = Pmax - Pmin;
        ge = CAImax;
    }// If n(Pmin) < 1/Beta, and n(Pmax) >= 1/Beta, then
        // we need to find a P such that the value of n(P) == 1/Beta.
        // Interpolation is performed by the GetEnergy() and
        // Value() methods of the G4MaterialPropertiesTable and
        // the GetValue() method of G4PhysicsVector.

    else {
        Pmin = RefractionIndeces[MaterialIndex]->GetEnergy(BetaInverse);
        dp = Pmax - Pmin;

        // need boolean for current implementation of G4PhysicsVector
        // ==> being phased out
        G4bool isOutRange;
        G4double CAImin = CerenkovAngleIntegrals[MaterialIndex]->GetValue(Pmin, isOutRange);
        ge = CAImax - CAImin;
    }

    // Calculate number of photons
    G4double NumPhotons = Rfact * charge / CLHEP::eplus * charge / CLHEP::eplus *
            (dp - ge * BetaInverse * BetaInverse);
    return NumPhotons;
}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

