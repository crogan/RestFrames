/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RFList.hh
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

#ifndef RFList_HH
#define RFList_HH

#include <vector>

using namespace std;

class TLorentzVector;
class TVector3;

namespace RestFrames {
  
  class RFKey;
  class RestFrame;
  class State;

  template <class T>
  class RFListEmpty {
  public:
    RFListEmpty(const RFKey& key);
    ~RFListEmpty();
    T& GetEmpty() const;
  private:
    T* m_Empty;
  };
    
  ///////////////////////////////////////////////
  // RFListBase class
  ///////////////////////////////////////////////
  template <class T, class Derived>
  class RFListBase {
  public:
    RFListBase(){ }
    virtual ~RFListBase(){ }
    
    bool Add(T& obj);
    bool Add(const Derived& objs);
    int Remove(const T& obj);
    void Remove(const Derived& objs);
    void Clear();
    int GetN() const { return m_Objs.size(); }
    T& Get(int i) const;
    T& Get(const RFKey& key) const;
    int GetIndex(const RFKey& key) const;
    int GetIndex(const T& obj) const;
    bool Contains(const RFKey& key) const;
    bool Contains(const T& obj) const;
    bool Contains(const Derived& objs) const;
    
    bool IsSame(const Derived& objs) const;
    Derived Copy() const;
    Derived Union(const Derived& objs) const;
    Derived Intersection(const Derived& objs) const;
    Derived Complement(const Derived& objs) const;
    int SizeUnion(const Derived& objs) const;
    int SizeIntersection(const Derived& objs) const;
    int SizeComplement(const Derived& objs) const;
    
    // operator overload methods
    void operator=(const Derived& objs);
    T& operator[](int i) const;
    bool operator==(const Derived& objs) const;
    Derived operator+(T& obj) const;
    Derived operator+(const Derived& objs) const;
    Derived operator-(const T& obj) const;
    Derived operator-(const Derived& objs) const;
    Derived& operator+=(T& obj);
    Derived& operator+=(const Derived& objs);
    Derived& operator-=(const T& obj);
    Derived& operator-=(const Derived& objs);
    
  protected:
    vector<T*> m_Objs;
    static RFListEmpty<T> m_EmptyHandler;
    static T& m_Empty;
  };
    
  ///////////////////////////////////////////////
  // RFList class
  ///////////////////////////////////////////////
  template <class T>
  class RFList : public RestFrames::RFListBase<T,RFList<T> > {
  public:
    RFList() : RFListBase<T,RFList<T> >() { }
    virtual ~RFList(){ }
  };
  
  template <>
  class RFList<RestFrames::State>
    : public RFListBase<State,RFList<RestFrames::State> > {
  public:
    RFList() : RFListBase<State,RFList<State> >() { }
    virtual ~RFList(){ }

    int GetIndexFrame(const RestFrame& frame) const;
    TLorentzVector GetFourVector() const;
    void Boost(const TVector3& B) const;
  };

  template <> 
  class RFList<RestFrames::RestFrame>
    : public RFListBase<RestFrame,RFList<RestFrames::RestFrame> > {
  public:
    RFList() : RFListBase<RestFrame,RFList<RestFrame> >() { }
    virtual ~RFList(){ }

    double GetMass() const;
    TLorentzVector GetFourVector() const;
    TLorentzVector GetFourVector(const RestFrame& frame) const;
    TLorentzVector GetVisibleFourVector() const;
    TLorentzVector GetVisibleFourVector(const RestFrame& frame) const;
    TLorentzVector GetInvisibleFourVector() const;
    TLorentzVector GetInvisibleFourVector(const RestFrame& frame) const;
    double GetEnergy(const RestFrame& frame) const;
    double GetMomentum(const RestFrame& frame) const;
  };

}

#endif
