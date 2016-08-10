#ifdef __ROOTCLING__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace RestFrames;
using namespace RestFrames;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;

#pragma link C++ enum RestFrames::LogType;
#pragma link C++ class RestFrames::RFLog;
#pragma link C++ class RestFrames::RFBase;
#pragma link C++ class RestFrames::RFKey;
#pragma link C++ class RestFrames::RFCharge;

#pragma link C++ class RestFrames::RFList<RFBase>;
#pragma link C++ class RestFrames::RFList<RestFrame>; 
#pragma link C++ class RestFrames::RFList<const RestFrame>; 
#pragma link C++ class RestFrames::RFList<ReconstructionFrame>;
#pragma link C++ class RestFrames::RFList<GeneratorFrame>;
#pragma link C++ class RestFrames::RFList<VisibleRecoFrame>;
#pragma link C++ class RestFrames::RFList<DecayRecoFrame>;
#pragma link C++ class RestFrames::RFList<ResonanceGenFrame>; 
#pragma link C++ class RestFrames::RFList<Jigsaw>;
#pragma link C++ class RestFrames::RFList<const Jigsaw>;
#pragma link C++ class RestFrames::RFList<Group>;
#pragma link C++ class RestFrames::RFList<const Group>;
#pragma link C++ class RestFrames::RFList<State>;
#pragma link C++ class RestFrames::RFList<VisibleState>;
#pragma link C++ class RestFrames::RFList<InvisibleState>;
#pragma link C++ class RestFrames::RFList<CombinatoricState>;
#pragma link C++ class RestFrames::RFList<HistPlotVar>;
#pragma link C++ class RestFrames::RFList<HistPlotCategory>;
#pragma link C++ class RestFrames::RFList<const HistPlotCategory>;

#pragma link C++ typedef RestFrames::RFBaseList;
#pragma link C++ typedef RestFrames::RestFrameList;
#pragma link C++ typedef RestFrames::ConstRestFrameList;
#pragma link C++ typedef RestFrames::GroupList;
#pragma link C++ typedef RestFrames::ConstGroupList;
#pragma link C++ typedef RestFrames::JigsawList;
#pragma link C++ typedef RestFrames::ConstJigsawList;
#pragma link C++ typedef RestFrames::StateList;
#pragma link C++ typedef RestFrames::VisibleStateList;
#pragma link C++ typedef RestFrames::HistPlotVarList;
#pragma link C++ typedef RestFrames::HistPlotCatList;

#pragma link C++ typedef RestFrames::RestFrameListList;
#pragma link C++ typedef RestFrames::StateListList;

#pragma link C++ class RestFrames::RestFrame;

#pragma link C++ class RestFrames::GeneratorFrame;
#pragma link C++ class RestFrames::LabGenFrame;
#pragma link C++ class RestFrames::DecayGenFrame;
#pragma link C++ class RestFrames::InvisibleGenFrame;
#pragma link C++ class RestFrames::VisibleGenFrame;
#pragma link C++ class RestFrames::ResonanceGenFrame;
#pragma link C++ class RestFrames::ppLabGenFrame;

#pragma link C++ class RestFrames::ReconstructionFrame;
#pragma link C++ class RestFrames::LabRecoFrame;
#pragma link C++ class RestFrames::DecayRecoFrame;
#pragma link C++ class RestFrames::InvisibleRecoFrame;
#pragma link C++ class RestFrames::VisibleRecoFrame;
#pragma link C++ class RestFrames::SelfAssemblingRecoFrame;

#pragma link C++ class+protected RestFrames::Group;
#pragma link C++ class RestFrames::InvisibleGroup;
#pragma link C++ class RestFrames::CombinatoricGroup;

#pragma link C++ class RestFrames::Jigsaw;

#pragma link C++ class RestFrames::InvisibleJigsaw;
#pragma link C++ class RestFrames::SetMassInvJigsaw;
#pragma link C++ class RestFrames::SetRapidityInvJigsaw;
#pragma link C++ class RestFrames::ContraBoostInvJigsaw;
#pragma link C++ class RestFrames::CombinedCBInvJigsaw;
#pragma link C++ class RestFrames::MaxProbBreitWignerInvJigsaw;
#pragma link C++ class RestFrames::MinMassDiffInvJigsaw;
#pragma link C++ class RestFrames::MinMassesSqInvJigsaw;

#pragma link C++ class RestFrames::CombinatoricJigsaw;
#pragma link C++ class RestFrames::MinMassesCombJigsaw;
#pragma link C++ class RestFrames::MaxProbBreitWignerCombJigsaw;
#pragma link C++ class RestFrames::MinMassChi2CombJigsaw;
#pragma link C++ class RestFrames::MinMassDiffCombJigsaw;
#pragma link C++ class RestFrames::MinMassesSqCombJigsaw;

#pragma link C++ class RestFrames::State;
#pragma link C++ class RestFrames::VisibleState;
#pragma link C++ class RestFrames::InvisibleState;
#pragma link C++ class RestFrames::CombinatoricState;

#pragma link C++ class RestFrames::RFPlot;
#pragma link C++ class RestFrames::TreePlot;
#pragma link C++ class RestFrames::TreePlotLink;
#pragma link C++ class RestFrames::TreePlotNode;
#pragma link C++ class RestFrames::HistPlot;
#pragma link C++ class RestFrames::HistPlotCategory;
#pragma link C++ class RestFrames::HistPlotVar;

#elif __MAKECINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace RestFrames;
using namespace RestFrames;

#pragma link C++ enum LogType;
#pragma link C++ class RFLog+;
#pragma link C++ class RFBase+;
#pragma link C++ class RFKey+;
#pragma link C++ class RFCharge+;

#pragma link C++ class RFList<RFBase>+;
#pragma link C++ class RFList<RestFrame>+; 
#pragma link C++ class RFList<const RestFrame>+; 
#pragma link C++ class RFList<ReconstructionFrame>+;
#pragma link C++ class RFList<GeneratorFrame>+;
#pragma link C++ class RFList<VisibleRecoFrame>+;
#pragma link C++ class RFList<DecayRecoFrame>+;
#pragma link C++ class RFList<ResonanceGenFrame>+; 
#pragma link C++ class RFList<Jigsaw>+;
#pragma link C++ class RFList<const Jigsaw>+;
#pragma link C++ class RFList<Group>+;
#pragma link C++ class RFList<const Group>+;
#pragma link C++ class RFList<State>+;
#pragma link C++ class RFList<VisibleState>+;
#pragma link C++ class RFList<InvisibleState>+;
#pragma link C++ class RFList<CombinatoricState>+;
#pragma link C++ class RFList<HistPlotVar>+;
#pragma link C++ class RFList<HistPlotCategory>+;
#pragma link C++ class RFList<const HistPlotCategory>+;

#pragma link C++ typedef RestFrames::RFBaseList;
#pragma link C++ typedef RestFrames::RestFrameList;
#pragma link C++ typedef RestFrames::ConstRestFrameList;
#pragma link C++ typedef RestFrames::GroupList;
#pragma link C++ typedef RestFrames::ConstGroupList;
#pragma link C++ typedef RestFrames::JigsawList;
#pragma link C++ typedef RestFrames::ConstJigsawList;
#pragma link C++ typedef RestFrames::StateList;
#pragma link C++ typedef RestFrames::VisibleStateList;
#pragma link C++ typedef RestFrames::HistPlotVarList;
#pragma link C++ typedef RestFrames::HistPlotCatList;

#pragma link C++ class RestFrame+;

#pragma link C++ class GeneratorFrame+;
#pragma link C++ class LabGenFrame+;
#pragma link C++ class DecayGenFrame+;
#pragma link C++ class InvisibleGenFrame+;
#pragma link C++ class VisibleGenFrame+;
#pragma link C++ class ResonanceGenFrame+;
#pragma link C++ class ppLabGenFrame+;

#pragma link C++ class ReconstructionFrame+;
#pragma link C++ class LabRecoFrame+;
#pragma link C++ class DecayRecoFrame+;
#pragma link C++ class InvisibleRecoFrame+;
#pragma link C++ class VisibleRecoFrame+;
#pragma link C++ class SelfAssemblingRecoFrame+;

#pragma link C++ class Group+;
#pragma link C++ class InvisibleGroup+;
#pragma link C++ class CombinatoricGroup+;

#pragma link C++ class Jigsaw+;

#pragma link C++ class InvisibleJigsaw+;
#pragma link C++ class SetMassInvJigsaw+;
#pragma link C++ class SetRapidityInvJigsaw+;
#pragma link C++ class ContraBoostInvJigsaw+;
#pragma link C++ class CombinedCBInvJigsaw+;
#pragma link C++ class MaxProbBreitWignerInvJigsaw+;
#pragma link C++ class MinMassDiffInvJigsaw+;
#pragma link C++ class MinMassesSqInvJigsaw+;

#pragma link C++ class CombinatoricJigsaw+;
#pragma link C++ class MinMassesCombJigsaw+;
#pragma link C++ class MaxProbBreitWignerCombJigsaw+;
#pragma link C++ class MinMassChi2CombJigsaw+;
#pragma link C++ class MinMassDiffCombJigsaw+;
#pragma link C++ class MinMassesSqCombJigsaw+;

#pragma link C++ class State+;
#pragma link C++ class VisibleState+;
#pragma link C++ class InvisibleState+;
#pragma link C++ class CombinatoricState+;

#pragma link C++ class RFPlot+;
#pragma link C++ class TreePlot+;
#pragma link C++ class TreePlotLink+;
#pragma link C++ class TreePlotNode+;
#pragma link C++ class HistPlot+;
#pragma link C++ class HistPlotCategory+;
#pragma link C++ class HistPlotVar+;

#endif /* __ROOTCLING__ and __CINT__ */

