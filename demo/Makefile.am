LDADD		= $(top_builddir)/tvision/libtvision.la
AM_CPPFLAGS	= -I$(top_srcdir)/tvision

noinst_PROGRAMS = demo

demo_SOURCES	= \
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

EXTRA_DIST	= \
	demohelp.txt

CLEANFILES	= \
	demohelp.h \
	demohelp.hlp

$(srcdir)/tvdemo1.cc: demohelp.h
$(srcdir)/tvdemo2.cc: demohelp.h
$(srcdir)/tvdemo3.cc: demohelp.h

demohelp.h: $(top_srcdir)/demo/demohelp.txt $(top_builddir)/tvhc/tvhc
	$(top_builddir)/tvhc/tvhc $(top_srcdir)/demo/demohelp.txt demohelp.hlp demohelp.h

demohelp.hlp: $(top_srcdir)/demo/demohelp.txt $(top_builddir)/tvhc/tvhc
	$(top_builddir)/tvhc/tvhc $(top_srcdir)/demo/demohelp.txt demohelp.hlp demohelp.h
