#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace RestFrames;
#ifndef __ROOTCLING__
using namespace RestFrames;
#endif

#pragma link C++ class RestFrames::RFKey+;
#pragma link C++ class RestFrames::RFCharge+;
#pragma link C++ enum RestFrames::LogType;
#pragma link C++ class RestFrames::RFLog+;

#pragma link C++ class RestFrames::RFList<RestFrames::RestFrame>+; 
#pragma link C++ class RestFrames::RFList<const RestFrames::RestFrame>+; 
#pragma link C++ class RestFrames::RFList<RestFrames::Jigsaw>+;
#pragma link C++ class RestFrames::RFList<const RestFrames::Jigsaw>+;
#pragma link C++ class RestFrames::RFList<RestFrames::Group>+;
#pragma link C++ class RestFrames::RFList<const RestFrames::Group>+;
#pragma link C++ class RestFrames::RFList<RestFrames::HistPlotVar>+;
#pragma link C++ class RestFrames::RFList<RestFrames::HistPlotCategory>+;
#pragma link C++ class RestFrames::RFList<const RestFrames::HistPlotCategory>+;

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

#pragma link C++ class RestFrames::RFBase+;
#pragma link C++ class RestFrames::RestFrame+;

#pragma link C++ class RestFrames::LabGenFrame+;
#pragma link C++ class RestFrames::DecayGenFrame+;
#pragma link C++ class RestFrames::InvisibleGenFrame+;
#pragma link C++ class RestFrames::VisibleGenFrame+;
#pragma link C++ class RestFrames::ResonanceGenFrame+;
#pragma link C++ class RestFrames::ppLabGenFrame+;

#pragma link C++ class RestFrames::LabRecoFrame+;
#pragma link C++ class RestFrames::DecayRecoFrame+;
#pragma link C++ class RestFrames::InvisibleRecoFrame+;
#pragma link C++ class RestFrames::VisibleRecoFrame+;
#pragma link C++ class RestFrames::SelfAssemblingRecoFrame+;

#pragma link C++ class RestFrames::Group+;
#pragma link C++ class RestFrames::InvisibleGroup+;
#pragma link C++ class RestFrames::CombinatoricGroup+;

#pragma link C++ class RestFrames::Jigsaw+;
#pragma link C++ class RestFrames::InvisibleJigsaw+;
#pragma link C++ class RestFrames::CombinatoricJigsaw+;

#pragma link C++ class RestFrames::SetMassInvJigsaw+;
#pragma link C++ class RestFrames::SetRapidityInvJigsaw+;
#pragma link C++ class RestFrames::ContraBoostInvJigsaw+;
#pragma link C++ class RestFrames::CombinedCBInvJigsaw+;
#pragma link C++ class RestFrames::MaxProbBreitWignerInvJigsaw+;
#pragma link C++ class RestFrames::MinMassDiffInvJigsaw+;
#pragma link C++ class RestFrames::MinMassesSqInvJigsaw+;

#pragma link C++ class RestFrames::MinMassesCombJigsaw+;
#pragma link C++ class RestFrames::MaxProbBreitWignerCombJigsaw+;
#pragma link C++ class RestFrames::MinMassChi2CombJigsaw+;
#pragma link C++ class RestFrames::MinMassDiffCombJigsaw+;
#pragma link C++ class RestFrames::MinMassesSqCombJigsaw+;

#pragma link C++ class RestFrames::RFPlot+;
#pragma link C++ class RestFrames::TreePlotNode+;
#pragma link C++ class RestFrames::TreePlotLink+;
#pragma link C++ class RestFrames::TreePlot+;
#pragma link C++ class RestFrames::HistPlotCategory+;
#pragma link C++ class RestFrames::HistPlotVar+;
#pragma link C++ class RestFrames::HistPlot+;

#pragma link C++ function RestFrames::SetStyle();
#pragma link C++ function RestFrames::SetStyle(bool);
#pragma link C++ function RestFrames::SetZPalette(bool);
#pragma link C++ function RestFrames::SetLogPrint(bool);
#pragma link C++ function RestFrames::SetLogPrint(RestFrames::LogType, bool);
#pragma link C++ function RestFrames::SetLogStream(std::ostream*);
#pragma link C++ function RestFrames::SetLogColor(bool);
#pragma link C++ function RestFrames::SetLogMaxWidth(int);

#pragma link C++ global RestFrames::RFLog::g_Log;

#endif /* __ROOTCLING__ and __CINT__ */

