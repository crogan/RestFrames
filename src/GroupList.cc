#include "RestFrames/GroupList.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GroupList class methods
  ///////////////////////////////////////////////
  GroupList::GroupList(){ }

  GroupList::~GroupList(){
    Clear();
  }

  void GroupList::Clear(){
    m_Groups.clear();
  }

  Group* GroupList::Get(int i) const { 
    if(i < 0 || i >= GetN()) return nullptr;
    return m_Groups[i];
  }

  void GroupList::Add(Group* groupPtr){
    if(!groupPtr) return;
    if(!Contains(groupPtr)) m_Groups.push_back(groupPtr);
  }

  void GroupList::Add(GroupList* groupsPtr){
    if(!groupsPtr) return;
    int N = groupsPtr->GetN();
    for(int i = 0; i < N; i++) Add(groupsPtr->Get(i));
  }

  void GroupList::Remove(const Group* groupPtr){
    if(!groupPtr) return;
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(groupPtr->GetKey() == m_Groups[i]->GetKey()){
	m_Groups.erase(m_Groups.begin()+i);
	return;
      }
    }
  }

  void GroupList::Remove(const GroupList* groupsPtr){
    if(!groupsPtr) return;
    int N = groupsPtr->GetN();
    for(int i = 0; i < N; i++) Remove(groupsPtr->Get(i));
  }

  bool GroupList::Contains(const Group* groupPtr) const {
    if(!groupPtr) return false;
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(groupPtr->GetKey() == m_Groups[i]->GetKey()) return true;
    }
    return false;
  }

  GroupList* GroupList::Copy() const {
    GroupList* groupsPtr = new GroupList();
    int N = GetN();
    for(int i = 0; i < N; i++) groupsPtr->Add(m_Groups[i]);
    return groupsPtr;
  }

}
