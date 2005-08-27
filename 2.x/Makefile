# Makefile for ePiX 2.0
#

### Modify these lines as appropriate ###

## Compiler ##
CXX=g++

## ePiX is bash-centric; apologies if you're not... ##
SHELL=/bin/bash

############### Nothing below should require modification ###############

## Binutils ##
CFLAGS=-c -Wall
RANLIB=ranlib

EPIX_HDR=epix2.h
EPIX_LIB=libepix2.a

## Source, object, and header files ##
SRCS=Pair.cc Functions.cc Color.cc Visibility.cc \
	Edge.cc Screen.cc Tile.cc Domain.cc \
	Vector.cc Mapping.cc Basis.cc Hiding.cc Camera.cc \
	Output.cc Object.cc World.cc \
	Polyhedra.cc Triangle.cc Quad.cc Segment.cc Disk.cc Sphere.cc \
	Tetra.cc Cube.cc Octa.cc 

OBJS=Pair.o Functions.o Color.o Visibility.o \
	Edge.o Screen.o Tile.o Domain.o \
	Vector.o Mapping.o Basis.o Hiding.o Camera.o \
	Output.o Object.o World.o \
	Polyhedra.o Triangle.o Quad.o Segment.o Disk.o Sphere.o \
	Tetra.o Cube.o Octa.o 


HDRS=Errors.h Constants.h Pair.h Functions.h Color.h Point.h Visibility.h \
	Edge.h Screen.h Tile.h Domain.h Vector.h Mapping.h Basis.h Hiding.h \
	Camera.h Object.h Output.h World.h Output.h \
	Polyhedra.h Triangle.h Quad.h Segment.h Disk.h Sphere.h \
	Tetra.h Cube.h Octa.h 


## Targets ##

:	$(EPIX_LIB) $(EPIX_HDR)

all:	$(EPIX_LIB) $(EPIX_HDR)

Pair.o:	Pair.cc Pair.h
	$(CXX) $(CFLAGS) Pair.cc

Functions.o:	Functions.cc Functions.h Constants.h Errors.h
	$(CXX) $(CFLAGS) Functions.cc

Domain.o:Domain.cc Domain.h Point.h
	$(CXX) $(CFLAGS) Domain.cc

Color.o:Color.cc Color.h Enums.h
	$(CXX) $(CFLAGS) Color.cc

Edge.o:	Edge.cc Edge.h Pair.h Point.h Color.h
	$(CXX) $(CFLAGS) Edge.cc

Screen.o:	Screen.cc Screen.h Pair.h Functions.h Output.h
	$(CXX) $(CFLAGS) Screen.cc

Tile.o:	Tile.cc Tile.h Pair.h
	$(CXX) $(CFLAGS) Tile.cc

Vector.o:	Vector.cc Vector.h Point.h Constants.h Errors.h
	$(CXX) $(CFLAGS) Vector.cc

Visibility.o:	Visibility.cc Visibility.h Point.h
	$(CXX) $(CFLAGS) Visibility.cc

Mapping.o:	Mapping.cc Mapping.h Point.h
	$(CXX) $(CFLAGS) Mapping.cc

Basis.o:	Basis.cc Basis.h Vector.h Point.h Errors.h Functions.h
	$(CXX) $(CFLAGS) Basis.cc

Hiding.o:	Hiding.cc Hiding.h Vector.h Point.h
	$(CXX) $(CFLAGS) Hiding.cc

Camera.o:	Camera.cc Camera.h Basis.h Vector.h Point.h Pair.h \
	Constants.h
	$(CXX) $(CFLAGS) Camera.cc

Object.o:Object.cc Object.h Enums.h Color.h Point.h Vector.h Basis.h
	$(CXX) $(CFLAGS) Object.cc

Output.o:Output.cc Output.h Color.h
	$(CXX) $(CFLAGS) Output.cc

Segment.o:	Segment.cc Segment.h Object.h
	$(CXX) $(CFLAGS) Segment.cc

Polyhedra.o:	Polyhedra.cc Polyhedra.h Object.h
	$(CXX) $(CFLAGS) Polyhedra.cc

Triangle.o:	Triangle.cc Triangle.h Object.h
	$(CXX) $(CFLAGS) Triangle.cc

Quad.o:	Quad.cc Quad.h Object.h
	$(CXX) $(CFLAGS) Quad.cc

Disk.o:	Disk.cc Disk.h Object.h
	$(CXX) $(CFLAGS) Disk.cc

Sphere.o:	Sphere.cc Sphere.h Object.h Triangle.h Quad.h
	$(CXX) $(CFLAGS) Sphere.cc

Tetra.o:	Tetra.cc Tetra.h Object.h Triangle.h
	$(CXX) $(CFLAGS) Tetra.cc

Cube.o:	Cube.cc Cube.h Object.h Quad.h
	$(CXX) $(CFLAGS) Cube.cc

Octa.o:	Octa.cc Octa.h Object.h Triangle.h
	$(CXX) $(CFLAGS) Octa.cc

World.o:	World.cc World.h \
	Pair.h Point.h Screen.h Camera.h Object.h Hiding.h Tile.h
	$(CXX) $(CFLAGS) World.cc

epix2.h:$(HDRS)
	./make_header2

libepix2.a:	$(OBJS)
	ar -ru $(EPIX_LIB) $(OBJS)
	$(RANLIB) $(EPIX_LIB)

out-test:	$(EPIX_LIB) $(EPIX_HDR) test.cc
	@g++ -I. -L. test.cc -lepix2 -lm && ./a.out

speed-test:	$(EPIX_LIB) $(EPIX_HDR) speed_test.cc
	@g++ -I. -L. speed_test.cc -lepix2 -lm && ./a.out > speed_test.eepic

test:	$(EPIX_LIB) $(EPIX_HDR) test.cc
	@g++ -I. -L. test.cc -lepix2 -lm && \
	./a.out > test.eepic && elaps test.eepic

clean:
	-$(RM) $(OBJS) a.out
#	$(EPIX_LIB) $(EPIX_HDR)