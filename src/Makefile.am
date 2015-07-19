# make RestFrames shared library
AUTOMAKE_OPTIONS            = gnu

includestuff              = $(top_srcdir)/inc
RestFrameslibdir          = $(libdir)
# RestFrameslib_DATA        = $(top_srcdir)/config/libRestFrames.rootmap 
RestFrameslib_LTLIBRARIES = libRestFrames.la
libRestFrames_la_CXXFLAGS = -I../inc -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS) $(ROOTAUXCXXFLAGS)
libRestFrames_la_LDFLAGS  = $(ROOTLDFLAGS) -version-info $(SOVERSION)
libRestFrames_la_SOURCES  = RestFrames_Dict.cxx RestFrames_Dict.h RFBase.cc RFKey.cc RFList.cc RFLog.cc\
	FramePlot.cc RestFrame.cc ReconstructionFrame.cc GeneratorFrame.cc LabFrame.cc DecayFrame.cc\
	VisibleFrame.cc InvisibleFrame.cc LabRecoFrame.cc DecayRecoFrame.cc VisibleRecoFrame.cc\
	InvisibleRecoFrame.cc LabGenFrame.cc DecayGenFrame.cc VisibleGenFrame.cc InvisibleGenFrame.cc\
	ResonanceGenFrame.cc State.cc VisibleState.cc InvisibleState.cc CombinatoricState.cc Group.cc\
	InvisibleGroup.cc CombinatoricGroup.cc Jigsaw.cc InvisibleJigsaw.cc\
	CombinatoricJigsaw.cc InvisibleMassJigsaw.cc InvisibleRapidityJigsaw.cc\
	MinimizeMassesCombinatoricJigsaw.cc\
	ContraBoostInvariantJigsaw.cc SelfAssemblingRecoFrame.cc 

uninstall-hook:
	rm -f $(DESTDIR)$(libdir)/libRestFrames.rootmap
	rm -f $(DESTDIR)$(libdir)/RestFrames_Dict_rdict.pcm

CLEANFILES = *Dict.cxx *Dict.h *~

include $(top_srcdir)/config/rootconfig.mk

if EXISTROOTCLING
RestFrameslib_DATA = RestFrames_Dict_rdict.pcm
RestFrames_Dict_rdict.pcm: RestFrames_Dict.cxx
	echo "Invoking rootcling"

RestFrames_Dict.cxx: $(includestuff)/RestFrames/*.hh $(includestuff)/RestFrames/RestFrames_LinkDef.h
	$(ROOTCINT) -f $@ -rmf temp.rootmap -rml libRestFrames -c -I$(includestuff) $(ROOTAUXCXXFLAGS) $(ROOTCXXFLAGS) $^

install-data-hook: 
	rm -f $(DESTDIR)$(libdir)/libRestFrames.rootmap
	cp temp.rootmap $(DESTDIR)$(libdir)/libRestFrames.rootmap

else
RestFrames_Dict.cxx: $(includestuff)/RestFrames/*.hh $(includestuff)/RestFrames/RestFrames_LinkDef.h
	$(ROOTCINT) -f $@ -c -I$(includestuff) $(ROOTCXXFLAGS) $^

install-data-hook:
	rm -f $(DESTDIR)$(libdir)/libRestFrames.rootmap
	rm -f temp.rootmap
	rlibmap -f -o temp.rootmap -d libRestFrames -c $(includestuff)/RestFrames/RestFrames_LinkDef.h
	cat temp.rootmap $(top_srcdir)/config/libRestFrames.rootmap > $(DESTDIR)$(libdir)/libRestFrames.rootmap
	rm -f temp.rootmap

endif
