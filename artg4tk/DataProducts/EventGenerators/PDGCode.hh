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
// PDGCode.hh: A minimal version of a convenience class to allow compile 
// time access to the PDG identifier codes.
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef DataProducts_EventGenerators_PDGCode_h
#define DataProducts_EventGenerators_PDGCode_h
//
// A minimal version of a convenience class to allow compile time access
// to the PDG identifier codes.
//

namespace artg4tk {

  class PDGCode {

  public:
    enum type
      {
        invalid  =    0 ,
        pi_plus  =  211 ,
        pi_minus = -211 ,
        K_plus   =  321 ,
        K_minus  = -321 ,
        phi      =  333 ,
        proton   =  2212

      };
  };

}

#endif /* DataProducts_PDGCode_h */
