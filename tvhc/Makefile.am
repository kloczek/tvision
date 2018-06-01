AUTOMAKE_OPTIONS = foreign

noinst_PROGRAMS	= tvhc

tvhc_SOURCES	= \
	tvhc.cc \
	tvhc.h

LDADD		= $(top_builddir)/tvision/libtvision.la
AM_CPPFLAGS	= -I$(top_srcdir)/tvision

EXTRA_DIST = DEMOHELP.TXT README
