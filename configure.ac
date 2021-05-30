dnl Process this file with `autoconf' to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([tvision],
	[0.8.0],
	[https://github.com/kloczek/tvision/issues],
	[],
	[https://github.com/kloczek/tvision])
AM_INIT_AUTOMAKE([
	--warnings=all --warnings=error
	dist-xz 
	no-dist-gzip
	foreign
])

AM_MAINTAINER_MODE([disable])
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_MACRO_DIR([m4])

dnl Check for system.
AC_CANONICAL_HOST

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AM_PROG_AR
AC_LANG([C++])

AC_ENABLE_SHARED([])
AC_DISABLE_STATIC([])
LT_INIT

dnl Checks for header files.
AC_CHECK_INCLUDES_DEFAULT

AC_CHECK_HEADERS(ncurses.h curses.h gpm.h)

dnl Checks for libraries.
AC_SEARCH_LIBS(initscr, [ncursesw ncurses curses],
	[AC_SUBST([SCREEN_LIB], ["-l${ac_lib}"])],
	[AC_MSG_ERROR(You need to install the ncurses or curses library)]
)
LIBS=""

dnl Checks for library functions.
AC_CHECK_LIB(gpm, Gpm_Close)

AC_MSG_CHECKING(if the alternate character set should be disabled)
AC_ARG_ENABLE(acs,
	[  --disable-acs           disable alternate character set],
	[AC_MSG_RESULT(yes)
	AC_DEFINE(DISABLE_ACS, 1, [alterate character set])
	],
	[AC_MSG_RESULT(no)]
)

case $host_os in
	linux*)
AC_MSG_CHECKING(if the Linux GPM mouse support should be disabled)
AC_ARG_ENABLE(linux-gpm,
	[  --disable-linux-gpm     disable Linux GPM mouse support],
	[AC_MSG_RESULT(yes)],
	[AC_MSG_RESULT(no)
	AC_DEFINE(ENABLE_GPM, 1, [Linux GPM mouse support])]
)

AC_MSG_CHECKING(if the FreeBSD mouse support should be disabled)
AC_ARG_ENABLE(freebsd-mouse,
	[  --disable-freebsd-mouse disable FreeBSD mouse support],
	[AC_MSG_RESULT(yes)],
	[AC_MSG_RESULT(no)
	AC_DEFINE(ENABLE_FBSDM, 1, [the FreeBSD mouse support])
	]
)
	;;
esac

AC_CONFIG_FILES([
	Makefile
	tvision/Makefile
	tvhc/Makefile
	demo/Makefile
	tutorial/Makefile
	doc/Doxyfile
	doc/Makefile
	tvision.pc
	tvision.spec
])
AC_OUTPUT
