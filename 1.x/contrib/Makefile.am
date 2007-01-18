# Makefile.am for ePiX-1.x "contrib" directory
#

libepixext_a_CXXSOURCES = CartesianCoord.cc CoordAxis.cc CoordSystem.cc \
	DoubleLogCoord.cc Hatch.cc LinearAxis.cc LogaritmicAxis.cc \
	PicCommand.cc SingleLogCoord.cc
libepixext_a_HDRS = epix_ext.h
libepixext_a_SOURCES = $(libepixext_a_CXXSOURCES) $(libepixext_a_HDRS)

libepixext_a_LIBADD = $(libepixext_a_CXXSOURCES:.cc=.o)

pkglib_LIBRARIES = libepixext.a
include_HEADERS = $(libepixext_a_HDRS)

CLEANFILES = *~ $(libepixext_a_OBJECTS)

INCLUDES = -I..

## Manual Targets ##

libepixext.a: ../epix.h $(libepixext_a_OBJECTS)
	ar -ru $@ $(libepixext_a_OBJECTS)
	$(RANLIB) $@

# object file dependencies
$(libepixext_a_LIBADD): %.o: %.cc $(libepixext_a_HEADERS) ../epix.h
