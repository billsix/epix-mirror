# Makefile for ePiX-ext v0.8.x
#

### Modify these lines as appropriate ###

## Compiler ##
CXX=g++
#CXX=g++-2.95.3
#CXX=g++-3.0.1
#CXX=g++-3.2

############### Nothing below should require modification ###############

## Binutils ##
CFLAGS=-c -Wall
LINT=lint
RANLIB=ranlib

## Miscellaneous commands ##
CHMOD=chmod
MKDIR=mkdir
RM=rm -f

## Directories ##
srcdir=.

## Source, object, and header files ##

SRCS=CartesianCoord.cc CoordAxis.cc CoordSystem.cc DoubleLogCoord.cc \
	Hatch.cc LinearAxis.cc LogaritmicAxis.cc PicCommand.cc \
	SingleLogCoord.cc

HDRS=epix_ext.h
OBJS=CartesianCoord.o CoordAxis.o CoordSystem.o DoubleLogCoord.o \
	Hatch.o LinearAxis.o LogaritmicAxis.o PicCommand.o \
	SingleLogCoord.o

EPIX_LIB=../libepix.a
EPIX_HDR=../epix.h

GLOBALS=globals.h

## Targets ##

:	$(HDRS) $(OBJS)

all:	$(HDRS) $(OBJS)

CartesianCoord.o:	CartesianCoord.cc $(EPIX_HDR) $(HDR)
	$(CXX) $(CFLAGS) CartesianCoord.cc

CoordAxis.o:	CoordAxis.cc $(EPIX_HDR) $(HDR)
	$(CXX) $(CFLAGS) CoordAxis.cc

CoordSystem.o:	CoordSystem.cc $(EPIX_HDR) $(HDR)
	$(CXX) $(CFLAGS) CoordSystem.cc

DoubleLogCoord.o:	DoubleLogCoord.cc $(EPIX_HDR) $(HDR)
	$(CXX) $(CFLAGS) DoubleLogCoord.cc

Hatch.o:	Hatch.cc $(EPIX_HDR) $(HDR)
	$(CXX) $(CFLAGS) Hatch.cc

LinearAxis.o:	LinearAxis.cc $(EPIX_HDR) $(HDR)
	$(CXX) $(CFLAGS) LinearAxis.cc

LogaritmicAxis.o:	LogaritmicAxis.cc $(EPIX_HDR) $(HDR)
	$(CXX) $(CFLAGS) LogaritmicAxis.cc

PicCommand.o:	PicCommand.cc $(EPIX_HDR) $(HDR)
	$(CXX) $(CFLAGS) PicCommand.cc

SingleLogCoord.o:	SingleLogCoord.cc $(EPIX_HDR) $(HDR)
	$(CXX) $(CFLAGS) SingleLogCoord.cc


contrib:	$(EPIX_LIB) $(OBJS) $(EPIX_HDR)
	ar -ru $(EPIX_LIB) $(OBJS)
	$(RANLIB) $(EPIX_LIB)
	@cat $(HDRS) | grep -v "\#if" | grep -v "include \"" \
	     | grep -v endif >> $(EPIX_HDR)

clean:
	@-$(RM) $(OBJS)
