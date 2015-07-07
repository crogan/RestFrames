#ifdef __ROOTCLING__
#include "RestFrames/RFLog.hh"
#include "RestFrames/RestFrames.hh"
#pragma link C++ namespace RestFrames;
using namespace RestFrames;

#pragma link C++ class RFBase;
#pragma link C++ class RFKey;

#pragma link C++ class RFList<RFBase>;
#pragma link C++ class RFList<RestFrame>; 
#pragma link C++ class RFList<RFrame>; 
#pragma link C++ class RFList<Jigsaw>;
#pragma link C++ class RFList<Group>;
#pragma link C++ class RFList<State>;
#pragma link C++ class RFList<InvisibleState>;
#pragma link C++ class RFList<CombinatoricState>;

#pragma link C++ class RFLog;

#pragma link C++ class RestFrame;
#pragma link C++ class LabFrame;
#pragma link C++ class DecayFrame;
#pragma link C++ class InvisibleFrame;
#pragma link C++ class VisibleFrame;

#pragma link C++ class GFrame;
#pragma link C++ class GLabFrame;
#pragma link C++ class GDecayFrame;
#pragma link C++ class GInvisibleFrame;
#pragma link C++ class GVisibleFrame;

#pragma link C++ class RFrame;
#pragma link C++ class RLabFrame;
#pragma link C++ class RDecayFrame;
#pragma link C++ class RInvisibleFrame;
#pragma link C++ class RVisibleFrame;
#pragma link C++ class RSelfAssemblingFrame;

#pragma link C++ enum FrameType;
#pragma link C++ class std::vector<FrameType>;

#pragma link C++ enum LogType;
#pragma link C++ class std::map<LogType,bool>;

#pragma link C++ class Group;
#pragma link C++ class InvisibleGroup;
#pragma link C++ class CombinatoricGroup;

#pragma link C++ class Jigsaw;

#pragma link C++ class InvisibleJigsaw;
#pragma link C++ class InvisibleMassJigsaw;
#pragma link C++ class InvisibleRapidityJigsaw;
#pragma link C++ class ContraBoostInvariantJigsaw;

#pragma link C++ class CombinatoricJigsaw;
#pragma link C++ class MinimizeMassesCombinatoricJigsaw;

#pragma link C++ class State;
#pragma link C++ class InvisibleState;
#pragma link C++ class CombinatoricState;

#pragma link C++ class FramePlot;
#pragma link C++ class FramePlotNode;
#pragma link C++ class FramePlotLink;

#elif __MAKECINT__

#pragma link off all typedefs;
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace RestFrames;
using namespace RestFrames;

#pragma link C++ class RFBase+;
#pragma link C++ class RFKey+;
#pragma link C++ class RFList<RFBase>+;
#pragma link C++ class RFList<RestFrame>+; 
#pragma link C++ class RFList<RFrame>+; 
#pragma link C++ class RFList<Jigsaw>+;
#pragma link C++ class RFList<Group>+;
#pragma link C++ class RFList<State>+;
#pragma link C++ class RFList<InvisibleState>+;
#pragma link C++ class RFList<CombinatoricState>+;

#pragma link C++ class RFLog+;

#pragma link C++ class RestFrame+;
#pragma link C++ class LabFrame+;
#pragma link C++ class DecayFrame+;
#pragma link C++ class InvisibleFrame+;
#pragma link C++ class VisibleFrame+;

#pragma link C++ class GFrame+;
#pragma link C++ class GLabFrame+;
#pragma link C++ class GDecayFrame+;
#pragma link C++ class GInvisibleFrame+;
#pragma link C++ class GVisibleFrame+;

#pragma link C++ class RFrame+;
#pragma link C++ class RLabFrame+;
#pragma link C++ class RDecayFrame+;
#pragma link C++ class RInvisibleFrame+;
#pragma link C++ class RVisibleFrame+;
#pragma link C++ class RSelfAssemblingFrame+;

#pragma link C++ enum LogType+;
#pragma link C++ enum FrameType+;

#pragma link C++ class Group+;
#pragma link C++ class InvisibleGroup+;
#pragma link C++ class CombinatoricGroup+;

#pragma link C++ class Jigsaw+;

#pragma link C++ class InvisibleJigsaw+;
#pragma link C++ class InvisibleMassJigsaw+;
#pragma link C++ class InvisibleRapidityJigsaw+;
#pragma link C++ class ContraBoostInvariantJigsaw+;

#pragma link C++ class CombinatoricJigsaw+;
#pragma link C++ class MinimizeMassesCombinatoricJigsaw+;

#pragma link C++ class State+;
#pragma link C++ class InvisibleState+;
#pragma link C++ class CombinatoricState+;

#pragma link C++ class FramePlot+;
#pragma link C++ class FramePlotNode+;
#pragma link C++ class FramePlotLink+;

#endif /* __ROOTCLING__ and __CINT__ */

