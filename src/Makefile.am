# make RestFrames shared library
AUTOMAKE_OPTIONS            = gnu

includestuff              = $(top_srcdir)/inc
RestFrameslibdir          = $(libdir)
# RestFrameslib_DATA        = $(top_srcdir)/config/libRestFrames.rootmap 
RestFrameslib_LTLIBRARIES = libRestFrames.la
libRestFrames_la_CXXFLAGS = -I../inc -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS) $(ROOTAUXCXXFLAGS)
libRestFrames_la_LDFLAGS  = $(ROOTLDFLAGS) -version-info $(SOVERSION)
libRestFrames_la_SOURCES  = libRestFrames_rdict.cxx libRestFrames_rdict.h\
	RFBase.cc RFKey.cc RFList.cc RFLog.cc RFPlot.cc RFCharge.cc\
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

uninstall-hook:
	rm -f $(DESTDIR)$(libdir)/libRestFrames.rootmap
	rm -f $(DESTDIR)$(libdir)/libRestFrames_rdict.pcm

CLEANFILES = *rdict.cxx *rdict.h *~

include $(top_srcdir)/config/rootconfig.mk

if EXISTROOTCLING
RestFrameslib_DATA = libRestFrames_rdict.pcm
libRestFrames_rdict.pcm: libRestFrames_rdict.cxx
	echo "Invoking rootcling"

libRestFrames_rdict.cxx: $(includestuff)/RestFrames/*.hh $(includestuff)/RestFrames/RestFrames_LinkDef.h
	$(ROOTCINT) -f $@ -s libRestFrames.so -rmf temp.rootmap -rml libRestFrames.so -c -I$(includestuff) $(ROOTAUXCXXFLAGS) $(ROOTCXXFLAGS) $^
# $(ROOTCINT) -f $@ -rmf temp.rootmap -rml libRestFrames -c -I$(includestuff) $(ROOTAUXCXXFLAGS) $(ROOTCXXFLAGS) $^

install-data-hook: 
	rm -f $(DESTDIR)$(libdir)/libRestFrames.rootmap
	mv temp.rootmap $(DESTDIR)$(libdir)/libRestFrames.rootmap

else
libRestFrames_rdict.cxx: $(includestuff)/RestFrames/*.hh $(includestuff)/RestFrames/RestFrames_LinkDef.h
	$(ROOTCINT) -f $@ -c -I$(includestuff) $(ROOTCXXFLAGS) $^

install-data-hook:
	rm -f $(DESTDIR)$(libdir)/libRestFrames.rootmap
	rm -f temp.rootmap
	rlibmap -f -o temp.rootmap -d libRestFrames -c $(includestuff)/RestFrames/RestFrames_LinkDef.h
	cat temp.rootmap $(top_srcdir)/config/libRestFrames.rootmap > $(DESTDIR)$(libdir)/libRestFrames.rootmap
	rm -f temp.rootmap

endif
