#ifndef ArtG4tkVtx_H
#define ArtG4tkVtx_H

// no need as they come via ArtG4tkParticle
//#include "CLHEP/Vector/LorentzVector.h"
// #include "CLHEP/Vector/ThreeVector.h"

#include "artg4tk/G4PhysModelParamStudy/DataProd/ArtG4tkParticle.hh"

#include "art/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrVector.h"

#include<string>
// #include <ostream>

namespace artg4tk {

class ArtG4tkVtx 
{

   public:
   
      // NOTE(JVY): it might also be useful to add status:
      //            none, decay, interaction (em, had, elastic, inelastic,...)
      
      ArtG4tkVtx() : fVolumeName(""), fMaterialName(""), fIncoming(), fOutcoming() {}
      ArtG4tkVtx( const CLHEP::HepLorentzVector& pos, 
                const std::string& vname, 
		const std::string& mname ) : fPosition(pos), fVolumeName(vname), fMaterialName(mname),
		                             fIncoming(), fOutcoming() {}
      ~ArtG4tkVtx() {}
      
      void SetPosition( const CLHEP::HepLorentzVector& pos )     { fPosition=pos; return; }
      void SetVolumeName( const std::string& vname )             { fVolumeName=vname; return; }
      void SetMaterialName( const std::string& mname )           { fMaterialName=mname; return; }

      void SetIncoming( const art::Ptr<ArtG4tkParticle>& pin )   { fIncoming=pin; return; }
      void AddOutcoming( const art::Ptr<ArtG4tkParticle>& pout ) { fOutcoming.push_back( pout ); return; }
      
      const CLHEP::HepLorentzVector&         GetPosition()            const { return fPosition; }
      const std::string&                     GetVolumeName()          const { return fVolumeName; }
      const std::string&                     GetMaterialName()        const { return fMaterialName; }    
      const art::Ptr<ArtG4tkParticle>&       GetIncoming()            const { return fIncoming; }
      const art::PtrVector<ArtG4tkParticle>& GetAllOutcoming()        const { return fOutcoming; }
      const art::Ptr<ArtG4tkParticle>&       GetOutcoming( size_t i ) const { return fOutcoming.at(i); }

      // Patch up dictionary generation.
      // NOTE(from GenParticle, by HW):
      // There is source in art::PtrVector that should be protected and gccxml trying to instantiate it.
      // Until that is fixed, we need to provide a an operator< or else the dictionary generation fails.
      // So we provide a bogus version here. This is never actually used but,if gccxml cannot find 
      // the method, it quits.
      //
      bool operator<( const ArtG4tkVtx& ) const { return true; }
      
   private:
      
      // data members
      //
      CLHEP::HepLorentzVector         fPosition;
      std::string                     fVolumeName;
      std::string                     fMaterialName;
      art::Ptr<ArtG4tkParticle>       fIncoming;
      art::PtrVector<ArtG4tkParticle> fOutcoming;
     
};

}

#endif
