#!/bin/sh
#
source ./config || exit 1

for FILE in make_{elaps,epix,flix,header,laps} post-uninst.sh pre-install.sh \
    samples/sample.sh helpfiles.sh
do
    if [ ! -f $FILE.orig ]; then 
        mv $FILE $FILE.orig && chmod 600 $FILE.orig
    fi

    sed "s+/bin/bash+$EPIX_BASH_PATH+g" $FILE.orig > $FILE
    chmod u+x $FILE
done
exit 0

