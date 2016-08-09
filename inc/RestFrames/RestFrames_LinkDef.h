#ifndef RestFrames_LinkDef_h
#define RestFrames_LinkDef_h

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace RestFrames;
using namespace RestFrames;

#pragma link C++ class RestFrames::RFLog-;
#pragma link C++ class RestFrames::RFBase-;
#pragma link C++ class RestFrames::RFKey-;
#pragma link C++ class RestFrames::RFCharge-;

#pragma link C++ class RestFrames::RFList<RFBase>-;
#pragma link C++ class RestFrames::RFList<RestFrame>-; 
#pragma link C++ class RestFrames::RFList<const RestFrame>-; 
#pragma link C++ class RestFrames::RFList<ReconstructionFrame>-;
#pragma link C++ class RestFrames::RFList<GeneratorFrame>-;
#pragma link C++ class RestFrames::RFList<VisibleRecoFrame>-;
#pragma link C++ class RestFrames::RFList<DecayRecoFrame>-;
#pragma link C++ class RestFrames::RFList<ResonanceGenFrame>-; 
#pragma link C++ class RestFrames::RFList<Jigsaw>-;
#pragma link C++ class RestFrames::RFList<const Jigsaw>-;
#pragma link C++ class RestFrames::RFList<Group>-;
#pragma link C++ class RestFrames::RFList<const Group>-;
#pragma link C++ class RestFrames::RFList<State>-;
#pragma link C++ class RestFrames::RFList<VisibleState>-;
#pragma link C++ class RestFrames::RFList<InvisibleState>-;
#pragma link C++ class RestFrames::RFList<CombinatoricState>-;
#pragma link C++ class RestFrames::RFList<HistPlotVar>-;
#pragma link C++ class RestFrames::RFList<HistPlotCategory>-;
#pragma link C++ class RestFrames::RFList<const HistPlotCategory>-;

#pragma link C++ class RestFrames::RestFrame-;

#pragma link C++ class RestFrames::GeneratorFrame-;
#pragma link C++ class RestFrames::LabGenFrame-;
#pragma link C++ class RestFrames::DecayGenFrame-;
#pragma link C++ class RestFrames::InvisibleGenFrame-;
#pragma link C++ class RestFrames::VisibleGenFrame-;
#pragma link C++ class RestFrames::ResonanceGenFrame-;
#pragma link C++ class RestFrames::ppLabGenFrame-;

#pragma link C++ class RestFrames::ReconstructionFrame-;
#pragma link C++ class RestFrames::LabRecoFrame-;
#pragma link C++ class RestFrames::DecayRecoFrame-;
#pragma link C++ class RestFrames::InvisibleRecoFrame-;
#pragma link C++ class RestFrames::VisibleRecoFrame-;
#pragma link C++ class RestFrames::SelfAssemblingRecoFrame-;

#pragma link C++ class RestFrames::Group-;
#pragma link C++ class RestFrames::InvisibleGroup-;
#pragma link C++ class RestFrames::CombinatoricGroup-;

#pragma link C++ class RestFrames::Jigsaw-;

#pragma link C++ class RestFrames::InvisibleJigsaw-;
#pragma link C++ class RestFrames::SetMassInvJigsaw-;
#pragma link C++ class RestFrames::SetRapidityInvJigsaw-;
#pragma link C++ class RestFrames::ContraBoostInvJigsaw-;
#pragma link C++ class RestFrames::CombinedCBInvJigsaw-;
#pragma link C++ class RestFrames::MaxProbBreitWignerInvJigsaw-;
#pragma link C++ class RestFrames::MinMassDiffInvJigsaw-;
#pragma link C++ class RestFrames::MinMassesSqInvJigsaw-;

#pragma link C++ class RestFrames::CombinatoricJigsaw-;
#pragma link C++ class RestFrames::MinMassesCombJigsaw-;
#pragma link C++ class RestFrames::MaxProbBreitWignerCombJigsaw-;
#pragma link C++ class RestFrames::MinMassChi2CombJigsaw-;
#pragma link C++ class RestFrames::MinMassDiffCombJigsaw-;
#pragma link C++ class RestFrames::MinMassesSqCombJigsaw-;

#pragma link C++ class RestFrames::State-;
#pragma link C++ class RestFrames::VisibleState-;
#pragma link C++ class RestFrames::InvisibleState-;
#pragma link C++ class RestFrames::CombinatoricState-;

#pragma link C++ class RestFrames::RFPlot-;
#pragma link C++ class RestFrames::TreePlot-;
#pragma link C++ class RestFrames::TreePlotLink-;
#pragma link C++ class RestFrames::TreePlotNode-;
#pragma link C++ class RestFrames::HistPlot-;
#pragma link C++ class RestFrames::HistPlotCategory-;
#pragma link C++ class RestFrames::HistPlotVar-;

#endif /* __ROOTCLING__ and __CINT__ */

#endif // RestFrames_LinkDef_h
