
#include <TRootIOCtor.h>

#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/BeamThinTargetConfig.hh"

#include <cassert>

#include <iostream>

// ClassImp(BeamTargetConfig)

// artg4tk::
BeamThinTargetConfig::BeamThinTargetConfig( const int pid, const double m, const int tid )
   : TNamed()
{

   fBeamPartID   = pid;
   fBeamMomentum = m;
   fTargetID     = tid;

}

//artg4tk::
BeamThinTargetConfig::BeamThinTargetConfig( const BeamThinTargetConfig& rhs )
   : TNamed(rhs)
{

   fBeamPartID = rhs.fBeamPartID;
   fBeamMomentum = rhs.fBeamMomentum;
   fTargetID = rhs.fTargetID;

}

bool BeamThinTargetConfig::operator==( const BeamThinTargetConfig& rhs ) const
{

   return ( fBeamPartID==rhs.fBeamPartID && 
            fabs(fBeamMomentum-rhs.fBeamMomentum) < 1.e-10 && 
	    fTargetID==rhs.fTargetID );

}

//artg4tk::
BeamThinTargetConfig::BeamThinTargetConfig( TRootIOCtor* )
   : TNamed(), fBeamPartID(0), fBeamMomentum(-1.), fTargetID(0)
{
}

//artg4tk::
void BeamThinTargetConfig::Print( Option_t* ) const
{

   std::cout << " OBJ: BeamThinTargetConfig" << "\t" << GetName() << "\t" << GetTitle() << std::endl;
   
   std::cout << " Beam Particle ID = " << fBeamPartID << std::endl;
   std::cout << " Beam Momentum = " << fBeamMomentum << std::endl;
   std::cout << " Target ID (Z) = " << fTargetID << std::endl;
      
   return;

}
