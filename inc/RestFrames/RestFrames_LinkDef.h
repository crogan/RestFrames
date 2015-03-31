#ifdef __ROOTCLING__
#include "RestFrames/RestFrame.hh"
#pragma link C++ namespace RestFrames;
using namespace RestFrames;

#pragma link C++ class RestFrame;
#pragma link C++ class RestFrameList;
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
#pragma link C++ class FrameLink;

#pragma link C++ class Group;
#pragma link C++ class GroupList;
#pragma link C++ class InvisibleGroup;
#pragma link C++ class CombinatoricGroup;

#pragma link C++ class Jigsaw;
#pragma link C++ class JigsawList;

#pragma link C++ class InvisibleJigsaw;
#pragma link C++ class InvisibleMassJigsaw;
#pragma link C++ class InvisibleRapidityJigsaw;
#pragma link C++ class NVisibleMInvisibleJigsaw;
#pragma link C++ class ContraBoostInvariantJigsaw;

#pragma link C++ class CombinatoricJigsaw;
#pragma link C++ class MinimizeMassesCombinatoricJigsaw;

#pragma link C++ class State;
#pragma link C++ class StateList;
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

#pragma link C++ class RestFrame+;
#pragma link C++ class RestFrameList+;
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

#pragma link C++ enum FrameType+;
#pragma link C++ class std::vector<FrameType>+;
#pragma link C++ class FrameLink+;

#pragma link C++ class Group+;
#pragma link C++ class GroupList+;
#pragma link C++ class InvisibleGroup+;
#pragma link C++ class CombinatoricGroup+;

#pragma link C++ class Jigsaw+;
#pragma link C++ class JigsawList+;

#pragma link C++ class InvisibleJigsaw+;
#pragma link C++ class InvisibleMassJigsaw+;
#pragma link C++ class InvisibleRapidityJigsaw+;
#pragma link C++ class NVisibleMInvisibleJigsaw+;
#pragma link C++ class ContraBoostInvariantJigsaw+;

#pragma link C++ class CombinatoricJigsaw+;
#pragma link C++ class MinimizeMassesCombinatoricJigsaw+;

#pragma link C++ class State+;
#pragma link C++ class StateList+;
#pragma link C++ class InvisibleState+;
#pragma link C++ class CombinatoricState+;

#pragma link C++ class FramePlot+;
#pragma link C++ class FramePlotNode+;
#pragma link C++ class FramePlotLink+;

#endif /* __ROOTCLING__ and __CINT__ */

