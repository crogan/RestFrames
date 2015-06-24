/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RFBase.hh
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Jun
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

#ifndef RFBase_HH
#define RFBase_HH
#include <string>
#include <TVector3.h>
#include <TLorentzVector.h>
#include "RestFrames/FrameLog.hh"
#include "RestFrames/RFList.hh"

using namespace std;

namespace RestFrames {

  class FrameLog;

  ////////////////////////////////////////////////////////////////////
  /// \brief Base class for all RestFrame package objects
  ///
  /// Abstract base class from which all RestFrame package objects
  /// inherit. 
  ////////////////////////////////////////////////////////////////////
  class RFBase {
  
  public:
    
    ////////////////////////////////////////////////////////////////////
    /// \brief Standard constructor
    /// 
    /// \param sname    class instance name used for log statements
    /// \param stitle   class instance title used in figures
    ////////////////////////////////////////////////////////////////////
    RFBase(const string& sname, const string& stitle);
    
    virtual ~RFBase();

    ////////////////////////////////////////////////////////////////////
    /// \name RestFrame identity/comparison methods
    /// \brief RestFrame identity query member functions
    /// 
    /// Member functions for identifying/comparing class instances
    ////////////////////////////////////////////////////////////////////
    ///@{
    
    /// \brief sets object identification key
    void SetKey(int key);

    /// \brief Returns object identification key
    int GetKey() const;
    
    /// \brief Returns object name 
    string GetName() const;
    
    /// \brief Returns object title 
    string GetTitle() const;
    
    /// \brief Tests whether *obj* is the same as this
    bool IsSame(const RFBase& obj) const;
    
    /// \brief Tests whether *objPtr* points to this
    bool IsSame(const RFBase* objPtr) const;
    
    ///@} // end identity/comparison methods

    /// \brief Print information associated with object
    void Print(LogType type = LogVerbose) const;

    /// \brief String of information associated with object
    virtual string PrintString(LogType type = LogVerbose) const;

  protected:    
    mutable bool m_Body;       
    mutable bool m_Mind;       
    mutable bool m_Spirit;   
    mutable FrameLog* m_LogPtr;

    void SetBody(bool body) const;
    void SetMind(bool mind) const;
    void SetSpirit(bool spirit) const;

    virtual bool IsSoundBody() const;
    virtual bool IsSoundMind() const;
    virtual bool IsSoundSpirit() const;

    string m_Name;
    string m_Title;
    int m_Key;

  private:
    void Init(const string& sname, const string& stitle);
    
  };

}

#endif
