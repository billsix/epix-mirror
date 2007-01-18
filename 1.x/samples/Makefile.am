# Makefile.am for ePiX v1.x "samples" directory
#

EPIXFLAGS = --no-defaults -I.. -L.. -lepix -v

%.eepic: %.xp
	../epix-local $(EPIXFLAGS) $<

XPS := $(wildcard *.xp)

EEPICS = $(XPS:.xp=.eepic)

CLEANFILES = *~ *.aux *.dvi *.log *.out *.ilg \
	  sample_src.tar sample.pdf sample.ps \
	  sample_src.tar.gz sample.pdf.gz sample.ps.gz sample-stamp

MAINTAINERCLEANFILES = *.eepic *.aux

## Targets ##
doc_DATA = sample.pdf.gz sample.ps.gz sample_src.tar.gz


# tar up tex, xp, and eepic files
sample-stamp: sample.tex $(XPS) $(EEPICS)
	latex -interaction=batchmode sample &>/dev/null
	latex -interaction=batchmode sample &>/dev/null
	touch sample-stamp

sample_src.tar.gz: sample-stamp
	tar -cf sample_src.tar sample.tex $(XPS) $(EEPICS) extras
	gzip -9 sample_src.tar

#PS
sample.ps.gz: sample-stamp
	../laps-local sample.tex
	gzip -9 sample.ps

#PDF; no point compressing it
sample.pdf.gz: sample-stamp
	../laps-local --pdf sample.tex
	gzip -9 sample.pdf
