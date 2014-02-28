// classes.h

#include <map>
#include <string>
#include "TObject.h"

#include "art/Persistency/Common/Wrapper.h"

// For StringIDs
#include "artg4tk/pluginActions/writeGdml/gdmlText.hh"

template class art::Wrapper<artg4tk::GdmlText>;
