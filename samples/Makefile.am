# Makefile.am for ePiX v1.x "samples" directory
#

XPS = $(wildcard *.xp)
FLX = $(wildcard *.flx)

CLEANFILES = *~ *.eepic *.eps sample_src.tar sample_src.tar.gz sample-stamp

## Targets ##
doc_DATA = sample_src.tar.gz


# tar up tex, xp, and eepic files
sample-stamp: $(XPS) $(FLX)
	touch sample-stamp

sample_src.tar.gz: sample-stamp
	tar -cf sample_src.tar $(XPS) $(FLX) \
	README binom.cc lighting.h std_F.cc std_F.h
	gzip -9 sample_src.tar
