#!/bin/sh
#
for FILE in make_{elaps,epix,flix,header,laps} post-uninst.sh pre-install.sh \
    samples/sample.sh Makefile helpfiles.sh
do
	mv $FILE $FILE.orig
	sed 's+/bin/bash+/usr/local/bin/bash+g' $FILE.orig > $FILE
##                       ^-----------------^
## Replace the portion from + to + of the line above with the path to
## bash on your system. Sample replacement line:
##
##   sed 's+/bin/bash+/sw/bin/bash+g' $FILE.orig > $FILE
##
## After running the script, do "rm -f *.orig"
## to clean out the original versions of the files.
	chmod 755 $FILE
done
exit 0

