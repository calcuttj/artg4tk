// Physical Volume Store Data

#ifndef GDML_TEXT_HH
#define GDML_TEXT_HH

#include <string>

// Store the Geometry GDML

namespace artg4tk {
  class GdmlText {
    public:
    
      GdmlText() :
        content_()
      {}
    
      virtual ~GdmlText() {}
    
      // Forward these functions
      #ifndef __GCCXML__
    
      GdmlText(const std::string & content) :
        content_(content)
      {}
      
      const std::string & getData() { return content_; }
      
      #endif
        
    private:
      std::string content_;
  };
}

#endif