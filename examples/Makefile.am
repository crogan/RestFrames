# make RestFrames examples
SUBDIRS = .

noinst_PROGRAMS = example_Zll.x example_Wlnu.x example_top_to_bWlnu.x example_Hp_to_HggWlnu.x example_H_to_WlnuWlnu.x example_ttbar_to_bWlnubWlnu.x example_DiGluino_to_bbXbbX.x example_X2X2_to_ZllXHggX.x

includestuff = $(top_srcdir)/inc
LDADD = ../src/libRestFrames.la

example_Zll_x_SOURCES = example_Zll.C
example_Zll_x_SHORTNAME = Zll
example_Zll_x_CXXFLAGS = -I../inc -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS) $(ROOTAUXCXXFLAGS)

example_Wlnu_x_SOURCES = example_Wlnu.C
example_Wlnu_x_SHORTNAME = Wlnu
example_Wlnu_x_CXXFLAGS = -I../inc -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS) $(ROOTAUXCXXFLAGS)

example_top_to_bWlnu_x_SOURCES = example_top_to_bWlnu.C
example_top_to_bWlnu_x_SHORTNAME = top_to_bWlnu
example_top_to_bWlnu_x_CXXFLAGS = -I../inc -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS) $(ROOTAUXCXXFLAGS)

example_Hp_to_HggWlnu_x_SOURCES = example_Hp_to_HggWlnu.C
example_Hp_to_HggWlnu_x_SHORTNAME = Hp_to_HggWlnu
example_Hp_to_HggWlnu_x_CXXFLAGS = -I../inc -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS) $(ROOTAUXCXXFLAGS)

example_H_to_WlnuWlnu_x_SOURCES = example_H_to_WlnuWlnu.C
example_H_to_WlnuWlnu_x_SHORTNAME = H_to_WlnuWlnu
example_H_to_WlnuWlnu_x_CXXFLAGS = -I../inc -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS) $(ROOTAUXCXXFLAGS)

example_ttbar_to_bWlnubWlnu_x_SOURCES = example_ttbar_to_bWlnubWlnu.C
example_ttbar_to_bWlnubWlnu_x_SHORTNAME = ttbar_to_bWlnubWlnu
example_ttbar_to_bWlnubWlnu_x_CXXFLAGS = -I../inc -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS) $(ROOTAUXCXXFLAGS)

example_DiGluino_to_bbXbbX_x_SOURCES = example_DiGluino_to_bbXbbX.C
example_DiGluino_to_bbXbbX_x_SHORTNAME = DiGluino_to_bbXbbX
example_DiGluino_to_bbXbbX_x_CXXFLAGS = -I../inc -g -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS) $(ROOTAUXCXXFLAGS)

example_X2X2_to_ZllXHggX_x_SOURCES = example_X2X2_to_ZllXHggX.C
example_X2X2_to_ZllXHggX_x_SHORTNAME = X2X2_to_ZllXHggX
example_X2X2_to_ZllXHggX_x_CXXFLAGS = -I../inc -g -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS) $(ROOTAUXCXXFLAGS)

include $(top_srcdir)/config/rootconfig.mk
