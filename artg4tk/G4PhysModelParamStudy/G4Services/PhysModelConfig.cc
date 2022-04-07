// Authors: Julia Yarba

// Date: Oct. 2015

#include "artg4tk/G4PhysModelParamStudy/G4Components/ModelConfigMapper.hh"
#include "artg4tk/G4PhysModelParamStudy/G4Services/PhysModelConfig_service.hh"

artg4tk::PhysModelConfigService::PhysModelConfigService(const fhicl::ParameterSet& pset,
                                                        art::ActivityRegistry&)
{

  fConfigMapper = new ModelConfigMapper();
  fVerbosity = pset.get<bool>("Verbosity", false);
  if (fVerbosity) {
    // FIXME !!!
    // Need to find out a more "automated" way to handle it...
    //
    /*
          fConfigMapper->PrintDefaults("precompound");
          fConfigMapper->PrintDefaults("bertini");
    // --> too much output this way; better do only if model's verbosty is requested
    fConfigMapper->SetVerbosity("bertini",fVerbosity);
          // ---> for future --> fConfigMapper->PrintDefaults("inclxx");
          fConfigMapper->PrintDefaults("ftfp");
    */
    PrintDefaultSettings();
  }
}

artg4tk::PhysModelConfigService::~PhysModelConfigService()
{

  delete fConfigMapper;
}

void
artg4tk::PhysModelConfigService::PrintDefaultSettings()
{

  fConfigMapper->PrintDefaults("precompound");
  fConfigMapper->PrintDefaults("bertini");
  // inclxx in the future...
  fConfigMapper->PrintDefaults("ftfp");

  return;
}

void
artg4tk::PhysModelConfigService::ConfigureModel(const std::string& mname,
                                                const fhicl::ParameterSet& mpset)
{

  std::cout << " Entering PhysModelConfigService::ConfigureModel for " << mname << std::endl;

  // NOTE (JVY): This logic assumes that the model's verbosity key with within the model config.
  //             This is the case when using a physics list where multiple models can be configured.
  //             But NOT the case when running at the "process level" where the verbosity key is
  //             OUTSIDE of the model configuration specs and rather applies to the whole module.
  //             FIXME !!!
  //             Need to find a transparent solution !!!
  //             Using the verbosity key at the level of CENTRAL config service will work but it'll
  //             make the models (or at least Bertini) way to noisy.

  std::string mod = mname;
  mod = fConfigMapper->ToLower(mod);

  // this is a "hack" and kind of "violation" of the PSet philosophy but...
  //
  std::vector<std::string> keys = mpset.get_names(); // no loger: get_keys()
  for (unsigned int i = 0; i < keys.size(); ++i) {
    std::string thekey = keys[i];

    thekey = fConfigMapper->ToLower(thekey);

    // NOTE(JVY): there's a difference between "general" verbosity and
    //            specific Bertini verbosity (which is "too verbose");
    //            hence, the 2 different keys...
    //
    if (thekey == "verbosity") {
      fConfigMapper->SetVerbosity(mod, mpset.get<bool>(keys[i], false));
      continue;
    }
    if (thekey == "underlyingmodels") {
      fhicl::ParameterSet umpset = mpset.get<fhicl::ParameterSet>(keys[i]);
      std::vector<std::string> ukeys = umpset.get_names();
      for (unsigned int iu = 0; iu < ukeys.size(); ++iu) {
        ConfigureModel(ukeys[iu], umpset.get<fhicl::ParameterSet>(ukeys[iu]));
      }
      continue;
    }
    DoConfigure(mod, keys[i], mpset);
  }

  return;
}

void
artg4tk::PhysModelConfigService::Reset()
{

  fConfigMapper->RestoreAllDefaults();
  return;
}

void
artg4tk::PhysModelConfigService::PrintCurrentSettings()
{

  fConfigMapper->PrintCurrentSettings();
  return;
}

void
artg4tk::PhysModelConfigService::DoConfigure(const std::string& mod,
                                             const std::string& key,
                                             const fhicl::ParameterSet& mpset)
{

  std::string thekey = key;
  thekey = fConfigMapper->ToLower(thekey);

  if (thekey.find("use") != std::string::npos) {
    // e.g. Bertini itself treats parameters that contain "use" as integers (not bools);
    // they can be changed only "by value"
    //
    int value = mpset.get<int>(key, 0);
    fConfigMapper->ChangeParameter(mod, thekey, value, fVerbosity);
  } else {
    // Parameters that do NOT contain "use" can treated as doubles (doesn't matter
    // what e.g. Bertini "thinks" internally), and can be changed "by value" or "by ratio"
    //
    double value = mpset.get<double>(key);
    if (thekey.find("byratio") != std::string::npos) {
      //  change by ratio
      fConfigMapper->ChangeParameterByRatio(mod, thekey, value, fVerbosity);
    } else {
      // change by value
      fConfigMapper->ChangeParameter(mod, thekey, value, fVerbosity);
    }
  }

  return;
}
