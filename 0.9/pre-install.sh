#!/bin/bash
#
# pre-install.sh
#
# Create directories needed by ePiX; do not run manually.
#
# June 29, 2002  Andrew D. Hwang  <ahwang@mathcs.holycross.edu>
#
# Check whether we were called by the Makefile's install target
if [ "$EPIX_ROOTDIR" = "" ]
then
    echo "pre-install.sh: Please do not run this script manually!" && exit 1
fi

DIRS='bin lib include man man/man1 share share/epix share/epix/config share/epix/notes share/epix/samples share/epix/tutorial'

cd $EPIX_ROOTDIR

for i in $DIRS
do
    if [ ! -d $i ]
    then mkdir $i
    fi
done

chmod go+rX $DIRS

exit 0;
