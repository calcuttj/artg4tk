// classes.h

#include <map>
#include <string>
#include "TObject.h"

#include "art/Persistency/Common/Wrapper.h"

// For StringIDs
#include "artg4tk/pluginActions/physicalVolumeStore/PhysicalVolumeStoreData.hh"
template class art::Wrapper<artg4tk::PhysicalVolumeStoreData>;
template class art::Wrapper<artg4tk::StringIDs>;
