# Makefile.am for ePiX v1.x "doc" directory
#

EPIXFLAGS = --no-defaults -I.. -L.. -lepix -v

%.eepic: %.xp
	../epix-local $(EPIXFLAGS) $<

XPS := $(wildcard *.xp)

EEPICS = $(XPS:.xp=.eepic)

CLEANFILES = *~ *.aux *.dvi *.log *.out *.idx *.ilg *.ind *.toc \
	  manual_src.tar manual.pdf manual.ps \
	  manual_src.tar.gz manual.pdf.gz manual.ps.gz manual-stamp

MAINTAINERCLEANFILES = *.eepic *.aux *.dvi *.log *.out *.toc *.idx *.ind *.ilg

## Targets ##
doc_DATA = manual.pdf.gz manual.ps.gz manual_src.tar.gz


# tar up tex, xp, and eepic files
manual-stamp: manual.tex $(XPS) $(EEPICS)
	latex -interaction=batchmode manual &>/dev/null
	latex -interaction=batchmode manual &>/dev/null
	makeindex manual.idx &>/dev/null
	touch manual-stamp

manual_src.tar.gz: manual-stamp
	tar -cf manual_src.tar manual.aux manual.ind manual.tex manual.toc \
	   $(XPS) $(EEPICS)
	gzip -9 manual_src.tar

#PS
manual.ps.gz: manual-stamp
	../laps-local manual.tex
	gzip -9 manual.ps

#PDF; no point compressing it
manual.pdf.gz: manual-stamp
	../laps-local --pdf manual.tex
	gzip -9 manual.pdf
