#ifndef TreePlotNode_HH
#define TreePlotNode_HH

#include <string>
#include "RestFrames/RFList.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class Jigsaw;
  class State;

  class TreePlotNode {
  public:
    TreePlotNode();
    ~TreePlotNode();
    
    void SetX(double x);
    void SetY(double y);
    void SetLabel(const string& label);
    void SetSquare(bool square);
    void SetFrame(const RestFrame& frame);
    void SetState(const State& state);
    void AddJigsaw(const Jigsaw& jigsaw);
  
    double GetX() const;
    double GetY() const;
    bool DoLabel() const;
    bool DoSquare() const;
    string GetLabel() const;
    RestFrame const& GetFrame() const;
    int GetNJigsaws() const;
    RestFrames::RFList<const Jigsaw> GetJigsawList() const;
    State const& GetState() const;

  private:
    double m_X;
    double m_Y;
    string m_Label;
    bool m_DoLabel;
    bool m_DoSquare;
    const RestFrame* m_FramePtr;
    RestFrames::RFList<const Jigsaw> m_Jigsaws;
    const State* m_StatePtr;
    void Init();
  };

}

#endif
