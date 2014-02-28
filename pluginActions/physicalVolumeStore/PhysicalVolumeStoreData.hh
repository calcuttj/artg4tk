// Physical Volume Store Data

#ifndef PHYSICALVOLUMESTOREDATA_HH
#define PHYSICALVOLUMESTOREDATA_HH

#include "artg4tk/util/StringIDs.hh"

// Store the physical volume data - this is a wrapper for StringIDs

namespace artg4tk {
  class PhysicalVolumeStoreData {
    public:
    
      PhysicalVolumeStoreData() {}
      virtual ~PhysicalVolumeStoreData() {}
    
      // Forward these functions
      #ifndef __GCCXML__
        
      // Given a string, return the ID
      unsigned int idGivenString(const std::string & s) { return ids_.idGivenString(s); }
    
      // Reset the contents
      void reset( PhysicalVolumeStoreData const & desired ) { ids_.reset( desired.ids_ ); }
    
      #endif
    
      // Given the ID, return the string (you'll call this most often when reading)
      const std::string & stringGivenID(unsigned int id) const { return ids_.stringGivenID(id); }
    
      // The number of entries
      unsigned int size() const { return ids_.size(); }
      
    private:
    
      StringIDs ids_;
  };
}

#endif