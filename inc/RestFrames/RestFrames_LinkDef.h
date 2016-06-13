#ifdef __ROOTCLING__
#include "RestFrames/RestFrames.hh"
#pragma link C++ namespace RestFrames;
using namespace RestFrames;

#pragma link C++ class RFBase;
#pragma link C++ class RFKey;

#pragma link C++ class RFList<RFBase>;
#pragma link C++ class RFList<RestFrame>;
#pragma link C++ class RFList<const RestFrame>;
#pragma link C++ class RFList<ReconstructionFrame>;
#pragma link C++ class RFList<GeneratorFrame>;  
#pragma link C++ class RFList<ResonanceGenFrame>;
#pragma link C++ class RFList<Jigsaw>;
#pragma link C++ class RFList<Group>;
#pragma link C++ class RFList<State>;
#pragma link C++ class RFList<VisibleState>;
#pragma link C++ class RFList<InvisibleState>;
#pragma link C++ class RFList<CombinatoricState>;

#pragma link C++ class RFLog;

#pragma link C++ class RestFrame;

#pragma link C++ class GeneratorFrame;
#pragma link C++ class LabGenFrame;
#pragma link C++ class DecayGenFrame;
#pragma link C++ class InvisibleGenFrame;
#pragma link C++ class VisibleGenFrame;
#pragma link C++ class ResonanceGenFrame;

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

#pragma link C++ class CombinatoricJigsaw;
#pragma link C++ class MinMassesCombJigsaw;

#pragma link C++ class State;
#pragma link C++ class VisibleState;
#pragma link C++ class InvisibleState;
#pragma link C++ class CombinatoricState;

#pragma link C++ class TreePlot;
#pragma link C++ class HistPlot;
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
#pragma link C++ class RFList<RFBase>+;
#pragma link C++ class RFList<RestFrame>+; 
#pragma link C++ class RFList<const RestFrame>+;
#pragma link C++ class RFList<ReconstructionFrame>+; 
#pragma link C++ class RFList<ResonanceGenFrame>+; 
#pragma link C++ class RFList<Jigsaw>+;
#pragma link C++ class RFList<Group>+;
#pragma link C++ class RFList<State>+;
#pragma link C++ class RFList<VisibleState>+;
#pragma link C++ class RFList<InvisibleState>+;
#pragma link C++ class RFList<CombinatoricState>+;

#pragma link C++ class RFLog+;

#pragma link C++ class RestFrame+;

#pragma link C++ class GeneratorFrame+;
#pragma link C++ class LabGenFrame+;
#pragma link C++ class DecayGenFrame+;
#pragma link C++ class InvisibleGenFrame+;
#pragma link C++ class VisibleGenFrame+;
#pragma link C++ class ResonanceGenFrame+;

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

#pragma link C++ class CombinatoricJigsaw+;
#pragma link C++ class MinMassesCombJigsaw+;

#pragma link C++ class State+;
#pragma link C++ class VisibleState+;
#pragma link C++ class InvisibleState+;
#pragma link C++ class CombinatoricState+;

#pragma link C++ class TreePlot+;
#pragma link C++ class HistPlot+;
#pragma link C++ class HistPlotVar+;

#endif /* __ROOTCLING__ and __CINT__ */

