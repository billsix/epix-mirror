# Makefile for ePiX v0.8.x
#

### Modify these lines as appropriate ###

## Compiler ##
CXX=g++
#CXX=g++-2.95.3
#CXX=g++-3.0.1
#CXX=g++-3.2

## Install directory: uncomment exactly one ##
prefix=/usr/local
#prefix=/opt/epix
#prefix=$(HOME)

## ePiX is bash-centric; apologies if you're not... ##
SHELL=/bin/bash

############### Nothing below should require modification ###############

## Binutils ##
CFLAGS=-c -Wall
LINT=lint
RANLIB=ranlib

## Miscellaneous commands ##
CHMOD=chmod
MKDIR=mkdir
RM=rm -f

INSTALL=install
INSTALL_BIN=${INSTALL} -m 755
INSTALL_PROGRAM=${INSTALL} -m 644

## Directories ##
srcdir=.

bindir=${prefix}/bin
libdir=${prefix}/lib
hdrdir=${prefix}/include
mandir=${prefix}/man/man1
shrdir=${prefix}/share/epix

LEGACY_HDR=legacy.h

## Source, object, and header files ##
SRCS=triples.cc domain.cc segment.cc circle.cc lengths.cc camera.cc \
	cropping.cc path.cc sphere.cc plane.cc output.cc Label.cc \
	objects.cc curves.cc plots.cc dataplot.cc geometry.cc functions.cc

OBJS=triples.o domain.o segment.o circle.o lengths.o camera.o cropping.o \
	path.o sphere.o plane.o output.o Label.o objects.o curves.o plots.o \
	dataplot.o geometry.o functions.o

HDRS=globals.h triples.h domain.h map.h pairs.h segment.h circle.h lengths.h \
	camera.h cropping.h path.h sphere.h plane.h output.h \
	Label.h objects.h curves.h plots.h dataplot.h geometry.h functions.h

GLOBALS=globals.h triples.h functions.h map.h pairs.h
OUTPUTS=camera.h cropping.h lengths.h output.h 
OBJECTS=Label.h sphere.h segment.h circle.h plane.h objects.h
PATHS=domain.h path.h curves.h plots.h dataplot.h

## Installed files ##
# Main wrapper script
EPIX=epix
EPIX_HDR=epix.h
EPIX_LIB=libepix.a
EPIX_MAN=epix.1

# bash script to convert latex to Postscript
LAPS=laps
# convert epix/eepic files to encapsulated Postscript
ELAPS=elaps
# convert epix files to mng animation
FLIX=flix

# lookup script for C/C++ keywords
KEYWORDS=keywords

## Targets ##

:	$(EPIX_LIB) $(EPIX) $(EPIX_HDR)

all:	$(EPIX_LIB) $(EPIX) $(EPIX_HDR)

libepix.a:	$(OBJS)
	ar -ru $(EPIX_LIB) $(OBJS)
	$(RANLIB) $(EPIX_LIB)

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

output.o:	output.cc $(GLOBALS) $(OUTPUTS) output.h
	$(CXX) -O2 $(CFLAGS) output.cc

# Style.o:	Style.cc $(GLOBALS) Style.h
#	$(CXX) $(CFLAGS) Style.cc

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

epix.h:	$(HDRS)
	@./make_header

epix:	prepix
	@export EPIX_ROOTDIR=${prefix} && \
	export EPIX_CXX=${CXX} && \
	export EPIX_SCRIPT=$(EPIX) && \
	./prepix && chmod 755 $(EPIX)

flix:	preflix
	@export EPIX_ROOTDIR=${prefix} && \
	export EPIX_CXX=${CXX} && \
	export FLIX_SCRIPT=$(FLIX) && \
	./preflix && chmod 755 $(FLIX)

contrib:	$(EPIX_LIB) $(EPIX_HDR)
	cd contrib; make contrib

legacy:	$(EPIX_HDR) $(LEGACY_HDR)
	@echo "Making legacy target. DO NOT use legacy features in new files!"
	@cat $(LEGACY_HDR) | grep -v "\#if" | grep -v "include \"" \
	| grep -v endif >> $(EPIX_HDR)


test:	$(EPIX_LIB) $(EPIX) $(EPIX_HDR) $(FLIX)
	@export EPIX_ROOTDIR="${prefix}" && export EPIX_CXX=${CXX} && \
		./helpfiles.sh --test

install:$(EPIX_LIB) $(EPIX) $(EPIX_HDR) $(FLIX)
	@echo -n "Preparing samples and documentation..."
	@if [ ! -d ${prefix} ]; then $(MKDIR) ${prefix}; fi
	@export EPIX_ROOTDIR="${prefix}" && ./pre-install.sh
	@export EPIX_ROOTDIR="${prefix}" && ./helpfiles.sh --install
	@echo "done"
	$(INSTALL_PROGRAM) $(EPIX_HDR) ${hdrdir}/$(EPIX_HDR)
	$(INSTALL_PROGRAM) $(EPIX_LIB) ${libdir}/$(EPIX_LIB)
	$(INSTALL_PROGRAM) $(EPIX_MAN) ${mandir}/$(EPIX_MAN)
	$(INSTALL_BIN) $(EPIX) ${bindir}/$(EPIX)
	$(INSTALL_BIN) $(LAPS) ${bindir}/$(LAPS)
	$(INSTALL_BIN) $(ELAPS) ${bindir}/$(ELAPS)
	$(INSTALL_BIN) $(FLIX) ${bindir}/$(FLIX)
	$(INSTALL_BIN) $(KEYWORDS) ${bindir}/$(KEYWORDS)

clean:
	@-$(RM) $(OBJS) $(LEGACY_OBJ) $(EPIX_LIB) $(EPIX) $(FLIX) \
	$(EPIX_HDR) sample.{dvi,ps} samples/{*.eepic,sample.{dvi,log,tex}}
	cd contrib; make clean

uninstall:
	@echo -n "Uninstalling..."
	@-$(RM)	${hdrdir}/$(EPIX_HDR) \
		${libdir}/$(EPIX_LIB) \
		${mandir}/$(EPIX_MAN) \
		${bindir}/$(EPIX) \
		${bindir}/$(LAPS) \
		${bindir}/$(ELAPS) \
		${bindir}/$(FLIX) \
		${bindir}/$(KEYWORDS)
	@export EPIX_ROOTDIR="${prefix}" && ./helpfiles.sh --uninstall
	@export EPIX_ROOTDIR="${prefix}" && ./post-uninst.sh
	@echo "done. Sorry it didn't work out."
