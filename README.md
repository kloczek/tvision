# TVision - Turbo Vision

## Table of Contents

- [Description](#description)
- [Changelog](#changelog)
- [Wanted](#wanted)
- [History](#history)

## Description

Turbo Vision (or TV, for short) is a library that provides an application
framework.  With TV you can write a beautiful object-oriented character-mode
user interface in a short time.

Original TV was available in C++ and Pascal and is a product of Borland International.
It was developed to run on MS-DOS systems, but today it is available for many
other platforms (ported by independent programmers).

This port is based on the Borland 2.0 version with fixes.

Warning: the BSD-style copyright is applicable only to the modifications
brought by Sergio Sigala to the original code.

Happy hacking!

## Changelog
- 0.8

## Wanted

If you wrote a program using TVision or know any useful site please tell me
its address.  I'll add it in the list at the very bottom of file
`doc/tvision.texi' (see below).

## How to install the package

Read the `INSTALL' file for generic informations on installing this library.
More options are available for `configure':

Linker options:

  --enable-shared         build shared libraries [default=yes]
  --enable-shared=PKGS    only build shared libraries if the current package
                          appears as an element in the PKGS list
  --enable-static         build static libraries [default=yes]
  --enable-static=PKGS    only build shared libraries if the current package
                          appears as an element in the PKGS list
  --with-gnu-ld           assume the C compiler uses GNU ld [default=no]

Miscellaneous options:

  --enable-russian        enable Russian character set (now obsolete)
  --disable-acs           disable alternate character set
  --disable-linux-gpm     disable Linux GPM mouse support
  --disable-linux-vcs     disable Linux VCS support
  --disable-freebsd-mouse disable FreeBSD mouse support

## Where to find documentation

See directory `doc' and its subdirectory.  Here is a summary of their contents.

- `tvision.info' is a Texinfo file and contains the "installation handbook":
  information regarding installation, keyboard handling, screen manipulation,
  environment variables and mouse support.  You should take a look at it
  before using the library.  Just type `info -f tvision.info'.

- `html/tvision.html' is the main page of the documentation.  It includes
  links to both the html version of the "installation handbook" and to the
  "class hierarchy documentation".  The latter is a detailed reference
  manual for the various classes, methods and data members TVision uses.

## History

Sergio Sigala
sergio@sigala.it
