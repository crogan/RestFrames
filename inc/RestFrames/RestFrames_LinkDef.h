#ifdef __ROOTCLING__
#include "RestFrames/RestFrames.hh"
#pragma link C++ namespace RestFrames;
using namespace RestFrames;

#pragma link C++ class RFBase;
#pragma link C++ class RFKey;
#pragma link C++ class RFCharge;

#pragma link C++ class RFList<RFBase>;
#pragma link C++ class RFList<RestFrame>; 
#pragma link C++ class RFList<const RestFrame>; 
#pragma link C++ class RFList<ReconstructionFrame>;
#pragma link C++ class RFList<GeneratorFrame>;
#pragma link C++ class RFList<VisibleRecoFrame>;
#pragma link C++ class RFList<DecayRecoFrame>;
#pragma link C++ class RFList<ResonanceGenFrame>; 
#pragma link C++ class RFList<Jigsaw>;
#pragma link C++ class RFList<const Jigsaw>;
#pragma link C++ class RFList<Group>;
#pragma link C++ class RFList<const Group>;
#pragma link C++ class RFList<State>;
#pragma link C++ class RFList<VisibleState>;
#pragma link C++ class RFList<InvisibleState>;
#pragma link C++ class RFList<CombinatoricState>;
#pragma link C++ class RFList<HistPlotVar>;
#pragma link C++ class RFList<HistPlotCategory>;
#pragma link C++ class RFList<const HistPlotCategory>;

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

#pragma link C++ enum LogType;
#pragma link C++ class RFLog;

#pragma link C++ class RestFrame;

#pragma link C++ class GeneratorFrame;
#pragma link C++ class LabGenFrame;
#pragma link C++ class DecayGenFrame;
#pragma link C++ class InvisibleGenFrame;
#pragma link C++ class VisibleGenFrame;
#pragma link C++ class ResonanceGenFrame;
#pragma link C++ class ppLabGenFrame;

#pragma link C++ class ReconstructionFrame;
#pragma link C++ class LabRecoFrame;
#pragma link C++ class DecayRecoFrame;
#pragma link C++ class InvisibleRecoFrame;
#pragma link C++ class VisibleRecoFrame;
#pragma link C++ class SelfAssemblingRecoFrame;

#pragma link C++ class Group;
#pragma link C++ class InvisibleGroup;
#pragma link C++ class CombinatoricGroup;

#pragma link C++ class Jigsaw;

#pragma link C++ class InvisibleJigsaw;
#pragma link C++ class SetMassInvJigsaw;
#pragma link C++ class SetRapidityInvJigsaw;
#pragma link C++ class ContraBoostInvJigsaw;
#pragma link C++ class CombinedCBInvJigsaw;
#pragma link C++ class MaxProbBreitWignerInvJigsaw;
#pragma link C++ class MinMassDiffInvJigsaw;
#pragma link C++ class MinMassesSqInvJigsaw;

#pragma link C++ class CombinatoricJigsaw;
#pragma link C++ class MinMassesCombJigsaw;
#pragma link C++ class MaxProbBreitWignerCombJigsaw;
#pragma link C++ class MinMassChi2CombJigsaw;
#pragma link C++ class MinMassDiffCombJigsaw;
#pragma link C++ class MinMassesSqCombJigsaw;

#pragma link C++ class State;
#pragma link C++ class VisibleState;
#pragma link C++ class InvisibleState;
#pragma link C++ class CombinatoricState;

#pragma link C++ class RFPlot;
#pragma link C++ class TreePlot;
#pragma link C++ class TreePlotLink;
#pragma link C++ class TreePlotNode;
#pragma link C++ class HistPlot;
#pragma link C++ class HistPlotCategory;
#pragma link C++ class HistPlotVar;

#elif __MAKECINT__

#pragma link off all typedefs;
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace RestFrames;
using namespace RestFrames;

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

#pragma link C++ enum LogType;
#pragma link C++ class RFLog+;

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

