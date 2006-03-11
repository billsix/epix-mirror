# Makefile.am for ePiX v1.x "samples" directory
#

EPIXFLAGS=--no-defaults -I. -L. -lepix -vv
LN_SF = ln -sf

SUFFIXES = .xp .eepic

.xp.eepic:
	@./epix ${EPIXFLAGS} $<

.eepic:
	@./epix ${EPIXFLAGS} $^ -o $@


XPS = calculus.xp clipping.xp contour.xp cropplot.xp denom.xp geomsum.xp \
	koch.xp levelset.xp lissajous.xp newton.xp oscillator.xp \
	parabola.xp pascal.xp polar.xp pole.xp shadeplot.xp slopefield.xp \
	sphere.xp sqrt.xp torus.xp uppersum.xp weierstrass.xp wheel.xp

EEPICS = calculus.eepic clipping.eepic contour.eepic cropplot.eepic \
	denom.eepic geomsum.eepic koch.eepic levelset.eepic \
	lissajous.eepic newton.eepic oscillator.eepic parabola.eepic \
	pascal.eepic polar.eepic pole.eepic shadeplot.eepic \
	slopefield.eepic sphere.eepic sqrt.eepic torus.eepic \
	uppersum.eepic weierstrass.eepic wheel.eepic

## Targets ##
all:	${XPS} epix laps epix.h libepix.a eepics \
	sample.pdf.gz sample.ps.gz src-package

.PHONY: all epix laps epix.h libepix.h eepics src-package clean clean-all

epix:
	@${LN_SF} ../epix .

laps:
	@${LN_SF} ../laps .

epix.h:
	@${LN_SF} ../epix.h .

libepix.a:
	@${LN_SF} ../libepix.a .


eepics:	${EEPICS}

src-package:	src-package-stamp sample.tex ${XPS} ${EEPICS}

src-package-stamp:
	@tar -cf - sample.tex ${XPS} ${EEPICS} extras | \
		gzip -9 > ../epix-${VERSION}_sample_src.tar.gz; \
	touch src-package-stamp

sample.aux:	sample.tex
	@latex sample &>/dev/null

sample.ps:	sample.aux ${EEPICS}
	@./laps sample.tex

sample.ps.gz:	sample.ps
	@mv sample.ps epix-${VERSION}_sample.ps; \
		gzip -9 epix-${VERSION}_sample.ps; \
	mv epix-${VERSION}_sample.ps.gz ..

sample.pdf:	sample.aux ${EEPICS}
	@./laps --pdf sample.tex

sample.pdf.gz:	sample.pdf
	@mv sample.pdf epix-${VERSION}_sample.pdf; \
		gzip -9 epix-${VERSION}_sample.pdf; \
	mv epix-${VERSION}_sample.pdf.gz ..


# figures
calculus.eepic:		calculus.xp
clipping.eepic:		clipping.xp
contour.eepic:		contour.xp
cropplot.eepic:		cropplot.xp
denom.eepic:		denom.xp
geomsum.eepic:		geomsum.xp
koch.eepic:		koch.xp
levelset.eepic:		levelset.xp
lissajous.eepic:	lissajous.xp
newton.eepic:		newton.xp
oscillator.eepic:	oscillator.xp
parabola.eepic:		parabola.xp
pascal.eepic:		pascal.xp
polar.eepic:		polar.xp
pole.eepic:		pole.xp
shadeplot.eepic:	shadeplot.xp
slopefield.eepic:	slopefield.xp
sphere.eepic:		sphere.xp
sqrt.eepic:		sqrt.xp
torus.eepic:		torus.xp
uppersum.eepic:		uppersum.xp
weierstrass.eepic:	weierstrass.xp
wheel.eepic:		wheel.xp


clean:
	@-rm -f sample.{dvi,log} epix-${VERSION}_sample*.gz \
	epix epix.h laps libepix.a src-package-stamp *~

clean-all:	clean
	@-rm ${EEPICS} sample.aux
