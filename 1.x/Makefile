# Makefile for ePiX v1.x
#

##############################################################
##############################################################
########                                              ########
########  Please edit the "config" file to configure  ########
########                                              ########
##############################################################
##############################################################

include ./config

## ePiX is bash-centric; apologies if you're not... ##

SHELL=/bin/sh
#SHELL=${EPIX_BASH_PATH}


## Directories (full paths) -- names set by config ##
bindir=${EPIX_ROOTDIR}/bin
libdir=${EPIX_ROOTDIR}/lib
hdrdir=${EPIX_ROOTDIR}/include
mandir=${EPIX_MAN_PATH}
shrdir=${EPIX_SHR_PATH}
docdir=${EPIX_DOC_PATH}

## Binutils ##
CFLAGS=-c -Wall
RANLIB=ranlib

## Miscellaneous commands ##
RM=rm -f
LINK=ln -sf

INSTALL_BIN=install -m 755
INSTALL_DATA=install -m 644


## Installed files ##
EPIX_HDR=epix-$(EPIX_VERSION).h
EPIX_LIB=libepix-$(EPIX_VERSION).a

# shell scripts
ELAPS=elaps
EPIX=epix
FLIX=flix
LAPS=laps

KEYWORDS=keywords

# docs, samples
EPIX_MANUAL=epix-${EPIX_VERSION}_manual
EPIX_MANUAL_TAR=${EPIX_MANUAL}_src.tar

EPIX_SAMPLES=epix-${EPIX_VERSION}_sample
EPIX_SAMPLES_TAR=${EPIX_SAMPLES}_src.tar

EPIX_CONTRIB=epix-${EPIX_VERSION}_contrib
EPIX_CONTRIB_TAR=${EPIX_CONTRIB}_src.tar

EPIX_MAN=epix.1

# notes, configs
EPIX_CONFFILES=bash_completions config epix.ebuild epix.el epix.spec
EPIX_NOTEFILES=BUGS CHANGELOG COPYING INSTALL POST-INSTALL \
	README README-changes README-porting THANKS

## Source, object, and header files ##
CONFIG=config
EPIX_FCNS=epix_functions

SRCS=triples.cc domain.cc segment.cc circle.cc lengths.cc camera.cc \
	cropping.cc path.cc sphere.cc plane.cc polyhedron.cc output.cc \
	Label.cc objects.cc curves.cc plots.cc dataplot.cc geometry.cc \
	functions.cc

OBJS=triples.o domain.o segment.o circle.o lengths.o camera.o cropping.o \
	path.o sphere.o plane.o polyhedron.o output.o Label.o objects.o \
	curves.o plots.o dataplot.o geometry.o functions.o

HDRS=globals.h triples.h domain.h map.h pairs.h segment.h circle.h lengths.h \
	camera.h cropping.h path.h sphere.h plane.h polyhedron.h output.h \
	Label.h objects.h curves.h plots.h dataplot.h geometry.h functions.h

# header groups for build deps
GLOBALS=globals.h triples.h functions.h map.h pairs.h
OUTPUTS=camera.h cropping.h lengths.h output.h 
OBJECTS=Label.h sphere.h segment.h circle.h plane.h polyhedron.h objects.h
PATHS=domain.h path.h curves.h plots.h dataplot.h


## Targets ##
:	$(CONFIG)
	@if [ ${EPIX_BASH_PATH} != /bin/bash ]; then ./newbash.sh; fi
	make $(EPIX_LIB) $(EPIX_HDR) $(EPIX) $(ELAPS) $(FLIX) $(LAPS)

all:	$(CONFIG)
	@if [ ${EPIX_BASH_PATH} != /bin/bash ]; then ./newbash.sh; fi
	make $(EPIX_LIB) $(EPIX_HDR) $(EPIX) $(ELAPS) $(FLIX) $(LAPS)

# epix.h
$(EPIX_HDR):	$(HDRS) $(CONFIG)
	@./make_header

# libepix.a
$(EPIX_LIB):	$(OBJS)
	ar -ru $(EPIX_LIB) $(OBJS)
	$(RANLIB) $(EPIX_LIB)

# shell scripts
$(ELAPS):	make_elaps $(EPIX_FCNS) $(CONFIG)
	@./make_elaps

$(EPIX):	make_epix $(EPIX_FCNS) $(CONFIG)
	@./make_epix

$(FLIX):	make_flix $(EPIX_FCNS) $(CONFIG)
	@./make_flix

$(LAPS):	make_laps $(EPIX_FCNS) $(CONFIG)
	@./make_laps

contrib:	$(EPIX_LIB)
	@-$(RM) epix.h $(EPIX_HDR)
	@./make_header
	@$(LINK) $(EPIX_HDR) epix.h
	@$(LINK) $(EPIX_LIB) libepix.a
	cd contrib; make contrib; cd ..
	make contrib-doc

# the pieces...
triples.o:	triples.cc $(GLOBALS) triples.h
	$(CXX) $(CFLAGS) triples.cc

domain.o:	domain.cc $(GLOBALS) domain.h
	$(CXX) $(CFLAGS) domain.cc

segment.o:	segment.cc $(GLOBALS) segment.h
	$(CXX) $(CFLAGS) segment.cc

circle.o:	circle.cc $(GLOBALS) $(OUTPUTS) $(OBJECTS) circle.h
	$(CXX) $(CFLAGS) circle.cc

lengths.o:	lengths.cc $(GLOBALS) $(OUTPUTS) lengths.h
	$(CXX) $(CFLAGS) lengths.cc

camera.o:	camera.cc $(GLOBALS) $(OUTPUTS) camera.h
	$(CXX) $(CFLAGS) camera.cc

cropping.o:	cropping.cc $(GLOBALS) cropping.h
	$(CXX) $(CFLAGS) cropping.cc

path.o:	path.cc $(GLOBALS) $(OUTPUTS) path.h
	$(CXX) -O2 $(CFLAGS) path.cc

sphere.o:	sphere.cc $(GLOBALS) $(OBJECTS) sphere.h
	$(CXX) $(CFLAGS) sphere.cc

plane.o:	plane.cc $(GLOBALS) $(OBJECTS) plane.h
	$(CXX) $(CFLAGS) plane.cc

polyhedron.o:	polyhedron.cc $(GLOBALS) $(OBJECTS) polyhedron.h
	$(CXX) $(CFLAGS) polyhedron.cc

output.o:	output.cc $(GLOBALS) $(OUTPUTS) output.h
	$(CXX) -O2 $(CFLAGS) output.cc

Label.o:	Label.cc $(GLOBALS) $(OBJECTS) Label.h
	$(CXX) $(CFLAGS) Label.cc

objects.o:	objects.cc $(GLOBALS) $(OUTPUTS) $(OBJECTS) objects.h
	$(CXX) $(CFLAGS) objects.cc

curves.o:	curves.cc $(GLOBALS) $(OUTPUTS) $(OBJECTS) $(PATHS) curves.h
	$(CXX) -O2 $(CFLAGS) curves.cc

plots.o:	plots.cc $(GLOBALS) $(OUTPUTS) $(OBJECTS) $(PATHS) plots.h
	$(CXX) -O2 $(CFLAGS) plots.cc

dataplot.o: dataplot.cc $(GLOBALS) $(OUTPUTS) $(OBJECTS) $(PATHS) dataplot.h
	$(CXX) -O2 $(CFLAGS) dataplot.cc

geometry.o:	geometry.cc $(GLOBALS) $(OUTPUTS) $(OBJECTS) $(PATHS) \
	geometry.h
	$(CXX) -O2 $(CFLAGS) geometry.cc

functions.o:	functions.cc $(GLOBALS) functions.h
	$(CXX) $(CFLAGS) functions.cc

# documentation
doc:	$(EPIX_LIB) $(EPIX_HDR) $(EPIX) $(LAPS) $(ELAPS) $(FLIX)
	@./helpfiles.sh --doc

samples:$(EPIX_LIB) $(EPIX_HDR) $(EPIX) $(LAPS) $(ELAPS) $(FLIX)
	@./helpfiles.sh --samples

contrib-doc:$(EPIX_LIB) $(EPIX_HDR) $(EPIX) $(LAPS) $(ELAPS) $(FLIX)
	@./helpfiles.sh --contrib-doc

test:	
	@make doc samples

# (un)install, clean
install:$(EPIX_LIB) $(EPIX_HDR) $(EPIX) $(FLIX) $(ELAPS) $(LAPS)
	@./pre-install.sh

	$(INSTALL_DATA) $(EPIX_HDR) ${hdrdir}/$(EPIX_HDR)
	$(INSTALL_DATA) $(EPIX_LIB) ${libdir}/$(EPIX_LIB)

	$(INSTALL_BIN) $(EPIX) ${bindir}/$(EPIX)
	$(INSTALL_BIN) $(LAPS) ${bindir}/$(LAPS)
	$(INSTALL_BIN) $(ELAPS) ${bindir}/$(ELAPS)
	$(INSTALL_BIN) $(FLIX) ${bindir}/$(FLIX)
	$(INSTALL_BIN) $(KEYWORDS) ${bindir}/$(KEYWORDS)

	$(INSTALL_DATA) ${EPIX_NOTEFILES} ${EPIX_NOTE_PATH}
	$(INSTALL_DATA) ${EPIX_CONFFILES} ${EPIX_CONF_PATH}
	$(INSTALL_DATA) $(EPIX_MAN) ${mandir}/$(EPIX_MAN)

	@if [ -f ${EPIX_MANUAL}.ps.gz ]; then \
	$(INSTALL_DATA) \
		${EPIX_MANUAL}.ps.gz \
		${EPIX_MANUAL}.pdf.gz \
		${EPIX_MANUAL_TAR}.gz ${EPIX_DOC_PATH}; \
	fi

	@if [ -f ${EPIX_SAMPLES}.ps.gz ]; then \
	$(INSTALL_DATA) \
		${EPIX_SAMPLES}.ps.gz \
		${EPIX_SAMPLES}.pdf.gz \
		${EPIX_SAMPLES_TAR}.gz ${EPIX_DOC_PATH}; \
	fi

	$(RANLIB) ${libdir}/$(EPIX_LIB)
	$(LINK) ${hdrdir}/$(EPIX_HDR) ${hdrdir}/epix.h
	$(LINK) ${libdir}/$(EPIX_LIB) ${libdir}/libepix.a

	@if [ -f ${EPIX_CONTRIB_TAR}.gz ]; then \
	$(INSTALL_DATA) ${EPIX_CONTRIB_TAR}.gz ${EPIX_DOC_PATH}; \
	fi

doc-clean:
	@-$(RM) $(LAPS) doc/*.{c,dvi,log} doc/{koch,plot_?,torus}.eepic \
	doc/{epix.h,libepix.a} epix-*manual*.gz

samples-clean:
	@-$(RM) $(LAPS) samples/*.{c,dvi,eepic,log,tex} \
	samples/{epix.h,libepix.a} epix-*sample*.gz

contrib-doc-clean:
	@-$(RM) epix-*contrib*.gz doc/contrib/*.{dvi,eepic,log}

clean:
	@-$(RM) $(OBJS)	epix.h libepix.a \
	$(EPIX_LIB) $(EPIX) $(FLIX) $(ELAPS) $(LAPS) $(EPIX_HDR)
	make doc-clean
	make samples-clean
	make contrib-doc-clean
	cd contrib; make clean

uninstall:
	@echo "Uninstalling ePiX-$(EPIX_VERSION)"
	@-$(RM)	${hdrdir}/epix.h \
		${libdir}/libepix.a \
		${hdrdir}/$(EPIX_HDR) \
		${libdir}/$(EPIX_LIB) \
		${mandir}/$(EPIX_MAN) \
		${shrdir}/{config,notes}/* \
		${docdir}/epix-$(EPIX_VERSION)* \
		${bindir}/$(EPIX) \
		${bindir}/$(LAPS) \
		${bindir}/$(ELAPS) \
		${bindir}/$(FLIX) \
		${bindir}/$(KEYWORDS)
	@./post-uninst.sh


uninstall-all:
	@echo -n "Uninstalling all versions of ePiX..."
	@-$(RM)	${hdrdir}/epix*.h \
		${libdir}/libepix*.a \
		${mandir}/$(EPIX_MAN) \
		${shrdir}/{config,notes}/* \
		${docdir}/epix-* \
		${bindir}/$(EPIX) \
		${bindir}/$(LAPS) \
		${bindir}/$(ELAPS) \
		${bindir}/$(FLIX) \
		${bindir}/$(KEYWORDS)
	@./post-uninst.sh
	@echo "done. Sorry it didn't work out."
