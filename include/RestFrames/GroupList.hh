#ifndef GroupList_HH
#define GroupList_HH
#include <iostream>
#include <string>
#include <vector>
#include "RestFrames/Group.hh"

using namespace std;

namespace RestFrames {

  class Group;

  ///////////////////////////////////////////////
  // GroupList class
  ///////////////////////////////////////////////
  class GroupList {
  public:
    GroupList();
    ~GroupList();
  
    void Add(Group* groupPtr);
    void Add(GroupList* groupsPtr);
    void Remove(const Group* groupPtr);
    void Remove(const GroupList* groupsPtr);
    void Clear();
    int GetN() const { return m_Groups.size(); }
    Group* Get(int i) const;
    bool Contains(const Group* groupPtr) const;
    GroupList* Copy() const;

  protected:
    vector<Group*> m_Groups;

  };

}

#endif
