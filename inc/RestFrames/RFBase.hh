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

// adapted from boost/current_function.hpp
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
# define RF_FUNCTION __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
# define RF_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
# define RF_FUNCTION __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
# define RF_FUNCTION __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
# define RF_FUNCTION __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
# define RF_FUNCTION __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
# define RF_FUNCTION __func__
#else
# define RF_FUNCTION "(unknown)"
#endif

#include <iostream>
#include <string>
#include <TVector3.h>
#include <TLorentzVector.h>
#include "RestFrames/RFList.hh"
#include "RestFrames/RFKey.hh"
#include "RestFrames/RFLog.hh"

using namespace std;

namespace RestFrames {

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

    RFBase();
    
    virtual ~RFBase();

    /// \brief Clears RFBase of all connections to other objects
    virtual void Clear();

    /// \brief Checks whether this is default (empty) instance of class
    bool IsEmpty() const;

    /// \brief Tests whether key is the same as this
    bool operator !() const { return IsEmpty(); }

    ////////////////////////////////////////////////////////////////////
    /// \name RFBase identity/comparison methods
    /// \brief RFBase identity query member functions
    /// 
    /// Member functions for identifying/comparing class instances
    ////////////////////////////////////////////////////////////////////
    ///@{
    
    /// \brief sets object identification key
    void SetKey(int key);

    /// \brief sets object identification key
    void SetKey(const RFKey& key);

    /// \brief gets object identification key
    RFKey GetKey() const;

    /// \brief Returns object name 
    string GetName() const;
    
    /// \brief Returns object title 
    string GetTitle() const;
    
    /// \brief Tests whether key is the same as this
    bool IsSame(const RFKey& key) const;

    /// \brief Tests whether *obj* is the same as this
    bool IsSame(const RFBase& obj) const;
    
    /// \brief Tests whether key is the same as this
    bool operator==(const RFKey& key) const { return IsSame(key); }

    /// \brief Tests whether *obj* is the same as this
    bool operator==(const RFBase& obj) const { return IsSame(obj); }

    /// \brief Tests whether key is the same as this
    bool operator!=(const RFKey& key) const { return !IsSame(key); }

    /// \brief Tests whether *obj* is the same as this
    bool operator!=(const RFBase& obj) const { return !IsSame(obj); }

    ///@} // end identity/comparison methods

    /// \brief Print information associated with object
    void Print(LogType type = LogVerbose) const;

    /// \brief String of information associated with object
    virtual string PrintString(LogType type = LogVerbose) const;

    /// \brief pointer to RFBase object owned by this one
    void AddDependent(RFBase* dep);

  protected:    
    mutable bool m_Body;       
    mutable bool m_Mind;       
    mutable bool m_Spirit;   
    mutable RFLog* m_LogPtr;

    bool SetBody(bool body) const;
    bool SetMind(bool mind) const;
    bool SetSpirit(bool spirit) const;

    virtual bool IsSoundBody() const;
    virtual bool IsSoundMind() const;
    virtual bool IsSoundSpirit() const;

    void UnSoundBody(const string& function) const;
    void UnSoundMind(const string& function) const;
    void UnSoundSpirit(const string& function) const;

    string m_Name;
    string m_Title;
    RFKey m_Key;

  private:
    void Init(const string& sname, const string& stitle);
    vector<RFBase*> m_Owns;
    
  };

}

#endif
