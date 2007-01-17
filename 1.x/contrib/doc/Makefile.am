# Makefile.am for ePiX v1.x "contrib/doc" directory
#

EPIXFLAGS = --no-defaults -I.. -I../.. -L.. -L../.. -lepixext -lepix -v

%.eepic: %.cc
	../../epix-local $(EPIXFLAGS) $<

XPS := $(wildcard *.cc)

EEPICS = $(XPS:.cc=.eepic)

CLEANFILES = *~ *.aux *.dvi *.log *.toc contrib_src.tar contrib_src.tar.gz \
	  ePiXext.pdf ePiXext.pdf.gz ePiXext.ps ePiXext.ps.gz \
	  contrib.pdf.gz contrib.ps.gz contrib-stamp

MAINTAINERCLEANFILES = *.eepic *.aux *.dvi *.log *.toc

## Targets ##
doc_DATA = contrib.pdf.gz contrib.ps.gz contrib_src.tar.gz


# tar up tex, xp, and eepic files
contrib-stamp: ePiXext.tex $(XPS) $(EEPICS)
	latex -interaction=batchmode ePiXext &>/dev/null
	latex -interaction=batchmode ePiXext &>/dev/null
	touch contrib-stamp

contrib_src.tar.gz: contrib-stamp
	tar -cf contrib_src.tar ePiXext.aux ePiXext.tex ePiXext.toc \
	   $(XPS) $(EEPICS)
	gzip -9 contrib_src.tar

#PS
ePiXext.ps.gz: contrib-stamp
	../../laps-local ePiXext.tex
	gzip -9 ePiXext.ps

contrib.ps.gz: ePiXext.ps.gz
	$(LN_S) ePiXext.ps.gz contrib.ps.gz

#PDF
ePiXext.pdf.gz: contrib-stamp
	../../laps-local --pdf ePiXext.tex
	gzip -9 ePiXext.pdf

contrib.pdf.gz: ePiXext.pdf.gz
	$(LN_S) ePiXext.pdf.gz contrib.pdf.gz
