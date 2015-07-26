#ifndef TreePlotLink_HH
#define TreePlotLink_HH

#include <string>

using namespace std;

namespace RestFrames {
  
  class TreePlotNode;
  class Jigsaw;

  class TreePlotLink {
  public:
    TreePlotLink(TreePlotNode* Node1Ptr, TreePlotNode* Node2Ptr);
    ~TreePlotLink();

    void SetLabel(const string& label);
    void SetWavy(bool wavy);
    void SetJigsaw(const Jigsaw& jigsaw);

    TreePlotNode* GetNode1() const;
    TreePlotNode* GetNode2() const;
    bool DoWavy() const;
    string GetLabel() const;
    bool DoLabel() const;
    Jigsaw const& GetJigsaw() const;

  private:
    TreePlotNode* m_Node1Ptr;
    TreePlotNode* m_Node2Ptr;
    bool m_Wavy;
    bool m_DoLabel;
    string m_Label;
    const Jigsaw* m_JigsawPtr;
   
  };

}

#endif
