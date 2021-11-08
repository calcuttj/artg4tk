//
// A minimal class to hold information about generated particles.
//

#include "artg4tk/DataProducts/EventGenerators/GenParticle.hh"
#include <ostream>

artg4tk::GenParticle::GenParticle()
  : _pdgId(PDGCode::invalid), _parent(), _children(), _position(), _momentum(), _status(undefined)
{}

artg4tk::GenParticle::GenParticle(PDGCode::type pdgId,
                                  art::Ptr<GenParticle> const& parent,
                                  CLHEP::Hep3Vector const& position,
                                  CLHEP::HepLorentzVector const& momentum,
                                  status_type status)
  : _pdgId(pdgId)
  , _parent(parent)
  , _children()
  , _position(position)
  , _momentum(momentum)
  , _status(status)
{}

void
artg4tk::GenParticle::addChild(art::Ptr<GenParticle> const& child)
{
  _children.push_back(child);
}

std::ostream&
artg4tk::operator<<(std::ostream& ost, const artg4tk::GenParticle& genp)
{
  ost << "[ "
      << "pdg: " << genp.pdgId() << " "
      << "Position: " << genp.position() << " "
      << "4-momentum " << genp.momentum() << " "
      << "status:    " << genp.status() << " "
      << "parent: ";
  if (genp.parent().id() == art::ProductID()) {
    ost << "none ";
  } else {
    ost << genp.parent().id() << " " << genp.parent().key() << " ";
  }
  ost << " children: ";

  if (genp.children().empty()) {
    ost << "none";
  } else {
    ost << "( ";
    for (auto const& child : genp.children()) {
      ost << " " << child.id() << "." << child.key();
    }
    ost << " )";
  }
  ost << " ]";
  return ost;
}
