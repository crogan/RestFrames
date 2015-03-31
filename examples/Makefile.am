# make RestFrames examples
SUBDIRS = .

noinst_PROGRAMS = example_01.x

includestuff = $(top_srcdir)/inc

example_01_x_SOURCES = example_01_basics_Ztoll.C
example_01_x_SHORTNAME = 01
example_01_x_CXXFLAGS = -I../inc -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS) $(ROOTAUXCXXFLAGS)
example_01_x_LDADD = $(top_srcdir)/src/libRestFrames.la

include $(top_srcdir)/config/rootconfig.mk
