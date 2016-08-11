# make RestFrames shared library
AUTOMAKE_OPTIONS            = gnu

includestuff              = $(top_srcdir)/inc
RestFrameslibdir          = $(libdir)
# RestFrameslib_DATA        = $(top_srcdir)/config/libRestFrames.rootmap 
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

libRestFrames_rdict.cxx: $(includestuff)/RestFrames/*.hh $(includestuff)/RestFrames/RestFrames_LinkDef.h
	$(ROOTCINT) -f $@.tmp -s libRestFrames.so -rml libRestFrames.so -rmf libRestFrames.rootmap -c -I$(includestuff) $(ROOTAUXCXXFLAGS) $(ROOTCXXFLAGS) $+
	@# Some magic to prefix header names with "$(PACKAGE)/", and only that, in dictionary and rootmap:
	$(GREP) -F -v '"'"`pwd`"'/",' $@.tmp | $(SED) 's|"\([^"]*/\)\?\([^/"]*[.]h\)",|"'RestFrames/'\2",| ; s|\\"\([^"]*/\)\?\([^/"]*[.]h\)\\"\\n"|\\"'RestFrames/'\2\\"\\n"|' > $@ && $(RM) $@.tmp
	$(SED) 's|\$$clingAutoload\$$\([^/""]\+/\)*|$$clingAutoload$$'RestFrames'/|; /.*DICTPAYLOAD(.*/,/.*)DICTPAYLOAD.*/ s|#include "\([^/"]\+/\)*\(.*\)"|#include \"'RestFrames'/\2\"|' -i $@
#$(GREP) -F -v '"'"`pwd`"'/",' $@.tmp | $(SED) 's|"\([^"]*/\)\?\([^/"]*[.]h\)",|"'$(PACKAGE)/'\2",| ; s|\\"\([^"]*/\)\?\([^/"]*[.]h\)\\"\\n"|\\"'$(PACKAGE)/'\2\\"\\n"|' > $@ && $(RM) $@.tmp
#$(SED) 's|\$$clingAutoload\$$\([^/""]\+/\)*|$$clingAutoload$$'$(PACKAGE)'/|; /.*DICTPAYLOAD(.*/,/.*)DICTPAYLOAD.*/ s|#include "\([^/"]\+/\)*\(.*\)"|#include <'$(PACKAGE)'/\2>|' -i $@
#$(SED) 's|\(header \+\)\([^ ].*/\)\?\([^ ].*[.]h\)|\1'$(PACKAGE)/'\3|' -i libBAT.rootmap

libRestFrames.rootmap: libRestFrames_rdict.cxx

#libRestFrames_rdict.cxx: $(includestuff)/RestFrames/*.hh $(includestuff)/RestFrames/RestFrames_LinkDef.h
#	$(ROOTCINT) -f $@ -s libRestFrames.so -rmf temp.rootmap -rml libRestFrames.so -c -p -I$(includestuff) $(ROOTAUXCXXFLAGS) $(ROOTCXXFLAGS) $^
# $(ROOTCINT) -f $@ -rmf temp.rootmap -rml libRestFrames -c -I$(includestuff) $(ROOTAUXCXXFLAGS) $(ROOTCXXFLAGS) $^

#install-data-hook: 
#	rm -f $(DESTDIR)$(libdir)/libRestFrames.rootmap
#	cat temp.rootmap $(top_srcdir)/config/libRestFrames_ROOT6.rootmap > $(DESTDIR)$(libdir)/libRestFrames.rootmap
#cp temp.rootmap $(DESTDIR)$(libdir)/libRestFrames.rootmap

else

libRestFrames_rdict.cxx: $(includestuff)/RestFrames/*.hh $(includestuff)/RestFrames/RestFrames_LinkDef.h
	$(ROOTCINT) -f $@ -c -I$(includestuff) $(ROOTCXXFLAGS) $^

libRestFrames.rootmap: libRestFrames_rdict.cxx $(includestuff)/RestFrames/RestFrames_LinkDef.h
	rlibmap -f -o temp.rootmap -d libRestFrames -c $(includestuff)/RestFrames/RestFrames_LinkDef.h
	cat temp.rootmap $(top_srcdir)/config/libRestFrames.rootmap > libRestFrames.rootmap
	rm -f temp.rootmap

#install-data-hook:
#	rm -f $(DESTDIR)$(libdir)/libRestFrames.rootmap
#	rm -f temp.rootmap
#	rlibmap -f -o temp.rootmap -d libRestFrames -c $(includestuff)/RestFrames/RestFrames_LinkDef.h
#	cat temp.rootmap $(top_srcdir)/config/libRestFrames.rootmap > $(DESTDIR)$(libdir)/libRestFrames.rootmap
#	rm -f temp.rootmap

endif
