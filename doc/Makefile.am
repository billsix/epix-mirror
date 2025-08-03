# Makefile.am for ePiX v1.x "doc" directory
#

EPIXFLAGS = --no-defaults -I.. -L.. -lepix -v

%.eepic: %.xp
	../epix-local $(EPIXFLAGS) $<

XPS = $(wildcard *.xp)

EEPICS = $(XPS:.xp=.eepic)

CLEANFILES = *~ *.aux *.dvi *.log *.out *.toc \
	*.idx *.ilg *.ind \
	manual_src.tar manual.pdf manual.ps \
	manual_src.tar.gz manual.pdf.gz manual.ps.gz manual-stamp

MAINTAINERCLEANFILES = *.eepic *.aux *.dvi *.log *.out *.toc \
	*.idx *.ilg *.ind

## Targets ##
doc_DATA = manual.pdf.gz manual.ps.gz manual_src.tar.gz

info_TEXINFOS = epix.texi

# tar up tex, xp, and eepic files; build ps, pdf
manual-stamp: manual.tex $(XPS) $(EEPICS)
	latex -interaction=nonstopmode manual
	latex -interaction=nonstopmode manual
	makeindex manual.idx
	../laps-local --pdf --verbose manual.tex
	touch manual-stamp

manual_src.tar.gz: manual-stamp
	tar -cf manual_src.tar manual.aux manual.ind manual.tex manual.toc \
	   $(XPS) $(EEPICS)
	gzip -9 -n manual_src.tar

#PS
manual.ps.gz: manual-stamp
	gzip -9 -n -c manual.ps > manual.ps.gz

#PDF
manual.pdf.gz: manual-stamp
	gzip -9 -n -c manual.pdf > manual.pdf.gz

epix.info: manual-stamp epix.texi
	makeinfo epix.texi

epix.info.bz2: manual-stamp epix.info
	bzip2 -fk epix.info
