#!/bin/bash
#
# post-uninst.sh
#
# Remove empty directories when uninstalling ePiX
#
# June 29, 2002  Andrew D. Hwang,  rot 13 nujnat at zngupf dot ubylpebff dot rqh
#

# Check whether we were called by the Makefile's uninstall target
if [ "$EPIX_ROOTDIR" = "" ]
then
    echo "post-uninst.sh: Please do not run this script manually!" && exit 1
fi

cd $EPIX_ROOTDIR

for DIR in bin lib include man/man1 man share
do
    rmdir $DIR 2> /dev/null # fail silently if directory is not empty
done

exit 0;
