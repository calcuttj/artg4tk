#include "artg4tk/pluginActions/PrimaryEvent/PrimaryEventAction_service.hh"

#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "fhiclcpp/ParameterSet.h"

#include "artg4tk/services/ActionHolder_service.hh"
#include "artg4tk/DataProducts/EventGenerators/GenParticle.hh"
#include "artg4tk/DataProducts/EventGenerators/GenParticleCollection.hh"

// Geant4  includes
#include "Geant4/G4Event.hh"

#include <iostream>

using std::string;

artg4tk::PrimaryEventActionService::
PrimaryEventActionService(fhicl::ParameterSet const & p)
: PrimaryGeneratorActionBase(p.get<string>("name", "PrimaryEventActionService")),
// Initialize our message logger
logInfo_("PrimaryEventActionService") {
}

// Destructor

artg4tk::PrimaryEventActionService::~PrimaryEventActionService() {

}

// Create a primary particle for an event!
// (Standard Art G4 simulation)

void artg4tk::PrimaryEventActionService::generatePrimaries(G4Event * anEvent) {
    art::ServiceHandle<artg4tk::ActionHolderService> actionHolder;
    art::Event & e = actionHolder -> getCurrArtEvent();
    typedef std::vector< art::Handle<GenParticleCollection> > HandleVector;
    //HandleVector allGens;
    //e.getManyByType(allGens);
    auto allGens = e.getMany<GenParticleCollection>();
    std::cout << "Primary:: GenParticles*******************Size: " << allGens.size() << std::endl;
    for (HandleVector::const_iterator i = allGens.begin(); i != allGens.end(); ++i) {
        const GenParticleCollection & gens(**i);
	std::cout << "Primary:: GenParticlesCollection*******************Size: " << gens.size() << std::endl;
        for (GenParticleCollection::const_iterator j = gens.begin(); j != gens.end(); ++j) {
            const GenParticle& genpart = *j;
            std::cout << " genpart.pdgId():  " << genpart.pdgId() << std::endl;
            std::cout << "   position:  " << genpart.position() << std::endl;
            //            std::cout << "   time: " << genpart.time() << std::endl;
            //            std::cout << "  proper Time: " << genpart.properTime() << std::endl;
            std::cout << "momentum:  " << genpart.momentum() << std::endl;
            std::cout << "energy:  " << genpart.momentum().e() << std::endl;
            addG4Particle(anEvent,
                    genpart.pdgId(),
                    genpart.position(),
                    0.0,
                    genpart.momentum().e(),
                    genpart.momentum());
        }
    }









    /*       if(genParticles_) {
         for (unsigned i=0; i < genParticles_->size(); ++i) {
           const GenParticle& genpart = (*genParticles_)[i];
           addG4Particle(event,
                         genpart.pdgId(),
                         // Transform into G4 world coordinate system
                         genpart.position() + mu2eOrigin,
                         genpart.time(),
                         genpart.properTime(),
                         genpart.momentum());

           parentMapping_->addEntry(i, SimParticleCollection::key_type());
         }
       }
     */





}

void artg4tk::PrimaryEventActionService::addG4Particle(G4Event *event,
        int pdgId,
        const G4ThreeVector& pos,
        double time,
        double energy,
        //           double properTime,
        const G4ThreeVector& mom) {
    // Create a new vertex
    //     properTime=0.0;
    G4PrimaryVertex* vertex = new G4PrimaryVertex(pos, time);

    //    static int const verbosityLevel = 0; // local debugging variable
    //    if ( verbosityLevel > 0) {
    //      cout << __func__ << " pdgId   : " <<pdgId << endl;
    //    }

    //       static bool firstTime = true; // uncomment generate all nuclei ground states
    //       if (firstTime) {
    //         G4ParticleTable::GetParticleTable()->GetIonTable()->CreateAllIon();
    //         firstTime = false;
    //       }


    G4PrimaryParticle* particle =
            new G4PrimaryParticle(pdgId,
            mom.x(),
            mom.y(),
            mom.z(),
            energy);

    // Add the particle to the event.
    vertex->SetPrimary(particle);

    // Add the vertex to the event.
    event->AddPrimaryVertex(vertex);
}

using artg4tk::PrimaryEventActionService;
DEFINE_ART_SERVICE(PrimaryEventActionService)
