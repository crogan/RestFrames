# make RestFrames shared library
AUTOMAKE_OPTIONS            = gnu

includestuff              = $(top_srcdir)/inc
RestFrameslibdir          = $(libdir)
# RestFrameslib_DATA        = $(top_srcdir)/config/libRestFrames.rootmap 
RestFrameslib_LTLIBRARIES = libRestFrames.la
libRestFrames_la_CXXFLAGS = -I../inc -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS) $(ROOTAUXCXXFLAGS)
libRestFrames_la_LDFLAGS  = $(ROOTLDFLAGS) -version-info $(SOVERSION)
libRestFrames_la_SOURCES  = RestFrames_Dict.cxx RestFrames_Dict.h CombinatoricGroup.cc GroupList.cc\
	MinimizeMassesCombinatoricJigsaw.cc CombinatoricJigsaw.cc InvisibleFrame.cc\
	NVisibleMInvisibleJigsaw.cc CombinatoricState.cc InvisibleGroup.cc\
	RDecayFrame.cc ContraBoostInvariantJigsaw.cc InvisibleJigsaw.cc\
	RFrame.cc DecayFrame.cc	InvisibleMassJigsaw.cc\
	RInvisibleFrame.cc FrameLink.cc	InvisibleRapidityJigsaw.cc\
	RLabFrame.cc FramePlot.cc InvisibleState.cc\
	RSelfAssemblingFrame.cc GDecayFrame.cc Jigsaw.cc\
	RVisibleFrame.cc GFrame.cc JigsawList.cc\
	RestFrame.cc GInvisibleFrame.cc	LabFrame.cc\
	RestFrameList.cc GLabFrame.cc State.cc\
	GVisibleFrame.cc StateList.cc Group.cc\
	VisibleFrame.cc

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
	rm temp.rootmap

else
RestFrames_Dict.cxx: $(includestuff)/RestFrames/*.hh $(includestuff)/RestFrames/RestFrames_LinkDef.h
	$(ROOTCINT) -f $@ -c -I$(includestuff) $(ROOTCXXFLAGS) $^

install-data-hook:
	rm -f $(DESTDIR)$(libdir)/libRestFrames.rootmap
	rlibmap -f -o temp.rootmap -d libRestFrames -c $(includestuff)/RestFrames/RestFrames_LinkDef.h
	cat temp.rootmap $(top_srcdir)/config/libRestFrames.rootmap > $(DESTDIR)$(libdir)/libRestFrames.rootmap
	rm temp.rootmap

endif
