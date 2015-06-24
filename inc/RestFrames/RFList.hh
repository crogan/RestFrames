#ifndef RFList_HH
#define RFList_HH
#include <iostream>
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
