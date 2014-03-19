//
//               __        __ __  __  __  
//   ____ ______/ /_____ _/ // / / /_/ /__
//  / __ `/ ___/ __/ __ `/ // /_/ __/ //_/
// / /_/ / /  / /_/ /_/ /__  __/ /_/ ,<   
// \__,_/_/   \__/\__, /  /_/  \__/_/|_|  
//               /____/                  
//
// artg4tk: art based Geant 4 Toolkit
// 
//=============================================================================
// ColorReader.cc Custom reader for handling "color" tags extensions in GDML.
// Author: Hans Wenzel (Fermilab)
//=============================================================================

#ifndef ColorReader_H
#define ColorReader_H 1

#include <map>
#include "Geant4/G4GDMLReadStructure.hh"

class G4VisAttributes;

/// GDML reader for the color attributes

class ColorReader : public G4GDMLReadStructure
{

 public:

   ColorReader();
  ~ColorReader();

   void ExtensionRead(const xercesc::DOMElement* const element);
   void ColorRead(const xercesc::DOMElement* const element);

   G4VisAttributes* GetVisAttribute(const G4String& ref);

 protected:

   virtual void VolumeRead(const xercesc::DOMElement* const);

 private:

   std::map<G4String, G4VisAttributes*> fAttribs;
};

#endif
