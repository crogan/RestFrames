# make RestFrames shared library
AUTOMAKE_OPTIONS            = gnu

includestuff              = $(top_srcdir)/inc
incrf                     = RestFrames/
RestFrameslibdir          = $(libdir)

RestFrameslib_LTLIBRARIES = libRestFrames.la
libRestFrames_la_CXXFLAGS = -I../inc -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS) $(ROOTAUXCXXFLAGS)
libRestFrames_la_LDFLAGS  = -version-info $(SOVERSION) $(ROOTLDFLAGS)
libRestFrames_la_LIBADD   = $(ROOTLIBS)
libRestFrames_la_SOURCES  = RFBase.cc RFKey.cc RFList.cc RFLog.cc RFPlot.cc RFCharge.cc\
	TreePlot.cc TreePlotNode.cc TreePlotLink.cc\
	HistPlot.cc HistPlotVar.cc HistPlotCategory.cc\
	RestFrame.cc ReconstructionFrame.cc GeneratorFrame.cc\
	LabFrame.cc DecayFrame.cc VisibleFrame.cc InvisibleFrame.cc\
	LabRecoFrame.cc DecayRecoFrame.cc VisibleRecoFrame.cc InvisibleRecoFrame.cc\
	SelfAssemblingRecoFrame.cc\
	LabGenFrame.cc DecayGenFrame.cc VisibleGenFrame.cc InvisibleGenFrame.cc\
	ResonanceGenFrame.cc ppLabGenFrame.cc\
	State.cc VisibleState.cc InvisibleState.cc CombinatoricState.cc\
	Group.cc InvisibleGroup.cc CombinatoricGroup.cc\
	Jigsaw.cc InvisibleJigsaw.cc CombinatoricJigsaw.cc\
	SetMassInvJigsaw.cc SetRapidityInvJigsaw.cc ContraBoostInvJigsaw.cc\
	CombinedCBInvJigsaw.cc MinMassDiffInvJigsaw.cc MinMassesSqInvJigsaw.cc\
	MaxProbBreitWignerInvJigsaw.cc\
	MinMassesCombJigsaw.cc MinMassChi2CombJigsaw.cc MaxProbBreitWignerCombJigsaw.cc\
	MinMassDiffCombJigsaw.cc MinMassesSqCombJigsaw.cc
nodist_libRestFrames_la_SOURCES  = libRestFrames_rdict.cxx
BUILT_SOURCES = libRestFrames_rdict.cxx

uninstall-hook:
	rm -f $(DESTDIR)$(libdir)/libRestFrames.rootmap
	rm -f $(DESTDIR)$(libdir)/libRestFrames_rdict.pcm

CLEANFILES = *_rdict.* *_rdict_* *.rootmap *~

include $(top_srcdir)/config/rootconfig.mk

RestFrameslib_DATA = libRestFrames.rootmap

if EXISTROOTCLING

RestFrameslib_DATA += libRestFrames_rdict.pcm
libRestFrames_rdict.pcm: libRestFrames_rdict.cxx

include_headers  = $(incrf)RFKey.hh $(incrf)RFCharge.hh $(incrf)RFPlot.hh $(incrf)RFBase.hh $(incrf)RFLog.hh\
	$(incrf)RestFrame.hh $(incrf)ReconstructionFrame.hh $(incrf)GeneratorFrame.hh\
	$(incrf)LabFrame.hh $(incrf)DecayFrame.hh $(incrf)VisibleFrame.hh $(incrf)InvisibleFrame.hh\
	$(incrf)LabRecoFrame.hh $(incrf)DecayRecoFrame.hh $(incrf)VisibleRecoFrame.hh $(incrf)InvisibleRecoFrame.hh\
	$(incrf)SelfAssemblingRecoFrame.hh\
	$(incrf)LabGenFrame.hh $(incrf)DecayGenFrame.hh $(incrf)VisibleGenFrame.hh $(incrf)InvisibleGenFrame.hh\
	$(incrf)ResonanceGenFrame.hh $(incrf)ppLabGenFrame.hh\
	$(incrf)State.hh $(incrf)VisibleState.hh $(incrf)InvisibleState.hh $(incrf)CombinatoricState.hh\
	$(incrf)Group.hh $(incrf)InvisibleGroup.hh $(incrf)CombinatoricGroup.hh\
	$(incrf)Jigsaw.hh $(incrf)InvisibleJigsaw.hh $(incrf)CombinatoricJigsaw.hh\
	$(incrf)SetMassInvJigsaw.hh $(incrf)SetRapidityInvJigsaw.hh $(incrf)ContraBoostInvJigsaw.hh\
	$(incrf)CombinedCBInvJigsaw.hh $(incrf)MinMassDiffInvJigsaw.hh $(incrf)MinMassesSqInvJigsaw.hh\
	$(incrf)MaxProbBreitWignerInvJigsaw.hh\
	$(incrf)MinMassesCombJigsaw.hh $(incrf)MinMassChi2CombJigsaw.hh $(incrf)MaxProbBreitWignerCombJigsaw.hh\
	$(incrf)MinMassDiffCombJigsaw.hh $(incrf)MinMassesSqCombJigsaw.hh\
	$(incrf)TreePlot.hh $(incrf)TreePlotNode.hh $(incrf)TreePlotLink.hh\
	$(incrf)HistPlotVar.hh $(incrf)HistPlotCategory.hh $(incrf)HistPlot.hh $(incrf)RFList.hh

libRestFrames_rdict.cxx: $(includestuff)/RestFrames/*.hh $(includestuff)/RestFrames/RestFrames_LinkDef.h
	$(ROOTCINT) -f $@ -s libRestFrames -rml libRestFrames -rmf libRestFrames.rootmap -c -I$(includestuff) $(ROOTAUXCXXFLAGS) $(ROOTCXXFLAGS) $(include_headers) $(includestuff)/RestFrames/RestFrames_LinkDef.h

libRestFrames.rootmap: libRestFrames_rdict.cxx

else

libRestFrames_rdict.cxx: $(includestuff)/RestFrames/*.hh $(includestuff)/RestFrames/RestFrames_LinkDef.h
	$(ROOTCINT) -f $@ -c -I$(includestuff) $(ROOTCXXFLAGS) $^

libRestFrames.rootmap: libRestFrames_rdict.cxx $(includestuff)/RestFrames/RestFrames_LinkDef.h
	rlibmap -f -o temp.rootmap -d libRestFrames -c $(includestuff)/RestFrames/RestFrames_LinkDef.h
	cat temp.rootmap $(top_srcdir)/config/libRestFrames.rootmap > libRestFrames.rootmap
	rm -f temp.rootmap

endif
