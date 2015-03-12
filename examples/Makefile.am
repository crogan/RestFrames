# make RestFrames examples
SUBDIRS = .

noinst_PROGRAMS = example_01

includestuff = $(top_srcdir)/inc

example_01_SOURCES = example_01_basics_Ztoll.C
example_01_SHORTNAME = 01
example_01_CXXFLAGS = -I../inc -I$(includestuff) $(AM_CXXFLAGS) $(ROOTCXXFLAGS)
example_01_LDADD = $(top_srcdir)/src/libRestFrames.la

include $(top_srcdir)/config/rootconfig.mk
