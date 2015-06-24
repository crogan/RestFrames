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

namespace RestFrames {

  class RestFrame;

  ///////////////////////////////////////////////
  // RFList class
  ///////////////////////////////////////////////
  template <class T>
  class RFList {
  public:
    RFList();
    virtual ~RFList();
  
    bool Add(T obj);
    bool Add(T* objPtr);
    bool Add(RFList<T>* objsPtr);
    int Remove(const T* objPtr);
    void Remove(const RFList<T>* objsPtr);
    void Clear();
    int GetN() const { return m_Objs.size(); }
    T* Get(int i) const;
    int GetIndex(const T* objPtr) const;
    bool Contains(const T* objPtr) const;
    bool Contains(const RFList<T>* objsPtr) const;

    bool IsSame(const RFList<T>* objsPtr) const;
    RFList<T>* Copy() const;
    RFList<T>* Union(const RFList<T>* objsPtr) const;
    RFList<T>* Intersection(const RFList<T>* objsPtr) const;
    RFList<T>* Complement(const RFList<T>* objsPtr) const;
    int SizeUnion(const RFList<T>* objsPtr) const;
    int SizeIntersection(const RFList<T>* objsPtr) const;
    int SizeComplement(const RFList<T>* objsPtr) const;

  protected:
    vector<T*> m_Objs;
  };

}

#endif
