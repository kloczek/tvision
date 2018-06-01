dnl Process this file with `autoconf' to produce a configure script.
AC_CONFIG_MACRO_DIRS([m4])

AC_INIT(lib/tv.h)
AM_INIT_AUTOMAKE(tvision, 0.8)
AC_CONFIG_MACRO_DIRS([m4])
AM_CONFIG_HEADER(config.h)

dnl Check for system.
AC_CANONICAL_HOST

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP

AM_ENABLE_SHARED
AM_DISABLE_STATIC
AM_PROG_LIBTOOL

dnl Checks for header files.
AC_STDC_HEADERS
AC_CHECK_HEADERS(ncurses.h curses.h gpm.h)

dnl Checks for libraries.
AC_SEARCH_LIBS(initscr, [ncursesw ncurses curses],
	[],
	[AC_MSG_ERROR(You need to install the ncurses or curses library)]
)

dnl Checks for library functions.
AC_CHECK_LIB(gpm, Gpm_Close)


dnl Various enable/disable checks.

AC_MSG_CHECKING(if the Russian character set should be enabled (now obsolete))
AC_ARG_ENABLE(russian,
	[  --enable-russian        enable Russian character set (now obsolete)],
	[AC_MSG_RESULT(yes)
	AC_DEFINE(ENABLE_RUSSIAN_CHARSET)
	],
	[AC_MSG_RESULT(no)]
)

AC_MSG_CHECKING(if the alternate character set should be disabled)
AC_ARG_ENABLE(acs,
	[  --disable-acs           disable alternate character set],
	[AC_MSG_RESULT(yes)
	AC_DEFINE(DISABLE_ACS)
	],
	[AC_MSG_RESULT(no)]
)

AC_MSG_CHECKING(if the 8 bit output should be disabled)
AC_ARG_ENABLE(8bit,
	[  --disable-8bit          disable 8 bit output],
	[AC_MSG_RESULT(yes)
	AC_DEFINE(DISABLE_8BIT_CHARS)
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
	AC_DEFINE(ENABLE_GPM)]
)

AC_MSG_CHECKING(if the Linux VCS support should be disabled)
AC_ARG_ENABLE(linux-vcs,
	[  --disable-linux-vcs     disable Linux VCS support],
	[AC_MSG_RESULT(yes)],
	[AC_MSG_RESULT(no)
	AC_DEFINE(ENABLE_VCS)
	]
)
	;;
	freebsd*)
AC_MSG_CHECKING(if the FreeBSD mouse support should be disabled)
AC_ARG_ENABLE(freebsd-mouse,
	[  --disable-freebsd-mouse disable FreeBSD mouse support],
	[AC_MSG_RESULT(yes)],
	[AC_MSG_RESULT(no)
	AC_DEFINE(ENABLE_FBSDM)
	]
)
	;;
esac

AC_OUTPUT([
	Makefile
	lib/Makefile
	tvhc/Makefile
	demo/Makefile
	tutorial/Makefile
	doc/Makefile
])
