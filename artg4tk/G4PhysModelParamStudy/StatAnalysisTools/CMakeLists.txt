cet_make_library(NO_INSTALL
  SOURCE Chi2Calc.cc
  LIBRARIES
  PUBLIC ROOT::Core
  PRIVATE ROOT::Hist ROOT::MathCore)
