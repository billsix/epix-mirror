#!/bin/bash
#
# helpfiles.sh: Build documentation/sample files
#
# Andrew D. Hwang, <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
#
# January 06, 2005
#

# We only require EPIX_VERSION from config
source ./config || ( echo "Can't find config file" && exit 1 )

## anchor
EPIX_SRC_DIR=$(pwd)

## firm-coded names
EPIX_MANUAL=epix-${EPIX_VERSION}_manual
EPIX_CONTRIB=epix-${EPIX_VERSION}_contrib
EPIX_SAMPLE=epix-${EPIX_VERSION}_sample

## hard-coded names
MANUAL_SRC_DIR=doc
MANUAL_SRC=tutorial

CONTRIB_SRC_DIR=doc/contrib
CONTRIB_SRC=ePiXext

SAMPLE_SRC_DIR=samples
SAMPLE_SRC=sample


# Mimic run of epix
function ePiX()
{
    EXT=${2:-xp}

    FILEROOT=${1%%".$EXT"}
    if [ "$EXT" != "cc" ]; then ln $FILEROOT.$EXT $FILEROOT.cc; fi

    $EPIX_CXX -w $FILEROOT.cc -o $FILEROOT.exe -I. -L. -lm -lepix &&

    if [ "$EXT" != "cc" ]; then rm $FILEROOT.cc; fi

    if [ -x "$FILEROOT.exe" ]; then ./$FILEROOT.exe > $FILEROOT.eepic; fi

    if [ -f "$FILEROOT.eepic" ] 
	then
	echo -n "." # Progress meter :)
	rm -f $FILEROOT.exe
	return 0;

    else 
	echo "failed, exiting"
	rm -f $FILEROOT.exe libepix.a 2>/dev/null
	exit 1;
    fi
} # end of ePiX()

# Assuming we're in a child of the srcdir, compile eepic-less xp files
function makefigs()
{
    ln -sf $EPIX_SRC_DIR/libepix-$EPIX_VERSION.a libepix.a
    ln -sf $EPIX_SRC_DIR/epix-$EPIX_VERSION.h epix.h
    ranlib libepix.a # for Apple devtools

    EXT=${1:-xp}

    for FILE in *.$EXT ; do 
        if [ ! -f ${FILE%%"$EXT"}eepic ] ||
           [ $FILE -nt ${FILE%%"$EXT"}eepic ]; then
            ePiX $FILE $EXT # N.B. Shell function ePiX
        fi
    done

    # clean up
    rm -f libepix.a epix.h
} # end of makefigs

# makedocs $SRCDIR $MSG $EXT $SRCFILE $OUTFILE $FILES
function makedocs()
{
    if [ ! -d $1 ]; then
        echo "$1 not found, skipping" && exit 0;

    else
        cd $1 &&
        echo -n "Compiling $2..." && 
	if [ $1 = $SAMPLE_SRC_DIR ]; then ./sample.sh; fi

        makefigs $3 &&
        $EPIX_SRC_DIR/laps -Pamz -Pcmz --pdf $4.tex &&

        mv -f $4.pdf ../$5.pdf &&
        mv -f $4.ps  ../$5.ps  &&
        echo "done"

        cd $EPIX_SRC_DIR
    fi
} # end of makedocs

case "$1" in

    --doc)

        makedocs $MANUAL_SRC_DIR manual xp $MANUAL_SRC $EPIX_MANUAL

        tar -cf ${EPIX_MANUAL}_src.tar \
            $MANUAL_SRC_DIR/$MANUAL_SRC.{aux,idx,ind,tex,toc} \
            $MANUAL_SRC_DIR/*.{xp,eepic}
        gzip -9 ${EPIX_MANUAL}*
        ;;


    --contrib-doc)

        echo -n "Compiling contrib figures..."
        cd $CONTRIB_SRC_DIR && makefigs cc && cd $EPIX_SRC_DIR
        echo "done"

        tar -cf ${EPIX_CONTRIB}_src.tar \
            $CONTRIB_SRC_DIR/$CONTRIB_SRC.{aux,tex,toc} \
            $CONTRIB_SRC_DIR/*.{cc,eepic}
        gzip -9 ${EPIX_CONTRIB}_src.tar
        ;;


    --samples)

        makedocs $SAMPLE_SRC_DIR samples xp $SAMPLE_SRC $EPIX_SAMPLE

        tar -cf ${EPIX_SAMPLE}_src.tar \
            $SAMPLE_SRC_DIR/{$SAMPLE_SRC.*,*.xp,template,extras}
        gzip -9 ${EPIX_SAMPLE}*
        ;;


    *)
	exit 1
	;;
esac

exit 0
