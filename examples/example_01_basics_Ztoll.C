/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_01_basics_Ztoll.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 March
///
///  \brief A simple example analyzing a Z to ll event
//
//   This file is part of RestFrames.
//
//   RestFrames is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   RestFrames is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with RestFrames. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#include <TLorentzVector.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <iostream>
#include "RestFrames/RLabFrame.hh"
#include "RestFrames/RDecayFrame.hh"
#include "RestFrames/RVisibleFrame.hh"
#include "RestFrames/FramePlot.hh"

using namespace std;
using namespace RestFrames;

void example_01_basics_Ztoll(){

  cout << "We're analyzing some stuff!" << endl;

}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_01_basics_Ztoll();
  return 0;
}
#endif
