#!/bin/bash
#
# post-uninst.sh
#
# Remove empty directories when uninstalling ePiX
#
# Jan 02, 2005 Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
#

source ./config 2>/dev/null || ( echo "Can't find config file" && exit 1 )


if [ -d $EPIX_ROOTDIR ]; then
    cd $EPIX_ROOTDIR
else
cat<<UNINST_WARN
  The config file seems to have changed since ePiX was installed.
  Please ensure that EPIX_ROOTDIR points to the install tree.

UNINST_WARN
    exit 1
fi

for DIR in $EPIX_SUB_DIRS $EPIX_TOP_DIRS $EPIX_ROOTDIR
do
    rmdir $DIR 2> /dev/null # fail silently if directory is not empty
done

exit 0;
