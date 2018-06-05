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
// InteractionSD.cc: sensitive detector that analysis the first interaction
// (based in test23 of the Geant 4 test suite)
// Author: Hans Wenzel (Fermilab)
//=============================================================================
// Geant 4 includes:
// #include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4ios.hh"
// #include "Geant4/G4RunManager.hh"
//artg4tk includes:
#include "artg4tk/pluginDetectors/gdml/HadInteractionSD.hh"
// c++ includes:
#include <typeinfo>

artg4tk::HadInteractionSD::HadInteractionSD(G4String name)
   : G4VSensitiveDetector(name) 
{
      
   fFirstInter.Clear();
   fStopAfter1stInt=false;

   G4SDManager* sdman = G4SDManager::GetSDMpointer();
   if ( !sdman->FindSensitiveDetector(name) ) sdman->AddNewDetector( this );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

artg4tk::HadInteractionSD::~HadInteractionSD() 
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void artg4tk::HadInteractionSD::Initialize(G4HCofThisEvent*) 
{

   fFirstInter.Clear();
   return;
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool artg4tk::HadInteractionSD::ProcessHits( G4Step* theStep, G4TouchableHistory* ) 
{

   G4Track*           trk = theStep->GetTrack();
   G4VPhysicalVolume* vol = trk->GetVolume();
   G4Material*        mat = trk->GetMaterial();
         
   const std::vector<const G4Track*>* secs = theStep->GetSecondaryInCurrentStep();
   int nsec = secs->size();
   
   // check if anything generated in this step; if not, just return (nothing to do)
   //
   if ( nsec <=0 ) return true;
      
   // check if primary track looses idenity
   //
   if ( trk->GetTrackStatus() != fAlive )
   {
      if ( trk->GetParentID() == 0 )
      {

         // OK, it's 1st (hadron inelastic) interaction
		 	 
/*
	 std::cout << " HadIntSD: 1st hadronic interaction " << std::endl;
	 std::cout << " HadIntSD: volume ptr = " << vol << std::endl;
	 std::cout << " HadIntSD: volume name = " << vol->GetName() << std::endl;
	 std::cout << " HadIntSD: position = " << theStep->GetTrack()->GetPosition().x() << " "
	                                       << theStep->GetTrack()->GetPosition().y() << " "
					       << theStep->GetTrack()->GetPosition().z() << std::endl;

	 
         std::cout << " nsec = " << nsec << std::endl;
*/	 
	 fFirstInter.SetPosition( CLHEP::HepLorentzVector( trk->GetPosition(), trk->GetGlobalTime() ) );
	 fFirstInter.SetPhysVolumeName( vol->GetName().c_str() );
	 fFirstInter.SetMaterialName( mat->GetName().c_str() );
	 fFirstInter.SetIncoming( ArtG4tkParticle( trk->GetParticleDefinition()->GetPDGEncoding(),
	                                           theStep->GetPreStepPoint()->GetMomentum() ) );
	 
	 if ( fStopAfter1stInt ) theStep->GetTrack()->SetTrackStatus( fStopAndKill );

         for ( int i=0; i<nsec; i++ )
         {
//            const G4Track* tr = (*secs)[i];
	    G4Track* tr = (G4Track*)((*secs)[i]);
	    fFirstInter.AddOutcoming( ArtG4tkParticle( tr->GetParticleDefinition()->GetPDGEncoding(),
	                                               tr->GetMomentum() ) );
            if ( fStopAfter1stInt ) tr->SetTrackStatus( fStopAndKill );	    
	 }	 

      }
   }


/*
    int nsc = fFirstInter->GetNumberOfSecondaries();
    if (nsc > 0) {
        for (int i = 0; i < nsc; i++) {
            delete fFirstInter->GetSecondary(i);
        }
        fFirstInter->Clear();
    }
    nsc = fOtherInter->GetNumberOfSecondaries();
    if (nsc > 0) {
        for (int i = 0; i < nsc; i++) {
            delete fOtherInter->GetSecondary(i);
        }
        fOtherInter->Clear();
    }
    const std::vector<const G4Track*>* secs = aStep->GetSecondaryInCurrentStep();
    int nsec = secs->size();
    for (int i = 0; i < nsec; i++) {

        G4Track* tr = new G4Track(*((*secs)[i]));

        if (aStep->GetTrack()->GetTrackStatus() != fAlive) // track looses identity
        {
            if (aStep->GetTrack()->GetParentID() == 0) // primary track
            {
                fFirstInter->AddSecondary(tr);
            } else // secondary track, and it's also looses identity (re-interaction)
            {
                fOtherInter->AddSecondary(tr);
            }
        }

    } //end loop over secondaries
    G4int NSec = fFirstInter->GetNumberOfSecondaries();
    if (NSec > 0) {
        const G4DynamicParticle* sec = 0;
        for (G4int i = 0; i < NSec; i++) {
            sec = fFirstInter->GetSecondary(i)->GetDynamicParticle();
            const G4String& pname = sec->GetDefinition()->GetParticleName();
            G4double pmom = (sec->GetTotalMomentum()) / MeV;
            G4double Ekin = (sec->GetKineticEnergy()) / MeV;
            G4double theta = (sec->GetMomentum()).theta();
            InteractionHit* newHit = new InteractionHit(pname, pmom, Ekin, theta);
            interactionCollection->insert(newHit);
        }
    }
*/


    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

