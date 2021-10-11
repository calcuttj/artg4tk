// Manage String IDs
//
// The idea here is that you have string identifiers that appear lots of times.
// Instead of storing the strings multiple times in the event (or in many events),
// you store an identifier that represents the string.
//
// This object allows for easy creation and management of the ID numbers.
// This object can be directly stored in Root - it will just store the internal vector.
//
// For easy lookups, there is also a map -- you need to initialize this object
// before you can use it. We don't want Root to know anything about the map,
// hence the @__GCCXML__@ ifdefs.

#ifndef artg4tk_util_StringIDs_hh
#define artg4tk_util_StringIDs_hh

#include <string>
#include <vector>

#ifndef __GCCXML__
#include <map>
#endif

namespace artg4tk {

  class StringIDs {

  public:
    // Default c'tor
    StringIDs();

    // Default d'tor
    virtual ~StringIDs() {}

#ifndef __GCCXML__

    // Initialize everything. You must call this after you
    // create it if you want to add
    void initialize();

    // Given a string, return the ID and add it to the list
    unsigned int idGivenString(const std::string& s);

    // Reset - put in a new string vector (presumedly because you've read one in)
    void reset(StringIDs const& desired);

#endif // __GCCXML__

    // Given an ID, return the string
    const std::string&
    stringGivenID(unsigned int id) const
    {
      return stringVec_.at(id);
    }

    // Return the number of entries
    unsigned int
    size() const
    {
      return stringVec_.size();
    }

  private:
    // The vector that holds the strings
    std::vector<std::string> stringVec_;

#ifndef __GCCXML__
    // Auxillary map
    std::map<std::string, unsigned int> stringToIdMap_;
#endif
  };
}

#endif /* artg4tk_util_StringIDs_hh */
