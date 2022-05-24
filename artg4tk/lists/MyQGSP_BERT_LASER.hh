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
//
//---------------------------------------------------------------------------
//
// ClassName:   MyQGSP_BERT_LASER
//
// Author: 2002 J.P. Wellisch
//
// Modified:
// 05.01.2022 D.Rivera: imported into artg4tk based on :
//            geant4.10.06.p01d/source/physics_lists/lists/include/QGSP_BERT.hh
//
//----------------------------------------------------------------------------
//
#ifndef MyQGSP_BERT_LASER_h
#define MyQGSP_BERT_LASER_h 1

#include "Geant4/globals.hh"
#include "Geant4/G4VModularPhysicsList.hh"

class MyQGSP_BERT_LASER: public G4VModularPhysicsList
{
public:
  MyQGSP_BERT_LASER(G4int ver = 1);
  virtual ~MyQGSP_BERT_LASER()=default;

  MyQGSP_BERT_LASER(const MyQGSP_BERT_LASER &) = delete;
  MyQGSP_BERT_LASER & operator=(const MyQGSP_BERT_LASER &)=delete;
  
};

#endif



