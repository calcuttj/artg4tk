//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//---------------------------------------------------------------------------
//
// ClassName:   G4EmStandardPhysics_muplusLaser
//
// Author:      V.Ivanchenko 09.11.2005
//
// Modified:
// 05.01.2022 D.Rivera: imported into artg4tk based on :
//            geant4.10.06.p01/source/physics_lists/constructors/electromagnetic/src/G4EmStandardPhysics.cc
//            Replace the mu+ ionisation model with one that does not produce
//            deltas to simulate the PDSP ionization laser. Also removed lines
//            that register other undesirable processes for a laser simulation,
//            such as Coulomb scattering, multiple scattering, pair prod.
//            and bremsstrahlung. mu- are untouched
//
//----------------------------------------------------------------------------
//

#include "G4EmStandardPhysics_muplusLaser.hh"

#include "Geant4/G4SystemOfUnits.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4EmParameters.hh"
#include "Geant4/G4LossTableManager.hh"

#include "Geant4/G4ComptonScattering.hh"
#include "Geant4/G4GammaConversion.hh"
#include "Geant4/G4PhotoElectricEffect.hh"
#include "Geant4/G4RayleighScattering.hh"
#include "Geant4/G4LivermorePhotoElectricModel.hh"

#include "Geant4/G4eMultipleScattering.hh"
#include "Geant4/G4MuMultipleScattering.hh"
#include "Geant4/G4hMultipleScattering.hh"
#include "Geant4/G4CoulombScattering.hh"
#include "Geant4/G4eCoulombScatteringModel.hh"
#include "Geant4/G4WentzelVIModel.hh"
#include "Geant4/G4UrbanMscModel.hh"

#include "Geant4/G4MuBremsstrahlungModel.hh"
#include "Geant4/G4MuPairProductionModel.hh"
#include "Geant4/G4hBremsstrahlungModel.hh"
#include "Geant4/G4hPairProductionModel.hh"

#include "Geant4/G4eIonisation.hh"
#include "Geant4/G4eBremsstrahlung.hh"
#include "Geant4/G4eplusAnnihilation.hh"
#include "Geant4/G4UAtomicDeexcitation.hh"

#include "Geant4/G4MuIonisation.hh" // -- mu- only
#include "MyG4MuIonisationNoDelta.hh" // -- mu+ only
#include "Geant4/G4MuBremsstrahlung.hh"
#include "Geant4/G4MuPairProduction.hh"
#include "Geant4/G4hBremsstrahlung.hh"
#include "Geant4/G4hPairProduction.hh"

#include "Geant4/G4hIonisation.hh"
#include "Geant4/G4ionIonisation.hh"
#include "Geant4/G4alphaIonisation.hh"

#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4Gamma.hh"
#include "Geant4/G4Electron.hh"
#include "Geant4/G4Positron.hh"
#include "Geant4/G4MuonPlus.hh"
#include "Geant4/G4MuonMinus.hh"
#include "artg4tk/particles/G4MuonLaser.hh"
#include "Geant4/G4PionPlus.hh"
#include "Geant4/G4PionMinus.hh"
#include "Geant4/G4KaonPlus.hh"
#include "Geant4/G4KaonMinus.hh"
#include "Geant4/G4Proton.hh"
#include "Geant4/G4AntiProton.hh"
#include "Geant4/G4Deuteron.hh"
#include "Geant4/G4Triton.hh"
#include "Geant4/G4He3.hh"
#include "Geant4/G4Alpha.hh"
#include "Geant4/G4GenericIon.hh"

#include "Geant4/G4PhysicsListHelper.hh"
#include "Geant4/G4BuilderType.hh"
#include "Geant4/G4EmModelActivator.hh"
#include "Geant4/G4GammaGeneralProcess.hh"

// factoryGeant4/
#include "Geant4/G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(G4EmStandardPhysics_muplusLaser);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4EmStandardPhysics_muplusLaser::G4EmStandardPhysics_muplusLaser(G4int ver, const G4String&)
  : G4VPhysicsConstructor("G4EmStandard_muplusLaser"), verbose(ver)
{
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetDefaults();
  param->SetVerbose(verbose);
  SetPhysicsType(bElectromagnetic);
  G4cout << "############# mu+ will be used to simulate the ionization laser!!!!! ##############\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4EmStandardPhysics_muplusLaser::~G4EmStandardPhysics_muplusLaser()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4EmStandardPhysics_muplusLaser::ConstructParticle()
{
  // gamma
  G4Gamma::Gamma();

  // leptons
  G4Electron::Electron();
  G4Positron::Positron();
  G4MuonPlus::MuonPlus();
  G4MuonMinus::MuonMinus();
  G4MuonLaser::MuonLaser();

  // mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();

  // barions
  G4Proton::Proton();
  G4AntiProton::AntiProton();

  // ions
  G4Deuteron::Deuteron();
  G4Triton::Triton();
  G4He3::He3();
  G4Alpha::Alpha();
  G4GenericIon::GenericIonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4EmStandardPhysics_muplusLaser::ConstructProcess()
{
  if(verbose > 1) {
    G4cout << "### " << GetPhysicsName() << " Construct Processes " << G4endl;
  }
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  G4LossTableManager* man = G4LossTableManager::Instance();

  // muon & hadron bremsstrahlung and pair production
  G4MuBremsstrahlung* mub = new G4MuBremsstrahlung();
  G4MuPairProduction* mup = new G4MuPairProduction();
  G4hBremsstrahlung* pib = new G4hBremsstrahlung();
  G4hPairProduction* pip = new G4hPairProduction();
  G4hBremsstrahlung* kb = new G4hBremsstrahlung();
  G4hPairProduction* kp = new G4hPairProduction();
  G4hBremsstrahlung* pb = new G4hBremsstrahlung();
  G4hPairProduction* pp = new G4hPairProduction();

  // muon & hadron multiple scattering
  G4MuMultipleScattering* mumsc = new G4MuMultipleScattering();
  mumsc->SetEmModel(new G4WentzelVIModel());
  G4CoulombScattering* muss = new G4CoulombScattering();

  G4hMultipleScattering* pimsc = new G4hMultipleScattering();
  pimsc->SetEmModel(new G4WentzelVIModel());
  G4CoulombScattering* piss = new G4CoulombScattering();

  G4hMultipleScattering* kmsc = new G4hMultipleScattering();
  kmsc->SetEmModel(new G4WentzelVIModel());
  G4CoulombScattering* kss = new G4CoulombScattering();

  G4hMultipleScattering* hmsc = new G4hMultipleScattering("ionmsc");

  // high energy limit for e+- scattering models
  G4double highEnergyLimit = G4EmParameters::Instance()->MscEnergyLimit();

  // Add standard EM Processes
  G4ParticleTable* table = G4ParticleTable::GetParticleTable();
  for(const auto& particleName : partList.PartNames()) {
    G4ParticleDefinition* particle = table->FindParticle(particleName);
    if (!particle) { continue; }
    if (particleName == "gamma") {

      G4PhotoElectricEffect* pee = new G4PhotoElectricEffect();
      pee->SetEmModel(new G4LivermorePhotoElectricModel());

      if(G4EmParameters::Instance()->GeneralProcessActive()) {
        G4GammaGeneralProcess* sp = new G4GammaGeneralProcess();
        sp->AddEmProcess(pee);
        sp->AddEmProcess(new G4ComptonScattering());
        sp->AddEmProcess(new G4GammaConversion());
        sp->AddEmProcess(new G4RayleighScattering());
        man->SetGammaGeneralProcess(sp);
	ph->RegisterProcess(sp, particle);

      } else {
	ph->RegisterProcess(pee, particle);
	ph->RegisterProcess(new G4ComptonScattering(), particle);
	ph->RegisterProcess(new G4GammaConversion(), particle);
	ph->RegisterProcess(new G4RayleighScattering(), particle);
      }

    } else if (particleName == "e-") {

      G4eMultipleScattering* msc = new G4eMultipleScattering;
      G4UrbanMscModel* msc1 = new G4UrbanMscModel();
      G4WentzelVIModel* msc2 = new G4WentzelVIModel();
      msc1->SetHighEnergyLimit(highEnergyLimit);
      msc2->SetLowEnergyLimit(highEnergyLimit);
      msc->SetEmModel(msc1);
      msc->SetEmModel(msc2);

      G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel(); 
      G4CoulombScattering* ss = new G4CoulombScattering();
      ss->SetEmModel(ssm); 
      ss->SetMinKinEnergy(highEnergyLimit);
      ssm->SetLowEnergyLimit(highEnergyLimit);
      ssm->SetActivationLowEnergyLimit(highEnergyLimit);

      ph->RegisterProcess(msc, particle);
      ph->RegisterProcess(new G4eIonisation(), particle);
      ph->RegisterProcess(new G4eBremsstrahlung(), particle);
      ph->RegisterProcess(ss, particle);

    } else if (particleName == "e+") {

      G4eMultipleScattering* msc = new G4eMultipleScattering;
      G4UrbanMscModel* msc1 = new G4UrbanMscModel();
      G4WentzelVIModel* msc2 = new G4WentzelVIModel();
      msc1->SetHighEnergyLimit(highEnergyLimit);
      msc2->SetLowEnergyLimit(highEnergyLimit);
      msc->SetEmModel(msc1);
      msc->SetEmModel(msc2);

      G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel(); 
      G4CoulombScattering* ss = new G4CoulombScattering();
      ss->SetEmModel(ssm); 
      ss->SetMinKinEnergy(highEnergyLimit);
      ssm->SetLowEnergyLimit(highEnergyLimit);
      ssm->SetActivationLowEnergyLimit(highEnergyLimit);

      ph->RegisterProcess(msc, particle);
      ph->RegisterProcess(new G4eIonisation(), particle);
      ph->RegisterProcess(new G4eBremsstrahlung(), particle);
      ph->RegisterProcess(new G4eplusAnnihilation(), particle);
      ph->RegisterProcess(ss, particle);

    } else if (particleName == "mu-" ) {

      ph->RegisterProcess(mumsc, particle);
      ph->RegisterProcess(new G4MuIonisation(), particle);
      ph->RegisterProcess(mub, particle);
      ph->RegisterProcess(mup, particle);
      ph->RegisterProcess(muss, particle);

      // -- distinguish the muplus for substitue of a laser
    } else if (particleName == "mu+") {

      // -- mu+ will ionize ONLY
      G4cout << "##### Turning off Multiple scattering, Coulomb Scattering, Pair Prodution, and Bremstrahlung for mu+!!!\n";
      ph->RegisterProcess(new G4MuIonisation(), particle); // -- ionization
      //ph->RegisterProcess(new MyG4MuIonisationNoDelta(), particle); // -- ionization
      // -- turn off Multiple scattering, bremstrahlung, pair production, and Coulomb scattering
      ph->RegisterProcess(mumsc, particle); // -- multiple scattering
      ph->RegisterProcess(mub, particle); // -- bremmstrahlung
      ph->RegisterProcess(mup, particle); // -- pair production
      ph->RegisterProcess(muss, particle); // -- Coulomb scattering

    } else if (particleName == "alpha" ||
               particleName == "He3") {

      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(new G4ionIonisation(), particle);

    } else if (particleName == "GenericIon") {

      ph->RegisterProcess(hmsc, particle);
      ph->RegisterProcess(new G4ionIonisation(), particle);

    } else if (particleName == "pi+" ||
               particleName == "pi-" ) {

      ph->RegisterProcess(pimsc, particle);
      ph->RegisterProcess(new G4hIonisation(), particle);
      ph->RegisterProcess(pib, particle);
      ph->RegisterProcess(pip, particle);
      ph->RegisterProcess(piss, particle);

    } else if (particleName == "kaon+" ||
               particleName == "kaon-" ) {

      ph->RegisterProcess(kmsc, particle);
      ph->RegisterProcess(new G4hIonisation(), particle);
      ph->RegisterProcess(kb, particle);
      ph->RegisterProcess(kp, particle);
      ph->RegisterProcess(kss, particle);

    } else if (particleName == "proton" ||
	       particleName == "anti_proton") {

      G4hMultipleScattering* pmsc = new G4hMultipleScattering();
      pmsc->SetEmModel(new G4WentzelVIModel());

      ph->RegisterProcess(pmsc, particle);
      ph->RegisterProcess(new G4hIonisation(), particle);
      ph->RegisterProcess(pb, particle);
      ph->RegisterProcess(pp, particle);
      ph->RegisterProcess(new G4CoulombScattering(), particle);

    } else if (particleName == "B+" ||
	       particleName == "B-" ||
	       particleName == "D+" ||
	       particleName == "D-" ||
	       particleName == "Ds+" ||
	       particleName == "Ds-" ||
               particleName == "anti_He3" ||
               particleName == "anti_alpha" ||
               particleName == "anti_deuteron" ||
               particleName == "anti_lambda_c+" ||
               particleName == "anti_omega-" ||
               particleName == "anti_sigma_c+" ||
               particleName == "anti_sigma_c++" ||
               particleName == "anti_sigma+" ||
               particleName == "anti_sigma-" ||
               particleName == "anti_triton" ||
               particleName == "anti_xi_c+" ||
               particleName == "anti_xi-" ||
	       particleName == "deuteron" ||
	       particleName == "lambda_c+" ||
               particleName == "omega-" ||
               particleName == "sigma_c+" ||
               particleName == "sigma_c++" ||
               particleName == "sigma+" ||
               particleName == "sigma-" ||
               particleName == "tau+" ||
               particleName == "tau-" ||
	       particleName == "triton" ||
               particleName == "xi_c+" ||
               particleName == "xi-" ) {

      ph->RegisterProcess(hmsc, particle);
      ph->RegisterProcess(new G4hIonisation(), particle);
    }
  }

  //Find the mulaser particle by hand
  G4ParticleDefinition* particle = table->FindParticle("mulaser");
  ph->RegisterProcess(new MyG4MuIonisationNoDelta(), particle); // -- ionization

  // Deexcitation
  //
  man->SetAtomDeexcitation(new G4UAtomicDeexcitation());

  G4EmModelActivator mact(GetPhysicsName());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
