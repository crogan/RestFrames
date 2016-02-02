/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   GeneratorFrame.hh
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

#ifndef GeneratorFrame_HH
#define GeneratorFrame_HH

#include <TRandom.h>
#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GeneratorFrame class
  ///////////////////////////////////////////////
  class GeneratorFrame : public RestFrame {
  public:
    GeneratorFrame(const string& sname, const string& stitle);
    GeneratorFrame();
    virtual ~GeneratorFrame();

    virtual void Clear();

    static GeneratorFrame& Empty();

    virtual bool InitializeAnalysisRecursive();
    virtual bool ClearEventRecursive();
    virtual bool AnalyzeEventRecursive();

    /// \brief Add a child RestFrame to this frame
    ///
    /// \param frame    RestFrame to be added as child
    ///
    /// Method for adding a RestFrame *frame* as a child 
    /// of this frame. *frame* will not be added as a child
    /// if it is already listed as a child.
    virtual void AddChildFrame(RestFrame& frame);

    /// \brief Set the parent frame for this frame
    ///
    /// \param frame     parent frame
    ///
    /// Method for connecting a child frame to its parent frame
    /// Empty default sets parent frame to none
    virtual void SetParentFrame(RestFrame& frame = 
				RestFrame::Empty());

    /// \brief Returns the parent of this frame
    ///
    /// Returns the parent frame of this frame.
    /// If the parent frame is not set, an empty
    /// frame is returned.
    virtual GeneratorFrame const& GetParentFrame() const;

    /// \brief Get the frame of the *i* th child
    virtual GeneratorFrame& GetChildFrame(int i) const;

    virtual double GetMass() const;

    bool IsVariableMassMCMC() const;
    virtual double GetMinimumMassMCMC() const;
    virtual void GenerateMassMCMC(double& mass, double& prob, 
				  double max = -1.) const;
    virtual double GetProbMCMC(double mass = -1.) const;

  protected:
    double m_Mass;

    virtual void ResetFrame() {}
    virtual bool GenerateFrame() { return false; }

    void SetChildren(const vector<TLorentzVector>& P_children);
    virtual bool InitializeGenAnalysis();

    double GetRandom() const;
    double GetGaus(double mu, double sig) const;

    virtual bool IterateMCMC();
    bool IterateRecursiveMCMC();

    void SetVariableMassMCMC(bool var = true);
    virtual void SetMassMCMC(double mass);
    void SetMassMCMC(double mass, GeneratorFrame& frame) const;

  private:
    void Init();
    TRandom *m_Random;
    bool m_VarMassMCMC;
   
  };

}

#endif
