//
//  Generate a very simple event; add it to the event as a data product of type
//  GenParticleCollection.
//

#include "artg4tk/DataProducts/EventGenerators/GenParticleCollection.hh"

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

#include "CLHEP/Random/RandGaussQ.h"

#include <cmath>
#include <memory>
#include <vector>

namespace artg4tk {

  class EventGenerator : public art::EDProducer {

  public:
    explicit EventGenerator(fhicl::ParameterSet const& pset);
    virtual ~EventGenerator();

    void produce(art::Event& event) override;

  private:
    int _nparticles;
    int fPDG;
    double fMass;
    CLHEP::Hep3Vector fMomentum;
    CLHEP::Hep3Vector fMomentumSig;
    CLHEP::Hep3Vector fVertex;

    CLHEP::RandGaussQ* fRandom;
  };

}

artg4tk::EventGenerator::EventGenerator(fhicl::ParameterSet const& pset)
  : EDProducer{pset}
  , _nparticles(pset.get<int>("nparticles", 1))
  , fPDG(pset.get<int>("pdgcode", 2212))
  , fMass(pset.get<double>("mass", 0.938272))
  , // mass in GeV !!!
  fMomentumSig(CLHEP::Hep3Vector(0., 0., 0.))
  , fVertex(CLHEP::Hep3Vector(0., 0., 0.))
  , fRandom(0)
{

  std::vector<double> mom = pset.get<std::vector<double>>("momentum");
  //
  fMomentum.setX(mom[0]); //*GeV;
  fMomentum.setY(mom[1]); //*GeV;
  fMomentum.setZ(mom[2]); //*GeV;

  std::vector<double> momsig =
    pset.get<std::vector<double>>("momentum_sigma", std::vector<double>());
  if (!momsig.empty())
    fMomentumSig.setX(momsig[0]);
  if (momsig.size() >= 2)
    fMomentumSig.setY(momsig[1]);
  if (momsig.size() >= 3)
    fMomentumSig.setZ(momsig[2]);

  std::vector<double> vtx = pset.get<std::vector<double>>("vertex", std::vector<double>());
  if (!vtx.empty())
    fVertex.setX(vtx[0]); // in cm
  if (vtx.size() >= 2)
    fVertex.setY(vtx[1]); // in cm
  if (vtx.size() >= 3)
    fVertex.setZ(vtx[2]); // in cm

  long seed = pset.get<long>("RNDMSeed", -1);
  if (seed == -1) {
    // Construct seed from time and pid. (default behavior if
    // no seed is provided by the fcl file)
    // Note: According to Kevin Lynch, the two lines below are not portable.
    seed = time(0) + getpid();
    seed =
      ((seed & 0xFFFF0000) >> 16) | ((seed & 0x0000FFFF) << 16); // exchange upper and lower word
    seed = seed % 900000000; // ensure the seed is in the correct range for engine
  }

  fRandom = new CLHEP::RandGaussQ(
    createEngine(seed)); // create_engine is inherited from "EngineCreator" base class

  produces<GenParticleCollection>();
}

artg4tk::EventGenerator::~EventGenerator()
{
  if (fRandom)
    delete fRandom;
}

void
artg4tk::EventGenerator::produce(art::Event& event)
{
  std::unique_ptr<GenParticleCollection> gens(new GenParticleCollection());
  gens->reserve(_nparticles);

  if (fPDG == PDGCode::invalid)
    return;

  // Apply momentum spread (assume gauss)
  double tmpx = std::max(0., (fMomentumSig.x() * fRandom->fire() + std::fabs(fMomentum.x())));
  double tmpy = std::max(0., (fMomentumSig.y() * fRandom->fire() + std::fabs(fMomentum.y())));
  double tmpz = std::max(0., (fMomentumSig.z() * fRandom->fire() + std::fabs(fMomentum.z())));
  if (fMomentum.x() < 0.)
    tmpx *= -1.;
  if (fMomentum.y() < 0.)
    tmpy *= -1.;
  if (fMomentum.z() < 0.)
    tmpz *= -1.;
  CLHEP::Hep3Vector tmp(tmpx, tmpy, tmpz);

  double e = std::sqrt(tmp.mag2() + fMass * fMass);
  CLHEP::HepLorentzVector mom4(tmp, e);
  PDGCode::type code = static_cast<PDGCode::type>(fPDG);
  gens->push_back(GenParticle(code, art::Ptr<GenParticle>(), fVertex, mom4, GenParticle::alive));

  // Put the output collection into the event.
  event.put(std::move(gens));
}

DEFINE_ART_MODULE(artg4tk::EventGenerator)
