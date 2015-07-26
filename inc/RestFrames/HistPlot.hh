/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   HistPlot.hh
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Jul
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

#ifndef HistPlot_HH
#define HistPlot_HH

#include <utility>
#include <TLatex.h>
#include <TColor.h>
#include <TStyle.h>
#include <TH1D.h>
#include <TH2D.h>

#include "RestFrames/RFPlot.hh"

namespace RestFrames {

  class HistPlotVar;

  class HistPlot : public RFPlot {

  public:
    HistPlot(const string& sname, const string& stitle);
    ~HistPlot();

    virtual void Clear();

    HistPlotVar const& GetNewVar(const string& name, const string& title, 
				 double minval, double maxval);
    
    void AddHist(const HistPlotVar& var);

    void AddHist(const HistPlotVar& varX,
		 const HistPlotVar& varY);
    
    void Fill(double weight = 1.);

    void Draw();

    void SetPlotLabel(const string& label);

    void SetPlotTitle(const string& title);

    static void SetStyle();

  private:
    string m_PlotLabel;
    string m_PlotTitle;
    vector<TH1D*> m_1DHists;
    vector<TH2D*> m_2DHists;
    vector<HistPlotVar*> m_Vars;
    map<TH1D*,const HistPlotVar*>        m_HistToVar;
    map<TH2D*,pair<const HistPlotVar*,
		   const HistPlotVar*> > m_HistToVars;

    void DrawHist(TH1D* hist);
    void DrawHist(TH2D* hist);

    void ClearHist();

  };

}

#endif
