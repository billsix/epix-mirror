#!/bin/bash
#
# helpfiles.sh: (un)install/test sample files
#
# This script is NOT INTENDED TO BE RUN MANUALLY!!
#
#   Options: --install, --uninstall, --test
#   Assumes EPIX_ROOTDIR is set to install directory from Makefile
#
# Andrew D. Hwang, <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
#
# August 12, 2004  
#

PROG=$(basename $0)
INSTALL="install -m 644"
COMPILER=${EPIX_CXX:-g++}

if [ "$EPIX_ROOTDIR" = "" ]
then 
    echo "$PROG: Please do not run this script manually!" && exit 1
fi

EPIX_SHAREDIR=${EPIX_ROOTDIR}/share
EPIX_EPIXDIR=${EPIX_SHAREDIR}/epix

EPIX_CONFDIR=${EPIX_EPIXDIR}/config
EPIX_NOTEDIR=${EPIX_EPIXDIR}/notes
EPIX_SAMPDIR=${EPIX_EPIXDIR}/samples
EPIX_TTRLDIR=${EPIX_EPIXDIR}/tutorial

EPIX_CONFFILES="bash_completions epix.el epix.spec update_figs.sh"
EPIX_NOTEFILES="BUGS CHANGELOG COPYING INSTALL POST-INSTALL README README-authors README-changes THANKS TODO"

EPIX_DOCDIR=$(ls -d epix-*_tex 2>/dev/null)
EPIX_MAJOR_VERSION=${EPIX_DOCDIR%%"_tex/"} # "epix-0.8.x"
EPIX_VERSION=epix-EPIX_VERSION_NUMBER
EPIX_TARBALL=${EPIX_VERSION}_samples.tar

# Mimic run of epix on sample file, using newly-compiled library
ePiX() 
{
    FILEROOT=${1%%".xp"}
    ln $FILEROOT.xp $FILEROOT.c # compiler doesn't know about .xp files

    $COMPILER $CFLAGS $FILEROOT.c -o $FILEROOT.exe -I. -L. -lm -lepixtest &&
    rm $FILEROOT.c

    if [ -x "$FILEROOT.exe" ] 
	then ./$FILEROOT.exe > $FILEROOT.eepic
    fi

    if [ -f "$FILEROOT.eepic" ] 
	then
	echo -n "." # Progress meter :)
	rm -f $FILEROOT.exe
	return 0;

    else 
	echo "failed, exiting"
	rm -f $FILEROOT.exe libepixtest.a 2>/dev/null
	exit 1;
    fi
} # end of ePiX()

# Assuming we're in a child of the srcdir, compile eepic-less xp files
makefigs()
{
    cp -p ../libepix.a libepixtest.a
    cp -p ../epix.h epix.h

    for file in *.xp; do
	if [ ! -f ${file%%"xp"}eepic ]
	    then ePiX $file
	fi
    done

    rm -f libepixtest.a epix.h
} # end of makefigs

case "$1" in

    --install)
	if [ ! -d $EPIX_EPIXDIR ]; then
	    # pre-install script has failed...
	    echo "$PROG: Can't find \"$EPIX_EPIXDIR\""
	    exit 1;
	fi

	### Notes and misc config files ###
	$INSTALL $EPIX_NOTEFILES $EPIX_NOTEDIR

	$INSTALL $EPIX_CONFFILES $EPIX_CONFDIR

	### Tutorial ###
	if [ "$EPIX_DOCDIR" != "" ]; then # we're "complete"
	    cd $EPIX_DOCDIR && makefigs && ../laps -Pamz -Pcmz tutorial.tex
	    # Put tutorial in $INSTALL_DIR/share/epix/tutorial
	    $INSTALL tutorial.ps $EPIX_TTRLDIR/${EPIX_MAJOR_VERSION}_howto.ps

	    rm tutorial.{dvi,log,ps}

	    # Tutorial source files
	    $INSTALL tutorial.{aux,idx,ind,tex,toc} *.{xp,eepic} $EPIX_TTRLDIR

	    # Contrib documentation
	    if [ ! -d $EPIX_TTRLDIR/contrib ]; then
		mkdir $EPIX_TTRLDIR/contrib
	    fi

	    $INSTALL contrib/* $EPIX_TTRLDIR/contrib

	    cd .. # back to src directory
	fi

	### Sample files ###
	tar -cf $EPIX_TARBALL samples/{*.xp,sample.*,makefigs,template,extras}
	# Put samples tarball in $INSTALL_DIR/share/epix/samples
	$INSTALL $EPIX_TARBALL $EPIX_SAMPDIR/$EPIX_TARBALL &&
	rm $EPIX_TARBALL

	# Fix ownership/permissions in $INSTALL_DIR/share/epix
	cd $EPIX_EPIXDIR && chown -R $USER:$GROUPS *
	
	if [ $UID -eq 0 ]; then # Public installation
	    chmod -R go+rX * # Give everyone read permission
	fi
	;;

    --uninstall)
	# Delete $EPIX_EPIXDIR; Makefile handles the rest
	cd $EPIX_SHAREDIR 
	if [ -d epix ]; then rm -Rf epix; fi
	;;

    --test)
	CFLAGS="-w"

	if [ -d samples ] 
	then cd ./samples
	else echo "$PROG: Directory \"samples\" does not exist" && exit 1;
	fi

	# Now we're in samples
	cp -p ../libepix.a libepixtest.a
	cp -p ../epix.h epix.h

	# generate sample.tex
	echo -n "Creating sample.tex..."
	./sample.sh
	echo "done"
	echo -n "Creating .eepic files"
	for FILE in *.xp ; do 
	    if [ ! -f ${FILE%%"xp"}eepic ] || [ $FILE -nt ${FILE%%"xp"}eepic ]
	    then
		ePiX $FILE # N.B. Shell function ePiX
	    fi
	done
	echo "done"

	# Clean up
	rm -f libepixtest.a epix.h

	echo; echo -n "Testing laps..."

	../laps sample.tex &>/dev/null

	if [ -f "sample.ps" ] 
	then echo "success"
	     mv -f sample.dvi  sample.ps ..
	     rm -f sample.log
	     echo "You may now preview sample.dvi or sample.ps"; exit 0;
	else echo "failed"; exit 1;
	fi
	;;

    *)
	echo "$PROG: Unknown option $1" && exit 1
	;;
esac

exit 0
