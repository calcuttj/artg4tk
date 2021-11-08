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

#ifndef artg4tk_pluginDetectors_gdml_ColorReader_hh
#define artg4tk_pluginDetectors_gdml_ColorReader_hh

#include "Geant4/G4GDMLReadStructure.hh"
#include <map>

class G4VisAttributes;

/// GDML reader for the color attributes

class ColorReader : public G4GDMLReadStructure {
public:
  ColorReader();
  ~ColorReader();

  G4VisAttributes* GetVisAttribute(const G4String& ref);

  void ColorRead(const xercesc::DOMElement* const element);

private:
  void ExtensionRead(const xercesc::DOMElement* const element) override;
  void VolumeRead(const xercesc::DOMElement* const) override;

  std::map<G4String, G4VisAttributes*> fAttribs;
};

#endif /* artg4tk_pluginDetectors_gdml_ColorReader_hh */
