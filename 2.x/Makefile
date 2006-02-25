# Makefile for ePiX 2.0
#

### Modify these lines as appropriate ###

## Build compiler ##
CXX=g++

############### Nothing below should require modification ###############

RANLIB=ranlib

EPIX_HDR=epix2.h
EPIX_LIB=libepix2.a

## Directories
SRC_INT=src/Internals
SRC_OUT=src/Output
SRC_PIC=src/Picture
SRC_WOR=src/World


## Header files ##
HDRS_INT=$(SRC_INT)/Constants.h $(SRC_INT)/Errors.h $(SRC_INT)/Handle.h \
	$(SRC_INT)/Visibility.h $(SRC_INT)/Chop.h \
	$(SRC_INT)/Length.h $(SRC_INT)/Pair.h $(SRC_INT)/Functions.h \
	$(SRC_INT)/Pair_Map.h $(SRC_INT)/Color.h $(SRC_INT)/Point.h \
	$(SRC_INT)/Vector.h $(SRC_INT)/Basis.h $(SRC_INT)/Camera.h \
	$(SRC_INT)/Styles.h

HDRS_PIC=$(SRC_PIC)/Tile.h $(SRC_PIC)/Text2.h $(SRC_PIC)/Screen.h \
	$(SRC_PIC)/Picture.h $(SRC_PIC)/Silhouette.h

HDRS_WOR=$(SRC_WOR)/Text_Base.h $(SRC_WOR)/Object_Rep.h $(SRC_WOR)/Object.h \
	$(SRC_WOR)/Triangle.h $(SRC_WOR)/World.h

HDRS_OUT=$(SRC_OUT)/Output.h $(SRC_OUT)/PSTricks.h $(SRC_OUT)/PS.h

HDRS=$(HDRS_INT) $(HDRS_PIC) $(HDRS_OUT) $(HDRS_WOR)


## Source files ##
SRCS_INT=$(SRC_INT)/Visibility.cc $(SRC_INT)/Length.cc $(SRC_INT)/Pair.cc \
	$(SRC_INT)/Functions.cc $(SRC_INT)/Chop.cc \
	$(SRC_INT)/Pair_Map.cc $(SRC_INT)/Color.cc $(SRC_INT)/Point.cc \
	$(SRC_INT)/Vector.cc $(SRC_INT)/Basis.cc $(SRC_INT)/Camera.cc \
	$(SRC_INT)/Styles.cc

SRCS_PIC=$(SRC_PIC)/Text2.cc $(SRC_PIC)/Screen.cc \
	$(SRC_PIC)/Picture.cc $(SRC_PIC)/Outline.cc $(SRC_PIC)/Silhouette.cc

SRCS_WOR=$(SRC_WOR)/Text_Base.cc $(SRC_WOR)/Object_Rep.cc $(SRC_WOR)/Object.cc \
	$(SRC_WOR)/Triangle.cc $(SRC_WOR)/World.cc

SRCS_OUT=$(SRC_OUT)/Output.cc $(SRC_OUT)/PSTricks.cc $(SRC_OUT)/PS.cc

SRCS=$(SRCS_INT) $(SRCS_PIC) $(SRCS_OUT) $(SRCS_WOR)


## Object files ##
OBJS_INT=$(SRC_INT)/Visibility.o $(SRC_INT)/Length.o $(SRC_INT)/Pair.o \
	$(SRC_INT)/Functions.o $(SRC_INT)/Chop.o \
	$(SRC_INT)/Pair_Map.o $(SRC_INT)/Color.o $(SRC_INT)/Point.o \
	$(SRC_INT)/Vector.o $(SRC_INT)/Basis.o $(SRC_INT)/Camera.o \
	$(SRC_INT)/Styles.o

OBJS_PIC=$(SRC_PIC)/Text2.o $(SRC_PIC)/Screen.o \
	$(SRC_PIC)/Picture.o $(SRC_PIC)/Outline.o $(SRC_PIC)/Silhouette.o

OBJS_WOR=$(SRC_WOR)/Text_Base.o $(SRC_WOR)/Object_Rep.o $(SRC_WOR)/Object.o \
	$(SRC_WOR)/Triangle.o $(SRC_WOR)/World.o

OBJS_OUT=$(SRC_OUT)/Output.o $(SRC_OUT)/PSTricks.o $(SRC_OUT)/PS.o

OBJS=$(OBJS_INT) $(OBJS_PIC) $(OBJS_OUT) $(OBJS_WOR)


## Targets ##

:	$(EPIX_LIB) $(EPIX_HDR)
# $(OBJS)

all:	$(EPIX_LIB) $(EPIX_HDR)


Internals:$(HDRS_INT) $(SRCS_INT)
	cd $(SRC_INT) && make

Output:$(HDRS_OUT) $(SRCS_OUT)
	cd $(SRC_OUT) && make

Picture:$(HDRS_PIC) $(SRCS_PIC)
	cd $(SRC_PIC) && make

World:$(HDRS_WOR) $(SRCS_WOR)
	cd $(SRC_WOR) && make

epix2.h:$(HDRS) make_header2
	./make_header2

libepix2.a:	Internals Output Picture World
#	$(HDRS) $(SRCS) $(OBJS)
	ar -ru $(EPIX_LIB) $(OBJS)
	$(RANLIB) $(EPIX_LIB)

out_test:$(EPIX_LIB) $(EPIX_HDR) $(OBJS)
	cd Test && make out_test

src_test:$(EPIX_LIB) $(EPIX_HDR) $(OBJS)
	cd Test && make src_test

clean:
	cd $(SRC_INT) && make clean
	cd $(SRC_OUT) && make clean
	cd $(SRC_PIC) && make clean
	cd $(SRC_WOR) && make clean
	rm $(EPIX_LIB) $(EPIX_HDR)

