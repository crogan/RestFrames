# make RestFrames shared library
AUTOMAKE_OPTIONS            = gnu

includestuff              = $(top_srcdir)/include
RestFrameslibdir          = $(libdir)
RestFrameslib_DATA        = $(top_srcdir)/config/libRestFrames.rootmap
RestFrameslib_LTLIBRARIES = libRestFrames.la
libRestFrames_la_CXXFLAGS = -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS)
libRestFrames_la_LDFLAGS  = $(ROOTLDFLAGS) -version-info $(SOVERSION)
libRestFrames_la_SOURCES  = RestFrames_Dict.cxx CombinatoricGroup.cc GroupList.cc\
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

CLEANFILES = *Dict.cxx *Dict.h *~

include $(top_srcdir)/config/rootconfig.mk

RestFrames_Dict.cxx: $(includestuff)/RestFrames/*.hh $(includestuff)/RestFrames/RestFrames_LinkDef.h
	$(ROOTCINT) -f $@ -c -I$(includestuff) $^
