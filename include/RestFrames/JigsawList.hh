#ifndef JigsawList_HH
#define JigsawList_HH
#include <iostream>
#include <vector>
#include "RestFrames/Jigsaw.hh"

using namespace std;

namespace RestFrames {

  class Jigsaw;

  ///////////////////////////////////////////////
  // JigsawList class
  ///////////////////////////////////////////////
  class JigsawList {
  public:
    JigsawList();
    virtual ~JigsawList();
  
    void Clear();
    JigsawList* Copy() const;

    bool Add(Jigsaw& jigsaw);
    bool Add(Jigsaw* jigsawPtr);
    bool Add(JigsawList* jigsawsPtr);
    void Remove(const Jigsaw* jigsawPtr);
    void Remove(const JigsawList* jigsawsPtr);
    bool Contains(const Jigsaw* jigsawPtr) const;
    int GetIndex(const Jigsaw* jigsawPtr) const;

    int GetN() const { return m_Jigsaws.size(); }
    Jigsaw* Get(int i) const;

  protected:
    vector<Jigsaw*> m_Jigsaws;
  };

}

#endif
