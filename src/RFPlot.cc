/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RFPlot.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Jan
///
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

#include <TFile.h>

#include "RestFrames/RFPlot.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RFPlot class methods
  // class which can plot RestFrame trees
  ///////////////////////////////////////////////
  RFPlot::RFPlot(const string& sname, const string& stitle)
    : RFBase(sname, stitle, -1){
    m_Log.SetSource("RFPlot "+GetName());
  }
  
  RFPlot::~RFPlot(){
    Clear();
  }

  void RFPlot::Clear(){
    int Ncan = m_Canvases.size();
    for(int i = 0; i < Ncan; i++)
      delete m_Canvases[i];
    m_Canvases.clear();
    int Nobj = m_TObjects.size();
    for(int i = 0; i < Nobj; i++)
      delete m_TObjects[i];
    m_TObjects.clear();
    RFBase::Clear();
  }

  void RFPlot::WriteOutput(const string& filename){
    int N = m_Canvases.size();
    if(N <= 0) return;

    TFile* file = new TFile(filename.c_str(),"UPDATE");
    file->mkdir(GetName().c_str());
    file->cd(GetName().c_str());
    for(int i = 0; i < N; i++)
      m_Canvases[i]->Write("",TObject::kOverwrite);
    file->Close();
    delete file;
  }

  void RFPlot::AddCanvas(TCanvas* can){
    m_Canvases.push_back(can);
  }

  void RFPlot::AddTObject(TObject* obj){
    m_TObjects.push_back(obj);
  }

  int RFPlot::GetNCanvases(){
    return m_Canvases.size();
  }
}
