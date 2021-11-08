// Implementation of StringIDs

#include "artg4tk/util/StringIDs.hh"

#ifndef __GCCXML__
#include "cetlib_except/exception.h"
#endif

// C'tor
artg4tk::StringIDs::StringIDs() {}

// Root need not know about the below
#ifndef __GCCXML__

// Initialize
void
artg4tk::StringIDs::initialize()
{

  // If the string is not empty, then create the map
  if (!stringVec_.empty()) {
    unsigned int count = 0;
    for (auto entry : stringVec_) {
      stringToIdMap_.insert(std::pair<std::string, unsigned int>(entry, count));
    }
  }
}

// Get the ID for a string. If it is not already in the vector, then add it
unsigned int
artg4tk::StringIDs::idGivenString(const std::string& s)
{
  // Do we already have s?
  unsigned int val = 0;
  bool found = false;

  // If the string vector is not empty, then we have to look in the map
  if (!stringVec_.empty()) {

    // If the map is empty, then it hasn't been initialized. Throw.
    if (stringToIdMap_.empty()) {
      throw cet::exception("STRINGIDS") << "You forgot to initialize the StringIds object"
                                        << "\n";
    }

    // Look in the map for the ID number
    auto mapIter = stringToIdMap_.find(s);
    if (mapIter != stringToIdMap_.end()) {
      // Found it
      val = mapIter->second;
      found = true;
    }
  }

  if (!found) {
    // String not found, add it
    val = stringVec_.size();
    stringVec_.push_back(s);
    stringToIdMap_[s] = val;
  }

  return val;
}

// Reset - put in a new string vector (presumedly because you've read one in)
void
artg4tk::StringIDs::reset(StringIDs const& desired)
{

  // Replace contents
  stringVec_ = desired.stringVec_;

  // Re-initialize
  initialize();
}

#endif // __GCCXML__