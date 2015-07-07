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
  
  ///////////////////////////////////////////////
  // RFKey class
  ///////////////////////////////////////////////
  class RFKey {

  public:
    RFKey(){ }
    RFKey(int key){ m_Key = key; }
    RFKey(const RFKey& key){ m_Key = key.GetKey(); }
    ~RFKey(){ }

    void operator=(const RFKey& key){ m_Key = key.GetKey(); }

    bool operator==(const RFKey& key) const { return (m_Key == key.GetKey()); }

    void SetKey(int key){ m_Key = key; }

    int GetKey() const { return m_Key; }

    bool IsSame(const RFKey& key) const {
      return m_Key == key.GetKey();
    }

  private:
    int m_Key;

  };
 
  extern RFKey g_Key;

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
  // RFList class
  ///////////////////////////////////////////////
  template <class T>
  class RFList {
  public:
    RFList();
    virtual ~RFList();
  
    bool Add(T& obj);
    bool Add(const RFList<T>& objs);
    int Remove(const T& obj);
    void Remove(const RFList<T>& objs);
    void Clear();
    int GetN() const { return m_Objs.size(); }
    T& Get(int i) const;
    T& Get(const RFKey& key) const;
    int GetIndex(const RFKey& key) const;
    int GetIndex(const T& obj) const;
    bool Contains(const RFKey& key) const;
    bool Contains(const T& obj) const;
    bool Contains(const RFList<T>& objs) const;

    bool IsSame(const RFList<T>& objs) const;
    RFList<T> Copy() const;
    RFList<T> Union(const RFList<T>& objs) const;
    RFList<T> Intersection(const RFList<T>& objs) const;
    RFList<T> Complement(const RFList<T>& objs) const;
    int SizeUnion(const RFList<T>& objs) const;
    int SizeIntersection(const RFList<T>& objs) const;
    int SizeComplement(const RFList<T>& objs) const;

  protected:
    vector<T*> m_Objs;
    static RFListEmpty<T> m_EmptyHandler;
    static T& m_Empty;

  };

}

#endif
