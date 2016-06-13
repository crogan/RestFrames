/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
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
#include <TH1D.h>
#include <TH2D.h>

#include "RestFrames/RFPlot.hh"

namespace RestFrames {

  class HistPlotVar;
  class HistPlotCategory;

  class HistPlot : public RFPlot {

  public:
    HistPlot(const std::string& sname, const std::string& stitle);
    ~HistPlot();

    virtual void Clear();

    HistPlotVar const& GetNewVar(const std::string& name, const std::string& title, 
				 double minval, double maxval,
				 const std::string& unit = "");

    HistPlotCategory const& GetNewCategory(const std::string& name, const std::string& title);
    
    void AddPlot(const HistPlotVar& var, 
		 RestFrames::RFList<const HistPlotCategory> cats = 
		 RFList<const HistPlotCategory>(),
		 bool invert_colors = false);
    void AddPlot(const HistPlotVar& varX,
		 const HistPlotVar& varY,
		 RestFrames::RFList<const HistPlotCategory> cats =
		 RFList<const HistPlotCategory>(),
		 bool invert_colors = false);
    
    void Fill(double weight = 1.);
    void Fill(const HistPlotCategory& cat, double weight = 1.);

    void Draw(bool invert_colors = false);

    void SetPlotLabel(const std::string& label);

    void SetPlotTitle(const std::string& title);

    void SetScaleLabel(const std::string& label);

    void SetScale(double scale = -1);

    void SetRebin(int rebin = 4);

    void WriteHist(const std::string& filename);

  private:
    std::string m_PlotLabel;
    std::string m_PlotTitle;
    std::string m_ScaleLabel;
    double m_Scale;
    bool   m_SetScale;
    int    m_Rebin;
    
    std::vector<TH1D*> m_1DHists;
    std::vector<TH2D*> m_2DHists;
    // std::vector<HistPlotVar*>      m_Vars;
    // std::vector<HistPlotCategory*> m_Cats;
    HistPlotVarList m_Vars;
    HistPlotCatList m_Cats;
    std::map<const HistPlotCategory*,std::vector<TH1D*> > m_CatToHist1D;
    std::map<const HistPlotCategory*,std::vector<TH2D*> > m_CatToHist2D;
    
    std::vector<const HistPlotVar*>             m_Plot1D_Var;
    std::vector<HistPlotCatList>                m_Plot1D_Cats;
    std::vector<bool>                           m_Plot1D_Color;
    std::vector<std::pair<const HistPlotVar*,
			  const HistPlotVar*> > m_Plot2D_Vars;
    std::vector<const HistPlotCategory*>        m_Plot2D_Cat;
    std::vector<bool>                           m_Plot2D_Color;

    std::map<TH1D*,const HistPlotVar*>             m_HistToVar;
    std::map<TH2D*,std::pair<const HistPlotVar*,
			     const HistPlotVar*> > m_HistToVars;

    void DrawPlot(const HistPlotVar& var, 
		  const HistPlotCatList& cats,
		  bool invert_colors = false);
    void DrawPlot(const std::pair<const HistPlotVar*, 
		  const HistPlotVar*>& vars,
		  const HistPlotCategory& cat,
		  bool invert_colors = false);

  };

}

#endif
