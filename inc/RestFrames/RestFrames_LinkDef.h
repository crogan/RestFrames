#ifdef __ROOTCLING__

#pragma link C++ class RestFrames::RestFrame;
#pragma link C++ class RestFrames::RestFrameList;
#pragma link C++ class RestFrames::LabFrame;
#pragma link C++ class RestFrames::DecayFrame;
#pragma link C++ class RestFrames::InvisibleFrame;
#pragma link C++ class RestFrames::VisibleFrame;

#pragma link C++ class RestFrames::GFrame;
#pragma link C++ class RestFrames::GLabFrame;
#pragma link C++ class RestFrames::GDecayFrame;
#pragma link C++ class RestFrames::GInvisibleFrame;
#pragma link C++ class RestFrames::GVisibleFrame;

#pragma link C++ class RestFrames::RFrame;
#pragma link C++ class RestFrames::RLabFrame;
#pragma link C++ class RestFrames::RDecayFrame;
#pragma link C++ class RestFrames::RInvisibleFrame;
#pragma link C++ class RestFrames::RVisibleFrame;
#pragma link C++ class RestFrames::RSelfAssemblingFrame;

#pragma link C++ enum RestFrames::FrameType;
#pragma link C++ class std::vector<RestFrames::FrameType>;
#pragma link C++ class RestFrames::FrameLink;

#pragma link C++ class RestFrames::Group;
#pragma link C++ class RestFrames::GroupList;
#pragma link C++ class RestFrames::InvisibleGroup;
#pragma link C++ class RestFrames::CombinatoricGroup;

#pragma link C++ class RestFrames::Jigsaw;
#pragma link C++ class RestFrames::JigsawList;

#pragma link C++ class RestFrames::InvisibleJigsaw;
#pragma link C++ class RestFrames::InvisibleMassJigsaw;
#pragma link C++ class RestFrames::InvisibleRapidityJigsaw;
#pragma link C++ class RestFrames::NVisibleMInvisibleJigsaw;
#pragma link C++ class RestFrames::ContraBoostInvariantJigsaw;

#pragma link C++ class RestFrames::CombinatoricJigsaw;
#pragma link C++ class RestFrames::MinimizeMassesCombinatoricJigsaw;

#pragma link C++ class RestFrames::State;
#pragma link C++ class RestFrames::StateList;
#pragma link C++ class RestFrames::InvisibleState;
#pragma link C++ class RestFrames::CombinatoricState;

#pragma link C++ class RestFrames::FramePlot;
#pragma link C++ class RestFrames::FramePlotNode;
#pragma link C++ class RestFrames::FramePlotLink;

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

