
#
# This make file was generated from "Makefile.in" by "./configure" on
# Wed 10 Apr 2019 06:22:49 PM CEST -- all your changes will be lost if you
# run "./configure" again!
#


# -----------------------------------------------------------------------------
#
# Portions Copyright (c) 1999 GMRS Software GmbH
# Carl-von-Linde-Str. 38, D-85716 Unterschleissheim, http://www.gmrs.de
# All rights reserved.
#
# Author: Arno Unkrig <arno@unkrig.de>
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation; either version 2 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License in the file
# COPYING for more details.
#
# -----------------------------------------------------------------------------
#
# Changes to version 1.2.2 were made by Martin Bayer <mbayer@zedat.fu-berlin.de>
#
# -----------------------------------------------------------------------------

VERSION=1.3.2a

BISONXX  = bison++
YFLAGS   =

INSTALLER = install
BINDIR    = /usr/local/bin
MANDIR    = /usr/local/man
DOCDIR    = /usr/share/doc/html2text

CXX                 = g++
BOOL_DEFINITION     = 
EXPLICIT            = 
SYS_POLL_MISSING    = 
SOCKET_LIBRARIES    = 
LIBSTDCXX_INCLUDES  = 
LIBSTDCXX_LIBS      = 
AUTO_PTR_BROKEN     = -DAUTO_PTR_BROKEN
MAKEDEPEND_INCLUDES = -I/usr/include -I/usr/include/asm-generic -I/usr/include/c++/8 -I/usr/include/c++/8/backward -I/usr/include/c++/8/bits -I/usr/include/c++/8/debug -I/usr/include/c++/8/ext -I/usr/include/linux -I/usr/include/x86_64-linux-gnu/asm -I/usr/include/x86_64-linux-gnu/bits -I/usr/include/x86_64-linux-gnu/bits/types -I/usr/include/x86_64-linux-gnu/c++/8/bits -I/usr/include/x86_64-linux-gnu/gnu -I/usr/include/x86_64-linux-gnu/sys -I/usr/lib/gcc/x86_64-linux-gnu/8/include 

DEBUG=-O2 -g

INCLUDES  = $(LIBSTDCXX_INCLUDES)
DEFINES   = -DVERSION=$(VERSION) $(SYS_POLL_MISSING) $(BOOL_DEFINITION) $(EXPLICIT) $(AUTO_PTR_BROKEN)
CPPFLAGS  = $(INCLUDES) $(DEFINES)
CXXFLAGS  = $(CPPFLAGS) $(DEBUG)
LDFLAGS   = $(DEBUG)
LOADLIBES = $(LIBSTDCXX_LIBS) $(SOCKET_LIBRARIES)

.SUFFIXES : .C .o

.C.o :
	$(CXX) -c $(CXXFLAGS) $*.C

# -----------------------------------------------------------------------------

default : all

all : html2text
	@echo ;
	@echo 'Compilation completed. You may now move "html2text", "html2text.1.gz"';
	@echo 'and "html2textrc.5.gz" to their installation directories (e.g.';
	@echo '"/usr/local/bin", "/usr/local/man/man1" and "/usr/local/man/man5").';
	@echo

OBJS = html2text.o html.o HTMLControl.o HTMLParser.o Area.o format.o sgml.o table.o urlistream.o Properties.o cmp_nocase.o

html2text : $(OBJS) $(LIBSTDCXX_LIBS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LOADLIBES) $(LDLIBS) -o $@

libstd/libstd.a :
	cd libstd && $(MAKE)

# -----------------------------------------------------------------------------

# Since it is very unlikely that bison++-2.2 is installed (available on
# html2text's homepage), HTMLParser.h and HTMLParser.C are only built when
# 'make bison-local' is issued.

bison-local :
	cmp -s HTMLParser.h HTMLParser.k || cp HTMLParser.k HTMLParser.h;
	$(BISONXX) $(YFLAGS) -o HTMLParser.C -d -h HTMLParser.k HTMLParser.y

# -----------------------------------------------------------------------------

# This is mostly thought for RPM builts and users that don't read the documentation.

install :
	$(INSTALLER) -s -m 755 html2text $(BINDIR);
	$(INSTALLER) -m 644 html2text.1.gz $(MANDIR)/man1;
	$(INSTALLER) -m 644 html2textrc.5.gz $(MANDIR)/man5;
	$(INSTALLER) -d -m 755 $(DOCDIR);
	$(INSTALLER) -p -m 644 CHANGES COPYING CREDITS KNOWN_BUGS README RELEASE_NOTES TODO $(DOCDIR)

# -----------------------------------------------------------------------------

SUBDIRS = libstd

# "./configure" creates "Makefile"s only in the subdirectories that need to
# be built, so we check for the existance of these "Makefile".
clean clobber depend :
	@for i in $(SUBDIRS); do \
	  if test -r $$i/Makefile; then \
	    ( \
	      cd $$i && echo "*** make $@ in `pwd`" && $(MAKE) $@ || \
	      { echo "*** make $@ error in `pwd`" && false; } \
	    ) || exit 1; \
	    echo "*** Back in `pwd`"; \
	  fi; \
	done;

clean : local-clean
local-clean :
	rm -f *.o *~ core html2text;

clobber : local-clobber
local-clobber : local-clean
	rm -f html2text

depend : local-depend
local-depend : HTMLParser.h
	@>Dependencies
	makedepend -f Dependencies $(CPPFLAGS) $(MAKEDEPEND_INCLUDES) *.C
	@rm -f Dependencies.bak

# -----------------------------------------------------------------------------

include Dependencies

