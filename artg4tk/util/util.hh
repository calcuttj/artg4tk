// Utility functions

#ifndef artg4tk_util_util_hh
#define artg4tk_util_util_hh

#include <functional>
#include <string>
#include <vector>

#include "Geant4/G4SDManager.hh"

class G4LogicalVolume;
class G4VisAttributes;

namespace artg4tk {

  // This function either returns or creates an SD as necessary, based on
  // the string passed in. If the G4SDManager hasn't created an SD based on
  // that string before, it will now.
  // class T must be a class that inherits from G4VSensitive detector
  //(that's what the 2nd line enforces). If it doesn't, then the templated
  // class will not generate a method for you.

  // To get a new RingSD, put something *like* this line in your service:
  // RingSD* mysd = getSensitiveDetector<RingSD>("myRings");

  template <typename T>
  typename std::enable_if<std::is_base_of<G4VSensitiveDetector, T>::value, T*>::type
  getSensitiveDetector(G4String name)
  {
    T* SDptr = 0;
    G4SDManager* fSDM = G4SDManager::GetSDMpointer();
    if ((SDptr = static_cast<T*>(fSDM->FindSensitiveDetector(name))) == NULL)
      SDptr = new T(name);
    return SDptr;
  }

  // Set visual attributes
  void setVisAtts(G4LogicalVolume* lv, bool display, const std::vector<double>& rgba);
  void setVisAtts(G4LogicalVolume* lv,
                  bool display,
                  const std::vector<double>& rgba,
                  std::function<void(G4VisAttributes*)> func);

  // Put a number in a name
  std::string addNumberToName(const std::string& name, int number);

  // Find a file base path
  // Look for environment variable. If it is not set, then look for $MRB_BUILDDIR and add package
  // name
  std::string basePath(const std::string& envVar, const std::string& pkgName);
}

#endif /* artg4tk_util_util_hh */
