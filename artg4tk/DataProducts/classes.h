//
// Build a dictionary.
//
#include <vector>
#include "artg4tk/DataProducts/GenParticle.hh"
#include "artg4tk/DataProducts/GenParticleCollection.hh"
#include "artg4tk/DataProducts/PDGCode.hh"
#include "art/Persistency/Common/Wrapper.h"
template class art::PtrVector<artg4tk::GenParticle>;
template class art::Wrapper<artg4tk::GenParticleCollection>;
