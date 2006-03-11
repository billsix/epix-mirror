## epix-1.x Makefile.am for top-level directory
##
## Process this file with automake to produce Makefile.in
pkgdatadir = ${datadir}/epix

bin_SCRIPTS = epix elaps flix laps
noinst_SCRIPTS = epix_functions elaps_hdr epix_hdr flix_hdr laps_hdr \
	make_elaps_in make_epix_in make_flix_in make_laps_in

CLEANFILES = ${bin_SCRIPTS}

EXTRA_DIST = AUTHORS BUGS ChangeLog COPYING INSTALL NEWS \
	README README-changes README-porting THANKS ${bin_SCRIPTS}

lib_LIBRARIES = libepix.a
libepix_a_SOURCES = camera.cc circle.cc cropping.cc curves.cc \
	dataplot.cc domain.cc functions.cc geometry.cc Label.cc lengths.cc \
	objects.cc output.cc path.cc plane.cc plots.cc polyhedron.cc \
	segment.cc sphere.cc triples.cc \
	globals.h triples.h domain.h map.h pairs.h segment.h circle.h \
	lengths.h camera.h cropping.h path.h sphere.h plane.h polyhedron.h \
	output.h Label.h objects.h curves.h plots.h dataplot.h geometry.h \
	functions.h

libepix_a_LIBADD = camera.o circle.o cropping.o curves.o \
	dataplot.o domain.o functions.o geometry.o Label.o lengths.o \
	objects.o output.o path.o plane.o plots.o polyhedron.o \
	segment.o sphere.o triples.o

# Makefile for ePiX v1.x
#
CXX = @CXX@
RUNTIME_COMPILER = @RUNTIME_COMPILER@

BASH = @BASH@

# docs, samples
EPIX_MANUAL = epix-${VERSION}_manual
EPIX_SAMPLES = epix-${VERSION}_sample
EPIX_CONTRIB = epix-${VERSION}_contrib

EPIX_MAN = epix.1

# We handle this with debian/docs and other things in debian/rules
# notes, configs
EPIX_CONFFILES = bash_completions epix.ebuild epix.el epix.spec
EPIX_NOTEFILES = AUTHORS BUGS ChangeLog COPYING INSTALL NEWS POST-INSTALL \
	README README-changes README-porting THANKS VERSION


## Manual Targets ##
all:	libepix.a epix.h epix elaps flix laps samples doc
.PHONY:	all epix.h libepix.a

# Headers
epix-${VERSION}.h:
	export VERSION=${VERSION} && ./make_header_in

epix.h:	epix-${VERSION}.h
	@echo "Updating symlink to epix.h"
	@-${RM} -f epix.h && ${LN_S} epix-${VERSION}.h epix.h

## Save copy of header; called only by contrib and doc-contrib
epix-nocontrib.h:
	@echo "Appending contrib header to epix-${VERSION}.h"
	export VERSION="contrib" && ./make_header_in && \
	cat contrib/epix_ext.h \
		| grep -v "\#" | grep -v "include \"" \
		| grep -v endif >> epix-contrib.h
	export VERSION=${VERSION} && mv epix-${VERSION}.h epix-nocontrib.h && \
	mv epix-contrib.h epix-${VERSION}.h

# libepix.a
libepix-${VERSION}.a:	${OBJECTS}
	ar -ru libepix-${VERSION}.a ${OBJECTS}
	${RANLIB} libepix-${VERSION}.a

libepix.a:	libepix-${VERSION}.a
	@echo "Updating symlink to libepix.a"
	@-${RM} -f libepix.a && ${LN_S} libepix-${VERSION}.a libepix.a

# shell scripts
elaps: ${srcdir}/make_elaps_in ${srcdir}/elaps_hdr ${srcdir}/epix_functions
#	-${RM} -f elaps
	@echo "Creating shell script elaps"
	@echo "#! ${BASH}" > elaps && cat ${srcdir}/elaps_hdr >> elaps && \
	echo "EPIX_BIN=${bindir}/epix" >> elaps && \
	cat ${srcdir}/epix_functions ${srcdir}/make_elaps_in >> elaps
	chmod ugo+x elaps

epix: ${srcdir}/make_epix_in ${srcdir}/epix_hdr ${srcdir}/epix_functions
#	-${RM} -f epix
	@echo "Creating shell script epix"
	@echo "#! ${BASH}" > epix && cat ${srcdir}/epix_hdr >> epix && \
	echo "INSTALL_DIR=${prefix}" >> epix && \
	echo "COMPILER=${RUNTIME_COMPILER}" >> epix && \
	cat ${srcdir}/epix_functions ${srcdir}/make_epix_in >> epix
	chmod ugo+x epix

flix: ${srcdir}/make_flix_in ${srcdir}/flix_hdr ${srcdir}/epix_functions
#	-${RM} -f flix
	@echo "Creating shell script flix"
	@echo "#! ${BASH}" > flix && cat ${srcdir}/flix_hdr >> flix && \
	echo "INSTALL_DIR=${prefix}" >> flix && \
	echo "COMPILER=${RUNTIME_COMPILER}" >> flix && \
	cat ${srcdir}/epix_functions ${srcdir}/make_flix_in >> flix
	chmod ugo+x flix

laps: ${srcdir}/make_laps_in ${srcdir}/laps_hdr ${srcdir}/epix_functions
#	-${RM} -f laps
	@echo "Creating shell script laps"
	@echo "#! ${BASH}" > laps && cat ${srcdir}/laps_hdr >> laps && \
	cat ${srcdir}/epix_functions ${srcdir}/make_laps_in >> laps
	chmod ugo+x laps


# documentation and samples
.PHONY: doc doc-clean samples samples-clean

doc:	epix.h libepix.a epix laps
	${MAKE} -C doc

doc-clean:
	-${RM}	epix-*manual*.gz
	${MAKE} -C doc clean

samples:	epix.h libepix.a epix laps
	${MAKE} -C samples

samples-clean:
	-${RM} epix-*sample*.gz
	${MAKE} -C samples clean


# contributed code
contrib-all:	contrib	doc-contrib
.PHONY:	contrib doc-contrib

## Temporarily replace epix.h with epix-nocontrib.h
contrib-pkg-stamp: epix-nocontrib.h libepix.a
	@echo "Twiddling epix.h symlink to build contrib"
	@-${RM} -f epix.h && ${LN_S} epix-nocontrib.h epix.h
	${MAKE} -C contrib
	@echo "Reverting epix.h symlink"
	@-${RM} -f epix.h && ${LN_S} epix-${VERSION}.h epix.h
	touch contrib-pkg-stamp

## Ensure epix-${VERSION}.h contains contrib code
contrib-doc-stamp: epix-nocontrib.h laps epix libepix.a
	${MAKE} -C doc/contrib
	touch contrib-doc-stamp


contrib: contrib-pkg-stamp

contrib-clean:
	${MAKE} -C contrib clean
	-${RM} contrib-pkg-stamp


contrib-doc: contrib-doc-stamp
doc-contrib: contrib-doc-stamp

contrib-doc-clean:
	-${RM}	epix-*contrib*.gz
	${MAKE} -C doc/contrib clean
	-${RM} contrib-doc-stamp

doc-contrib-clean: contrib-doc-clean


# object files
triples.o:	triples.cc ${H_GLOB} triples.h

domain.o:	domain.cc ${H_GLOB} domain.h

segment.o:	segment.cc ${H_GLOB} segment.h

circle.o:	circle.cc ${H_GLOB} ${H_OUTS} ${H_OBJS} circle.h

lengths.o:	lengths.cc ${H_GLOB} ${H_OUTS} lengths.h

camera.o:	camera.cc ${H_GLOB} ${H_OUTS} camera.h

cropping.o:	cropping.cc ${H_GLOB} cropping.h

path.o:		path.cc ${H_GLOB} ${H_OUTS} path.h

sphere.o:	sphere.cc ${H_GLOB} ${H_OBJS} sphere.h

plane.o:	plane.cc ${H_GLOB} ${H_OBJS} plane.h

polyhedron.o:	polyhedron.cc ${H_GLOB} ${H_OBJS} polyhedron.h

output.o:	output.cc ${H_GLOB} ${H_OUTS} output.h

Label.o:	Label.cc ${H_GLOB} ${H_OBJS} Label.h

objects.o:	objects.cc ${H_GLOB} ${H_OUTS} ${H_OBJS} objects.h

curves.o:	curves.cc ${H_GLOB} ${H_OUTS} ${H_OBJS} ${H_PATH} curves.h

plots.o:	plots.cc ${H_GLOB} ${H_OUTS} ${H_OBJS} ${H_PATH} plots.h

dataplot.o: 	dataplot.cc ${H_GLOB} ${H_OUTS} ${H_OBJS} ${H_PATH} dataplot.h

geometry.o:	geometry.cc ${H_GLOB} ${H_OUTS} ${H_OBJS} ${H_PATH} geometry.h

functions.o:	functions.cc ${H_GLOB} functions.h


# retains eepic files
clean:
	-${RM} ${OBJECTS} epix*.h libepix*.a epix flix elaps laps \
	epix-${VERSION}_*.gz *-stamp
	${MAKE} -C doc clean
	${MAKE} -C doc/contrib clean
	${MAKE} -C samples clean
	${MAKE} -C contrib clean
	-${RM} Makefile

# removes eepic files from subdirectories
clean-all:
	-${RM} ${OBJECTS} epix*.h libepix*.a epix flix elaps laps \
	epix-${VERSION}_*.gz *-stamp
	${MAKE} -C doc clean-all
	${MAKE} -C doc/contrib clean-all
	${MAKE} -C samples clean-all
	${MAKE} -C contrib clean-all
	-${RM} Makefile


## Leaf directories
# bindir: shell scripts
# libdir: library and symlink
# includedir: header and symlink
# ${pkgdatadir}/{config,notes}: config files, notes
# ${datadir}/doc/epix: samples, manual
# ${datadir}/man/man1: man page
install:	libepix-${VERSION}.a epix-${VERSION}.h epix flix elaps laps
# Create top-level directories if necessary
#	@echo "Installing shell scripts"; \
#	mkdir -p ${bindir}
#	@INSTALL@ epix elaps flix laps ${bindir}
#
	@echo; echo "Installing header"; \
	mkdir -p ${includedir}
	@INSTALL_DATA@ epix-${VERSION}.h ${includedir}/epix-${VERSION}.h;
	@echo "Creating symlink to epix.h in ${includedir}" && \
	( cd ${includedir} && ${RM} -f epix.h && ${LN_S} epix-${VERSION}.h epix.h )
#
#	@echo; echo "Installing library"; \
#	mkdir -p ${libdir}
#	@INSTALL_DATA@ libepix-${VERSION}.a ${libdir}/libepix-${VERSION}.a;
#	@echo "Creating symlink to libepix.a in ${libdir}" && \
#	( cd ${libdir} && ${RM} -f libepix.a && ${LN_S} libepix-${VERSION}.a libepix.a )
#	${RANLIB} ${libdir}/libepix-${VERSION}.a
#
#
#	Not on Debian; dh_installdirs takes care of this one
	@echo; echo "Installing notes and configuration files, man page"; \
	mkdir -p ${pkgdatadir}/notes ${pkgdatadir}/config \
	${datadir}/man/man1
	@INSTALL_DATA@ ${EPIX_NOTEFILES} ${pkgdatadir}/notes
	@INSTALL_DATA@ ${EPIX_CONFFILES} ${pkgdatadir}/config
	@INSTALL_DATA@ ${EPIX_MAN} ${datadir}/man/man1/${EPIX_MAN}


	@if test -f ${EPIX_MANUAL}.ps.gz ; then \
	echo; echo "Installing manual and manual sources"; \
	mkdir -p ${datadir}/doc/epix; \
	@INSTALL_DATA@ ${EPIX_MANUAL}.ps.gz ${EPIX_MANUAL}.pdf.gz \
		${EPIX_MANUAL}_src.tar.gz ${datadir}/doc/epix; \
	fi

	@if test -f ${EPIX_SAMPLES}.ps.gz ; then \
	echo "Installing sample document and sources"; \
	mkdir -p ${datadir}/doc/epix; \
	@INSTALL_DATA@ ${EPIX_SAMPLES}.ps.gz ${EPIX_SAMPLES}.pdf.gz \
		${EPIX_SAMPLES}_src.tar.gz ${datadir}/doc/epix; \
	fi

	@if test -f ${EPIX_CONTRIB}_src.tar.gz ; then \
	echo "Installing contrib manual and sources"; \
	mkdir -p ${datadir}/doc/epix; \
	@INSTALL_DATA@ ${EPIX_CONTRIB}.ps.gz ${EPIX_CONTRIB}.pdf.gz \
		${EPIX_CONTRIB}_src.tar.gz ${datadir}/doc/epix; \
	fi


uninstall:
	@echo "Uninstalling ePiX-${VERSION}"
	-${RM}	${bindir}/epix   ${bindir}/laps \
		${bindir}/elaps  ${bindir}/flix \
		${includedir}/epix.h ${includedir}/epix-${VERSION}.h \
		${libdir}/libepix.a ${libdir}/libepix-${VERSION}.a \
		${datadir}/man/man1/${EPIX_MAN} \
		${pkgdatadir}/config/*  ${pkgdatadir}/notes/* \
		${datadir}/doc/epix/epix-${VERSION}* 2>/dev/null

	-rmdir ${datadir}/man/man1 ${datadir}/man \
		${pkgdatadir}/config ${pkgdatadir}/notes ${pkgdatadir} \
		${datadir}/doc/epix ${datadir}/doc ${datadir} \
		${libdir} ${includedir} ${bindir} ${prefix} 2>/dev/null


uninstall-all:
	@echo -n "Uninstalling all versions of ePiX..."
	-${RM}	${bindir}/epix   ${bindir}/laps \
		${bindir}/elaps  ${bindir}/flix \
		${includedir}/epix*.h ${libdir}/libepix*.a \
		${datadir}/man/man1/${EPIX_MAN} \
		${pkgdatadir}/config/* ${pkgdatadir}/notes/* \
		${datadir}/doc/epix/epix-* 2>/dev/null

	-rmdir ${datadir}/man/man1 ${datadir}/man \
		${pkgdatadir}/config ${pkgdatadir}/notes ${pkgdatadir} \
		${datadir}/doc/epix ${datadir}/doc ${datadir} \
		${libdir} ${includedir} ${bindir} ${prefix} 2>/dev/null

	@echo "done. Sorry it didn't work out."
