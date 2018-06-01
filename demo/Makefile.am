AUTOMAKE_OPTIONS = foreign

noinst_PROGRAMS = demo

demo_SOURCES = \
	ascii.cc \
	ascii.h \
	calc.cc \
	calc.h \
	calendar.cc \
	calendar.h \
	demohelp.h \
	fileview.cc \
	fileview.h \
	gadgets.cc \
	gadgets.h \
	mousedlg.cc \
	mousedlg.h \
	puzzle.cc \
	puzzle.h \
	tvcmds.h \
	tvdemo1.cc \
	tvdemo2.cc \
	tvdemo3.cc \
	tvdemo.h

LDADD = $(top_builddir)/lib/libtvision.la

AM_CPPFLAGS = -I../lib

EXTRA_DIST = DEMOHELP.H32
