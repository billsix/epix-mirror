#!/bin/sh
#
for FILE in elaps laps make_header post-uninst.sh pre-install.sh preflix \
    prepix samples/sample.sh samples/makefigs Makefile
do
	mv $FILE $FILE.orig
	sed 's+/bin/bash+/usr/local/bin/bash+g' $FILE.orig > $FILE
##                       ^-----------------^
## Replace the portion from + to + of the line above with the path to
## bash on your system. Sample replacement line:
##
##   sed 's+/bin/bash+/opt/bin/bash+g' $FILE.orig > $FILE
##
## After running the script, do "rm -f *.orig"
## to clean out the original versions of the files.
	chmod 755 $FILE
done
exit 0

