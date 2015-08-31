//
//  Generate a very simple event; add it to the event as a data product of type GenParticleCollection.
//


#include "artg4tk/DataProducts/EventGenerators/GenParticleCollection.hh"


#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

//#include "CLHEP/Random/RandFlat.h"

#include <iostream>
#include <cmath>
#include <memory>

#include <vector>

namespace artg4tk {

  class EventGenerator : public art::EDProducer {

  public:

    explicit EventGenerator( fhicl::ParameterSet const& pset );

    void produce( art::Event& event );

  private:

    //    // Particle data table.
    //PDT const& _pdt;
    int _nparticles;
    int               fPDG;
    double            fMass;
    CLHEP::Hep3Vector fMomentum;
    CLHEP::Hep3Vector fVertex;

    // Particle masses.
    //    double _mpi;
    //double _mka;
    //double _mphi;


  };

}

artg4tk::EventGenerator::EventGenerator( fhicl::ParameterSet const& pset ):
  _nparticles(pset.get<int>("nparticles",1)),
  fPDG(pset.get<int>("pdgcode",2212)),
  fMass(pset.get<double>("mass",0.938272)), // mass in GeV !!!
  fVertex(CLHEP::Hep3Vector(0.,0.,0.))

/*
  _pdt(( art::ServiceHandle<Conditions>()->pdt())),



  _mpi(  _pdt.getById( PDGCode::pi_plus).mass() ),
  _mka(  _pdt.getById( PDGCode::K_plus).mass() ),
  _mphi( _pdt.getById( PDGCode::phi).mass() ),

  _engine(      createEngine(_seed)),
  _flat(       _engine),
  _unitSphere( _engine)
*/
{
  
  std::vector<double> mom = pset.get<std::vector<double> >("momentum");
  fMomentum.setX( mom[0] ); //*GeV;
  fMomentum.setY( mom[1] ); //*GeV;
  fMomentum.setZ( mom[2] ); //*GeV;
  
  produces<GenParticleCollection>();

}

void artg4tk::EventGenerator::produce( art::Event& event ){

  // Create an empty output data product
  std::unique_ptr<GenParticleCollection> gens(new GenParticleCollection());
  gens->reserve(_nparticles);

  // Product Id of the data product to be created; needed for persistent pointers.
  art::ProductID gensID(getProductID<GenParticleCollection>(event));

  // All particles will be produced at the origin.
//  static CLHEP::Hep3Vector origin(0.,0.,0);
//  CLHEP::Hep3Vector mom(0.,0.,10000.);
//  CLHEP::HepLorentzVector lmom( mom, 1400.14);

  if ( fPDG == PDGCode::invalid ) return;

  // Put the phi into the output collection; it is a primary particle that has no parent.
  // gens->push_back ( GenParticle(  PDGCode::proton, art::Ptr<GenParticle>(), origin, lmom, GenParticle::alive));
  
  double e = std::sqrt( fMomentum.mag2() + fMass*fMass ); 
  CLHEP::HepLorentzVector mom4( fMomentum, e ); 
  PDGCode::type code = static_cast<PDGCode::type>(fPDG);
  gens->push_back ( GenParticle(  code, art::Ptr<GenParticle>(), fVertex, mom4, GenParticle::alive) );

  // Put the output collection into the event.
  event.put(std::move(gens));
}

DEFINE_ART_MODULE(artg4tk::EventGenerator)
