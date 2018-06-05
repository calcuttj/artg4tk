//
// Build a dictionary.
//
#include <vector>
#include "artg4tk/DataProducts/EventGenerators/GenParticle.hh"
#include "artg4tk/DataProducts/EventGenerators/GenParticleCollection.hh"
#include "artg4tk/DataProducts/EventGenerators/PDGCode.hh"
#include "canvas/Persistency/Common/Wrapper.h"
template class art::PtrVector<artg4tk::GenParticle>;
template class art::Wrapper<artg4tk::GenParticleCollection>;
