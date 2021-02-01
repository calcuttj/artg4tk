//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: MyQGSP_BERT_HP.hh 66892 2019-10-10 10:57:59Z drivera $
//
//---------------------------------------------------------------------------
//
// ClassName:   MyQGSP_BERT_HP
//
// Author: 2002 J.P. Wellisch
//
// Modified: 2019 D. Rivera
//
//----------------------------------------------------------------------------
//
#ifndef TMyQGSP_BERT_HP_NeutronXSBias_h
#define TMyQGSP_BERT_HP_NeutronXSBias_h 1

#include <CLHEP/Units/SystemOfUnits.h>

#include "Geant4/globals.hh"
#include "Geant4/G4VModularPhysicsList.hh"
#include "Geant4/CompileTimeConstraints.hh"

template<class T>
class TMyQGSP_BERT_HP_NeutronXSBias: public T
{
public:
  TMyQGSP_BERT_HP_NeutronXSBias(G4int ver=1);
  virtual ~TMyQGSP_BERT_HP_NeutronXSBias();

public:
  // SetCuts()
  virtual void SetCuts();

private:
  enum {ok = CompileTimeConstraints::IsA<T, G4VModularPhysicsList>::ok };
};

#include "artg4tk/lists/MyQGSP_BERT_HP_NeutronXSBias.icc"
typedef TMyQGSP_BERT_HP_NeutronXSBias<G4VModularPhysicsList> MyQGSP_BERT_HP_NeutronXSBias;

// 2019 by D. Rivera

#endif



