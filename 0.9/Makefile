# Makefile for ePiX v0.9.x
#

### Modify these lines as appropriate ###

## Compiler ##
CXX=g++

## Install directory: uncomment exactly one ##
#prefix=/usr/local
#prefix=/opt/epix
prefix=$(HOME)/epix-0.9

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

## Source, object, and header files ##
SRCS=Vector.cc Pair.cc Camera.cc Color.cc functions.cc

OBJS=Vector.o Pair.o Camera.o Color.o functions.o

HDRS=constants.h Errors.h Position.h Vector.h Pair.h functions.h Camera.h Color.h

## Installed files ##
# Main wrapper script
EPIX=epix
EPIX_HDR=epix.h
EPIX_LIB=libepix.a
EPIX_MAN=epix.1

# bash script to convert latex to Postscript
LAPS=laps
# convert epix files to encapsulated Postscript
ELAPS=elaps

## Targets ##

:	$(EPIX_LIB) $(EPIX) $(EPIX_HDR)

all:	$(EPIX_LIB) $(EPIX) $(EPIX_HDR)

Position.o:	Position.cc Position.h
	$(CXX) $(CFLAGS) Position.cc

Vector.o:	Vector.cc Vector.h
	$(CXX) $(CFLAGS) Vector.cc

Pair.o:	Pair.cc Pair.h
	$(CXX) $(CFLAGS) Pair.cc

functions.o:	functions.cc functions.h
	$(CXX) $(CFLAGS) functions.cc

Camera.o:	Camera.cc Camera.h
	$(CXX) $(CFLAGS) Camera.cc

Color.o:	Color.cc Color.h
	$(CXX) $(CFLAGS) Color.cc

epix.h:	$(HDRS)
	./make_header

epix:	prepix
	export EPIX_ROOTDIR=${prefix} && \
	export EPIX_CXX=${CXX} && \
	export EPIX_SCRIPT=$(EPIX) && \
	./prepix && chmod 755 $(EPIX)

libepix.a:	$(OBJS)
	ar -ru $(EPIX_LIB) $(OBJS)
	$(RANLIB) $(EPIX_LIB)

# test:	$(EPIX_LIB) $(EPIX) $(EPIX_HDR)
# 	export EPIX_ROOTDIR="${prefix}" && ./helpfiles.sh --test

install:$(EPIX_LIB) $(EPIX) $(EPIX_HDR)
	if [ ! -d ${prefix} ]; then $(MKDIR) ${prefix}; fi
	export EPIX_ROOTDIR="${prefix}" && ./pre-install.sh
	export EPIX_ROOTDIR="${prefix}" && ./helpfiles.sh --install
	$(INSTALL_PROGRAM) $(EPIX_HDR) ${hdrdir}/$(EPIX_HDR)
	$(INSTALL_PROGRAM) $(EPIX_LIB) ${libdir}/$(EPIX_LIB)
	$(INSTALL_PROGRAM) $(EPIX_MAN) ${mandir}/$(EPIX_MAN)
	$(INSTALL_BIN) $(EPIX) ${bindir}/$(EPIX)
	$(INSTALL_BIN) $(LAPS) ${bindir}/$(LAPS)
	$(INSTALL_BIN) $(ELAPS) ${bindir}/$(ELAPS)

clean:
	-$(RM) $(OBJS) $(CONTRIB_OBJ) $(LEGACY_OBJ) $(EPIX_LIB) $(EPIX) \
	$(EPIX_HDR) sample.{dvi,ps} samples/{*.eepic,sample.{dvi,log}}

uninstall:
	-$(RM)	${hdrdir}/$(EPIX_HDR) \
		${libdir}/$(EPIX_LIB) \
		${mandir}/$(EPIX_MAN) \
		${bindir}/$(EPIX) \
		${bindir}/$(LAPS) \
		${bindir}/$(ELAPS)
	export EPIX_ROOTDIR="${prefix}" && ./helpfiles.sh --uninstall
	export EPIX_ROOTDIR="${prefix}" && ./post-uninst.sh
