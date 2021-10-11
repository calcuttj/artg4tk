
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkParticle.hh"

#include "Geant4/G4ParticleTable.hh"

#include <assert.h>

artg4tk::ArtG4tkParticle::ArtG4tkParticle()
{

  fPDG = 0;
}

artg4tk::ArtG4tkParticle::ArtG4tkParticle(const int pdg, const CLHEP::HepLorentzVector& mom)
{

  fPDG = pdg;
  fMomentum = mom;
}

artg4tk::ArtG4tkParticle::ArtG4tkParticle(const int pdg, const CLHEP::Hep3Vector& mom)
{

  SetPDG(pdg);
  SetMomentum(mom);
}

artg4tk::ArtG4tkParticle::ArtG4tkParticle(const artg4tk::ArtG4tkParticle& other)
  : fPDG(other.fPDG), fMomentum(other.fMomentum)
{}

// artg4tk::ArtG4tkParticle& ArtG4tkParticle::operator=( const ArtG4tkParticle& other )
//{
//
//   fPDG = other.fPDG;
//   fMomentum = other.fMomentum;
//   return *this;
//
//}

void
artg4tk::ArtG4tkParticle::SetPDG(const int pdg)
{

  fPDG = pdg;
  return;
}

void
artg4tk::ArtG4tkParticle::SetMomentum(const CLHEP::HepLorentzVector& mom)
{

  fMomentum = mom;
  return;
}

void
artg4tk::ArtG4tkParticle::SetMomentum(const CLHEP::Hep3Vector& mom)
{

  // NOTE(JVY): A bit of "risky business" in case of ions (apart from
  //            the 6 most basic ones) because they're added "on the fly"
  //            during Geant4 simulation... so if used in the job where
  //            Geant4 sim is also running, it's OK; otherwse better avoid.
  //
  assert(fPDG != 0);
  G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* g4pd = ptable->FindParticle(fPDG);
  assert(g4pd);
  double mass = g4pd->GetPDGMass();
  double e = std::sqrt(mom.mag2() + mass * mass);
  fMomentum.setX(mom.x());
  fMomentum.setY(mom.y());
  fMomentum.setZ(mom.z());
  fMomentum.setE(e);

  return;
}

std::string
artg4tk::ArtG4tkParticle::GetName() const
{

  // NOTE(JVY): again, this a bit of "risky business" when it comes
  //            to ions/nuclei that can be produced during Geant4 sim.
  //            It all works fine if the analyzer runs in the same job
  //            as Geant4 app because G4IonTable gets populated with
  //            newly produced ions as they appears (on-the-fly).
  //            (Elementary) Particles can be added to the table at the
  //            very start of the job, that's no problem.
  //            But when reading back evt.products, it's NOT the case.
  //            And somehow even G4IonTable::CreateAll<...> doesn't help.
  //
  //            Maybe better to just store the name as a data member ???

  assert(fPDG != 0);
  G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* g4pd = ptable->FindParticle(fPDG);

  //    assert(g4pd);

  if (fPDG > 1000000000 && !g4pd) {
    return "GenericIon";
  } else {
    return std::string(g4pd->GetParticleName().c_str());
  }
}
