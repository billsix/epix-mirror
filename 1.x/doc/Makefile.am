# Makefile.am for ePiX v1.x "doc" directory
#

EPIXFLAGS=--no-defaults -I. -L. -lepix -vv
LN_SF = ln -sf

SUFFIXES = .xp .eepic

.xp.eepic:
	@./epix ${EPIXFLAGS} $<

.eepic:
	@./epix ${EPIXFLAGS} $^ -o $@


XPS=alignment_lr.xp alignment.xp arrow1.xp arrow2.xp arrow3.xp basepoint.xp \
	box.xp camera.xp cb.xp dots.xp graphpaper.xp koch.xp marker.xp \
	pathstyle.xp plot_a.xp plot_b.xp scaling.xp semicirc.xp sgn.xp \
	sinx.xp slices.xp tori.xp


EEPICS=alignment_lr.eepic alignment.eepic arrow1.eepic arrow2.eepic \
	arrow3.eepic basepoint.eepic box.eepic camera.eepic cb.eepic \
	dots.eepic graphpaper.eepic koch.eepic marker.eepic pathstyle.eepic \
	plot_a.eepic plot_b.eepic scaling.eepic semicirc.eepic sgn.eepic \
	sinx.eepic slices.eepic tori.eepic


## Targets ##
all:	${XPS} epix laps epix.h libepix.a eepics \
	manual.pdf.gz manual.ps.gz src-package

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

src-package:	src-package-stamp

# tar up tex, xp, and eepic files
src-package-stamp:	manual.tex manual.aux manual.ind ${XPS} ${EEPICS}
	@tar -cf - manual.{aux,ind,tex,toc} ${XPS} ${EEPICS} | \
		gzip -9 > ../epix-${VERSION}_manual_src.tar.gz; \
	touch src-package-stamp


manual.aux:	manual.tex
	@latex manual &>/dev/null

manual.idx:	manual.tex
	@latex manual &>/dev/null

manual.ind:	manual.idx
	@latex manual.tex &>/dev/null; latex manual.tex &>/dev/null; \
	makeindex manual.idx &>/dev/null

#PS
manual.ps:	manual.aux manual.ind ${EEPICS}
	@./laps manual.tex

manual.ps.gz:	manual.ps
	@mv manual.ps epix-${VERSION}_manual.ps; \
		gzip -9 epix-${VERSION}_manual.ps; \
	mv epix-${VERSION}_manual.ps.gz ..

#PDF
manual.pdf:	manual.aux manual.ind ${EEPICS}
	@./laps --pdf manual.tex

manual.pdf.gz:	manual.pdf
	@mv manual.pdf epix-${VERSION}_manual.pdf; \
		gzip -9 epix-${VERSION}_manual.pdf; \
	mv epix-${VERSION}_manual.pdf.gz ..


# eepic files
alignment_lr.eepic:	alignment_lr.xp
alignment.eepic:	alignment.xp
arrow1.eepic:		arrow1.xp
arrow2.eepic:		arrow2.xp
arrow3.eepic:		arrow3.xp
basepoint.eepic:	basepoint.xp
box.eepic:		box.xp
camera.eepic:		camera.xp
cb.eepic:		cb.xp
dots.eepic:		dots.xp
graphpaper.eepic:	graphpaper.xp
koch.eepic:		koch.xp
marker.eepic:		marker.xp
pathstyle.eepic:	pathstyle.xp
plot_a.eepic:		plot_a.xp
plot_b.eepic:		plot_b.xp
scaling.eepic:		scaling.xp
semicirc.eepic:		semicirc.xp
sgn.eepic:		sgn.xp
sinx.eepic:		sinx.xp
slices.eepic:		slices.xp
tori.eepic:		tori.xp


clean:
	@-${RM} *.{dvi,log,out,ilg} epix-${VERSION}_manual* \
	epix epix.h laps libepix.a src-package-stamp *~

clean-all:	clean
	@-${RM} *.{aux,eepic,idx,toc,ind}
