#!/bin/bash
#
# pre-install.sh
#
# Create directories needed by ePiX
#
# Jan 04, 2005 Andrew D. Hwang  <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
#

source ./config 2>/dev/null || ( echo "Can't find config file" && exit 1 )

# set directory permission flag
PERMS="u+rX"
# Public installation?
if [ $UID -eq 0 ]; then PERMS="ugo+rX"; fi

if [ ! -d $EPIX_ROOTDIR ]; then
    mkdir -p $EPIX_ROOTDIR
fi

cd $EPIX_ROOTDIR

for i in $EPIX_SUB_DIRS $EPIX_TOP_DIRS
do
    if [ ! -d $i ]; then mkdir -p $i; fi
    chmod $PERMS $i && chown $USER:$GROUPS $i
done

exit 0;
