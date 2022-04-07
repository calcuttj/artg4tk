cet_make_library(SOURCE GenParticle.cc
  LIBRARIES PUBLIC CLHEP::Vector canvas::canvas)
art_dictionary(DICTIONARY_LIBRARIES
  artg4tk::DataProducts_EventGenerators)

install_headers()
install_source()
