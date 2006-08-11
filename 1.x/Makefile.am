## epix-1.x Makefile.am for top-level directory
##
## Process this file with automake to produce Makefile.in

if BUILD_CONTRIB
  # Must be in this order!
  MAYBE_DIRS = contrib contrib/doc
endif

SUBDIRS = . samples doc $(MAYBE_DIRS)

# Now stuff for this directory specifically

bin_SCRIPTS = epix elaps flix laps

pkglib_LIBRARIES = libepix.a
pkglib_DATA = epix-lib.sh
include_HEADERS = epix.h

libepix_a_CXXSOURCES = camera.cc circle.cc cropping.cc curves.cc \
	dataplot.cc domain.cc functions.cc geometry.cc Label.cc lengths.cc \
	objects.cc output.cc pairs.cc path.cc plane.cc plots.cc polyhedron.cc \
	segment.cc sphere.cc surface.cc triples.cc
# header groups for build deps
H_GLOBALS = globals.h triples.h functions.h map.h pairs.h
H_OUTPUTS = camera.h cropping.h lengths.h output.h
H_OBJECTS = Label.h sphere.h segment.h circle.h plane.h polyhedron.h objects.h
H_PATHS = domain.h path.h curves.h plots.h dataplot.h
H_EXTRAS = geometry.h
libepix_a_HDRS = $(H_GLOBALS) $(H_OUTPUTS) $(H_OBJECTS) $(H_PATHS) \
	$(H_EXTRAS)

libepix_a_SOURCES = $(libepix_a_CXXSOURCES) $(libepix_a_HDRS)

libepix_a_LIBADD = $(libepix_a_CXXSOURCES:.cc=.o)

# Makefile for ePiX v1.x
#

man_MANS = epix.1 elaps.1 flix.1 laps.1

EPIX_CONFFILES = bash_completions epix.ebuild epix.el epix.spec
EPIX_NOTEFILES = AUTHORS BUGS ChangeLog COPYING INSTALL NEWS POST-INSTALL \
	README README-changes README-porting THANKS VERSION

EXTRA_DIST = $(bin_SCRIPTS) $(man_MANS) $(EPIX_CONFFILES) $(EPIX_NOTEFILES)

CLEANFILES = *~ $(libepix_a_OBJECTS) $(bin_SCRIPTS) $(bin_SCRIPTS:=-local) \
	epix.h epix.1

# Headers
epix.h: $(libepix_a_HDRS)
	./make_header

# Scripts; we create local variants which will work during the build process
epix elaps flix laps: %: %.in
	sed -e 's%^#! /bin/bash%#! $(BASH_PATH)%;' \
	    -e 's%@bindir\@%$(bindir)%;' \
	    -e 's%@pkglibdir\@%$(pkglibdir)%;' \
	    -e 's%@includedir\@%$(includedir)%;' \
	    -e 's%@RUNTIME_COMPILER\@%$(RUNTIME_COMPILER)%;' $< > $@
	sed -e 's%^#! /bin/bash%#! $(BASH_PATH)%;' \
	    -e 's%@bindir\@%$(CURDIR)%;' \
	    -e 's%@pkglibdir\@%$(CURDIR)%;' \
	    -e 's%@includedir\@%$(CURDIR)%;' \
	    -e 's%@RUNTIME_COMPILER\@%$(CXX)%;' $< > $@-local
	chmod a+x $@ $@-local

# libepix.a
libepix.a: $(libepix_a_OBJECTS)
	ar -ru libepix.a $(libepix_a_OBJECTS)
	$(RANLIB) libepix.a

# object file dependencies
$(libepix_a_LIBADD): %.o: %.cc %.h $(H_GLOBALS)

# These ones have extra dependencies
Label.o camera.o cropping.o functions.o lengths.o : $(H_OUTPUTS)
path.o segment.o : $(H_OUTPUTS)

segment.o : $(H_PATHS)

# And these depend on all of the groups of header files
circle.o curves.o dataplot.o geometry.o objects.o : $(libepix_a_HDRS)
output.o plots.o plane.o polyhedron.o sphere.o : $(libepix_a_HDRS)

# Manpage
epix.1: epix.1.in
	sed -e 's%@docdir\@%$(docdir)%;' $< > $@

# Extra things to install
install-data-local:
	@echo; echo "Installing notes and configuration files"
	./mkinstalldirs $(DESTDIR)$(docdir)/notes $(DESTDIR)$(docdir)/config
	$(INSTALL_DATA) $(EPIX_NOTEFILES) $(DESTDIR)$(docdir)/notes
	$(INSTALL_DATA) $(EPIX_CONFFILES) $(DESTDIR)$(docdir)/config

# This is being installed by install-exec, but should not be executable
install-exec-local: install-pkglibDATA
	chmod a-x $(DESTDIR)$(pkglibdir)/epix-lib.sh

uninstall-local:
	-rm -rf $(DESTDIR)$(docdir)/notes $(DESTDIR)$(docdir)/config
	-rmdir $(DESTDIR)$(docdir)
