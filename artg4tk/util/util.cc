#include "artg4tk/util/util.hh"

#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VisAttributes.hh"

#include <iomanip>
#include <sstream>

#include "cetlib_except/exception.h"
#include <cstdlib>

// Set visual attributes
void
artg4tk::setVisAtts(G4LogicalVolume* lv,
                    bool display,
                    const std::vector<double>& rgba,
                    std::function<void(G4VisAttributes*)> func)
{

  G4VisAttributes* att;

  // Do we want to display?
  if (display) {

    // We do! Set the color
    att = new G4VisAttributes(G4Colour(rgba[0], rgba[1], rgba[2], rgba[3]));

    // Run any function that was passed in
    func(att);

  }

  else {
    // We do not want to display. Too bad.
    att = new G4VisAttributes(0);
  }

  // Set the attributes in the logical volume
  lv->SetVisAttributes(att);
}

// Set visual attributes without a helper function
void
artg4tk::setVisAtts(G4LogicalVolume* lv, bool display, const std::vector<double>& rgba)
{
  setVisAtts(lv, display, rgba, [](G4VisAttributes*) {});
}

// Add number to name
std::string
artg4tk::addNumberToName(const std::string& name, int number)
{

  std::ostringstream newName;
  newName << name << '[' << std::setfill('0') << std::setw(2) << number << ']';

  return newName.str();
}

// find base path
std::string
artg4tk::basePath(const std::string& envVar, const std::string& pkgName)
{
  std::string path;
  char* pathC = std::getenv(envVar.c_str());
  if (pathC) {
    path = std::string(pathC);
  } else {
    pathC = std::getenv("MRB_BUILDDIR");
    if (pathC) {
      path = std::string(pathC) + "/" + pkgName;
    } else {
      throw cet::exception("NOBASEPATH")
        << "Need enviornment variable " << envVar << " or $MRB_BUILDDIR set to find path\n";
    }
  }

  return path;
}
