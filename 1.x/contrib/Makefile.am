# Makefile.am for ePiX-1.x "contrib" directory
#
noinst_LIBRARIES   = libepixext.a
libepixext_a_SOURCES  = CartesianCoord.cc CoordAxis.cc CoordSystem.cc \
	DoubleLogCoord.cc Hatch.cc LinearAxis.cc LogaritmicAxis.cc \
	PicCommand.cc SingleLogCoord.cc

libepixext_a_LIBADD=CartesianCoord.o CoordAxis.o CoordSystem.o \
	DoubleLogCoord.o Hatch.o LinearAxis.o LogaritmicAxis.o PicCommand.o \
	SingleLogCoord.o


EXT_HEADER=epix_ext.h
LN_SF = ln -sf

EPIX_LIB=libepix-${VERSION}.a

## Manual Targets ##
all:	epix.h libepix.a ${OBJECTS}
	ar -ru ../${EPIX_LIB} ${OBJECTS}
	ranlib ../${EPIX_LIB}

.PHONY: all epix.h libepix.a clean clean-all

epix.h:
	@$(LN_SF) ../epix.h .

libepix.a:
	@$(LN_SF) ../libepix.a .


CartesianCoord.o:	CartesianCoord.cc	${EXT_HEADER}
CoordAxis.o:		CoordAxis.cc		${EXT_HEADER}
CoordSystem.o:		CoordSystem.cc		${EXT_HEADER}
DoubleLogCoord.o:	DoubleLogCoord.cc	${EXT_HEADER}
Hatch.o:		Hatch.cc		${EXT_HEADER}
LinearAxis.o:		LinearAxis.cc		${EXT_HEADER}
LogaritmicAxis.o:	LogaritmicAxis.cc	${EXT_HEADER}
PicCommand.o:		PicCommand.cc		${EXT_HEADER}
SingleLogCoord.o:	SingleLogCoord.cc	${EXT_HEADER}


clean:
	@-rm -f ${OBJECTS} epix.h libepix.a

clean-all:
	@-rm -f ${OBJECTS} epix.h libepix.a