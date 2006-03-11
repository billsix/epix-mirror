# Makefile.am for ePiX v1.x "doc/contrib" directory
#

EPIXFLAGS=--no-defaults -I. -L. -lepix -vv
LN_SF = ln -sf

SUFFIXES = .cc .eepic

.cc.eepic:
	@./epix ${EPIXFLAGS} $<

.eepic:
	@./epix ${EPIXFLAGS} $^ -o $@


XPS=graf01.cc graf02.cc graf03.cc graf04.cc graf05.cc graf06.cc graf07.cc \
	graf08.cc graf09.cc graf10.cc graf11.cc graf12.cc graf13.cc graf14.cc \
	graf15.cc graf16.cc graf17.cc graf18.cc graf20.cc graf21.cc graf22.cc \
	graf23.cc graf24.cc stylegraf1.cc stylegraf2.cc stylegraf3.cc \
	stylegraf4.cc stylegraf5.cc stylegraf6.cc

EEPICS=graf01.eepic graf02.eepic graf03.eepic graf04.eepic graf05.eepic \
	graf06.eepic graf07.eepic graf08.eepic graf09.eepic graf10.eepic \
	graf11.eepic graf12.eepic graf13.eepic graf14.eepic graf15.eepic \
	graf16.eepic graf17.eepic graf18.eepic graf20.eepic graf21.eepic \
	graf22.eepic graf23.eepic graf24.eepic stylegraf1.eepic \
	stylegraf2.eepic stylegraf3.eepic stylegraf4.eepic stylegraf5.eepic \
	stylegraf6.eepic


## Targets ##
all:	${XPS} epix laps epix.h libepix.a eepics \
	ePiXext.pdf.gz ePiXext.ps.gz src-package

.PHONY: all epix laps epix.h libepix.h eepics src-package clean clean-all

epix:
	@${LN_SF} ../../epix .

laps:
	@${LN_SF} ../../laps .

epix.h:
	@${LN_SF} ../../epix.h .

libepix.a:
	@${LN_SF} ../../libepix.a .


eepics:	${EEPICS}

src-package:	src-package-stamp

# tar up tex, xp, and eepic files
src-package-stamp:	ePiXext.tex ePiXext.aux ${XPS} ${EEPICS}
	@tar -cf - ePiXext.{aux,tex,toc} ${XPS} ${EEPICS} | \
		gzip -9 > ../../epix-${VERSION}_contrib_src.tar.gz; \
	touch src-package-stamp


ePiXext.aux:	ePiXext.tex
	@latex ePiXext &>/dev/null


#PS
ePiXext.ps:	ePiXext.aux ${EEPICS}
	@./laps ePiXext.tex

ePiXext.ps.gz:	ePiXext.ps
	@mv ePiXext.ps epix-${VERSION}_contrib.ps; \
		gzip -9 epix-${VERSION}_contrib.ps; \
	mv epix-${VERSION}_contrib.ps.gz ../..

#PDF
ePiXext.pdf:	ePiXext.aux ${EEPICS}
	@./laps --pdf ePiXext.tex

ePiXext.pdf.gz:	ePiXext.pdf
	@mv ePiXext.pdf epix-${VERSION}_contrib.pdf; \
		gzip -9 epix-${VERSION}_contrib.pdf; \
	mv epix-${VERSION}_contrib.pdf.gz ../..


# individual eepic files
graf01.eepic:	graf01.cc
graf02.eepic:	graf02.cc
graf03.eepic:	graf03.cc
graf04.eepic:	graf04.cc
graf05.eepic:	graf05.cc
graf06.eepic:	graf06.cc
graf07.eepic:	graf07.cc
graf08.eepic:	graf08.cc
graf09.eepic:	graf09.cc
graf10.eepic:	graf10.cc
graf11.eepic:	graf11.cc
graf12.eepic:	graf12.cc
graf13.eepic:	graf13.cc
graf14.eepic:	graf14.cc
graf15.eepic:	graf15.cc
graf16.eepic:	graf16.cc
graf17.eepic:	graf17.cc
graf18.eepic:	graf18.cc
graf19.eepic:	graf19.cc
graf20.eepic:	graf20.cc
graf21.eepic:	graf21.cc
graf22.eepic:	graf22.cc
graf23.eepic:	graf23.cc
graf24.eepic:	graf24.cc

stylegraf1.eepic:	stylegraf1.cc
stylegraf2.eepic:	stylegraf2.cc
stylegraf3.eepic:	stylegraf3.cc
stylegraf4.eepic:	stylegraf4.cc
stylegraf5.eepic:	stylegraf5.cc
stylegraf6.eepic:	stylegraf6.cc


clean:
	@-${RM} *.{dvi,log} epix-${VERSION}_contrib*.gz \
	epix epix.h laps libepix.a src-package-stamp *~

clean-all:	clean
	@-${RM} *.{eepic,aux,toc}

