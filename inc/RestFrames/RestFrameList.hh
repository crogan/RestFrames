#ifndef RestFrameList_HH
#define RestFrameList_HH
#include <iostream>
#include <vector>
#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;

  ///////////////////////////////////////////////
  // RestFrameList class
  ///////////////////////////////////////////////
  class RestFrameList {
  public:
    RestFrameList();
    ~RestFrameList();
  
    void Add(RestFrame* framePtr);
    void Add(RestFrameList* framesPtr);
    int Remove(const RestFrame* framePtr);
    void Remove(const RestFrameList* framesPtr);
    void Clear();
    int GetN() const { return m_Frames.size(); }
    RestFrame* Get(int i) const;
    int GetIndex(const RestFrame* framePtr) const;
    bool Contains(const RestFrame* framePtr) const;
    bool Contains(const RestFrameList* framesPtr) const;
    bool IsSame(const RestFrameList* framesPtr) const;
    RestFrameList* Copy() const;
    RestFrameList* Union(const RestFrameList* framesPtr) const;
    RestFrameList* Intersection(const RestFrameList* framesPtr) const;
    RestFrameList* Complement(const RestFrameList* framesPtr) const;
    int SizeUnion(const RestFrameList* framesPtr) const;
    int SizeIntersection(const RestFrameList* framesPtr) const;
    int SizeComplement(const RestFrameList* framesPtr) const;

  protected:
    vector<RestFrame*> m_Frames;
  };

}

#endif
