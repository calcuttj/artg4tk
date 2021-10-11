#include "artg4tk/pluginActions/PrimaryEvent/PrimaryEventAction_service.hh"

#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "fhiclcpp/ParameterSet.h"

#include "artg4tk/DataProducts/EventGenerators/GenParticle.hh"
#include "artg4tk/DataProducts/EventGenerators/GenParticleCollection.hh"
#include "artg4tk/services/ActionHolder_service.hh"

// Geant4  includes
#include "Geant4/G4Event.hh"

#include <iostream>

artg4tk::PrimaryEventActionService::PrimaryEventActionService(fhicl::ParameterSet const& p)
  : PrimaryGeneratorActionBase(p.get<std::string>("name", "PrimaryEventActionService"))
{}

// Create a primary particle for an event!
// (Standard Art G4 simulation)
void
artg4tk::PrimaryEventActionService::generatePrimaries(G4Event* anEvent)
{
  art::Event const& e = art::ServiceHandle<artg4tk::ActionHolderService>()->getCurrArtEvent();
  auto const allGens = e.getMany<GenParticleCollection>();
  std::cout << "Primary:: GenParticles*******************Size: " << allGens.size() << '\n';
  for (auto const& h : allGens) {
    GenParticleCollection const& gens = *h;
    std::cout << "Primary:: GenParticlesCollection*******************Size: " << gens.size() << '\n';
    for (GenParticle const& genpart : gens) {
      std::cout << " genpart.pdgId():  " << genpart.pdgId()
                << "\n   position:  " << genpart.position() << "\nmomentum:  " << genpart.momentum()
                << "\nenergy:  " << genpart.momentum().e() << '\n';
      addG4Particle(anEvent,
                    genpart.pdgId(),
                    genpart.position(),
                    0.0,
                    genpart.momentum().e(),
                    genpart.momentum());
    }
  }
}

void
artg4tk::PrimaryEventActionService::addG4Particle(G4Event* event,
                                                  int const pdgId,
                                                  G4ThreeVector const& pos,
                                                  double const time,
                                                  double const energy,
                                                  G4ThreeVector const& mom)
{
  auto vertex = new G4PrimaryVertex{pos, time};
  vertex->SetPrimary(new G4PrimaryParticle{pdgId, mom.x(), mom.y(), mom.z(), energy});
  event->AddPrimaryVertex(vertex);
}
