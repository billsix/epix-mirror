# Common functions for ePiX's shell scripts
#
PROG=$(basename $0)

# global variables for all scripts
EPIX_PID=$$

# Needn't be secure
EPIX_TEMPDIR=${PROG}-${EPIX_PID}

# Caution: Common logfile name for all ${PROG} processes in cwd,
# used until we've parsed command-line options. Will assign a
# unique name once we have an input file name.
EPIX_LOGFILE=${PROG}.log
if [ -f "${EPIX_LOGFILE}" ]; then rm -f "${EPIX_LOGFILE}"; fi

EPIX_STDOUT=/dev/null
EPIX_STDERR=/dev/stderr

declare EPIX_NOTFOUND

declare EPIX_VERBOSE

# in increasing order of severity
function ePiX_msg {
    echo -e "$PROG: $@" | tee -a "$EPIX_LOGFILE" > "$EPIX_STDOUT"
}

function ePiX_warn {
    echo -e "$PROG: WARNING: $@" | tee -a "$EPIX_LOGFILE" >"$EPIX_STDERR"
}

function ePiX_fail {
    echo -e "$PROG: WARNING: $@" | tee -a "$EPIX_LOGFILE" >"$EPIX_STDERR"
    if [ "$EPIX_OUTROOT" = "$EPIX_INROOT" ]; then unset EPIX_OUTROOT; fi
    let failure=failure+1
}

function ePiX_die {
    echo -e "$PROG: ERROR: $@" | tee -a "$EPIX_LOGFILE" >"$EPIX_STDERR"
    echo -e "Transcript written on ${EPIX_LOGFILE}\n"
    exit 1;
}

# Echo command to stderr/stdout and logfile, then run command
#
function ePiX_command {
    ePiX_msg "$@"
    "$@" 2>&1 | tee -a "$EPIX_LOGFILE" > "$EPIX_STDOUT"
}

function ePiX_dvips {
    ePiX_msg "$@"
    ("$@" 2>&1) | tee -a "$EPIX_LOGFILE" > "$EPIX_STDOUT"
}


# epix_parse_filename EPIX_INFILE MY_EXTENSIONS:
#
#   Attempt to decompose EPIX_INFILE into EPIX_INROOT.EPIX_SUFFIX, with
#   EPIX_SUFFIX in MY_EXTENSIONS, then check for existence of file.
#   If EPIX_INFILE has no suffix, the first match (if any) wins.
#   If no file is found, EPIX_NOTFOUND="yes"
#
function epix_parse_filename {
    EPIX_INFILE="$1"
    shift;
    MY_EXTENSIONS="$@"

    unset EPIX_INROOT
    unset EPIX_SUFFIX
    unset EPIX_NOTFOUND

    local FOUND_CT=0

    # Check for recognized extension
    for EXT in $MY_EXTENSIONS; do
        TMP_ROOT="${EPIX_INFILE%.$EXT}"
        if [ "$EPIX_INFILE" = "$TMP_ROOT.$EXT" ]; then
            EPIX_INROOT="$TMP_ROOT"
            EPIX_SUFFIX="$EXT"
            break
        fi
    done

    # None found; were we given just the root?
    if [ -z "$EPIX_SUFFIX" ]; then
        EPIX_INROOT="${EPIX_INFILE%%.}"

        # Search for (and count) completions
        for EXT in $MY_EXTENSIONS; do
            if [ -f "$EPIX_INROOT.$EXT" ]; then
                if [ $FOUND_CT -eq 0 ]; then # first match wins
                    EPIX_INFILE="$EPIX_INROOT.$EXT"
                    EPIX_SUFFIX="$EXT"
                fi

                let FOUND_CT=FOUND_CT+1
            fi
        done

        # no match
        if [ $FOUND_CT -eq 0 ]; then EPIX_NOTFOUND="yes"; fi

    # Found suffix, check for input file
    else
        if [ -f "$EPIX_INFILE" ]; then
            let FOUND_CT=FOUND_CT+1
        else
            EPIX_NOTFOUND="yes"
        fi
    fi # if $EPIX_SUFFIX=""

    # initialize logfile
    EPIX_LOGFILE="${EPIX_INROOT}_${EPIX_SUFFIX}.log"
    if [ -f "$EPIX_LOGFILE" ]; then rm "$EPIX_LOGFILE"; fi
    touch "$EPIX_LOGFILE"

    # Print status messages
    ePiX_msg "Processing $EPIX_INFILE:"

    if [ $FOUND_CT -eq 0 ]; then
        ePiX_warn "  No completion of \"$EPIX_INROOT\" found"

    elif [ $FOUND_CT -ge 2 ]; then
        ePiX_warn "  Found $FOUND_CT completions of \"$EPIX_INROOT\", using $EPIX_INFILE"

    elif [ "$EPIX_NOTFOUND" = "yes" ]; then
        ePiX_warn "  \"$EPIX_INFILE\" not found"
    fi
} # end of epix_parse_filename


function ePiX_usage()
{
cat <<HELP
Usage: $PROG [options] <file(s)>[$1]

HELP
} # End of ePiX_usage


function ePiX_version 
{
    my_decode="tr '[a-m][n-z]' '[n-z][a-m]'" # Mac OS X has no 'rot'
    SPAMMY="<$(echo nujnat|$my_decode)@$(echo zngupf|$my_decode)."
    SPAMMY="${SPAMMY}$(echo ubylpebff|$my_decode).$(echo rqh|$my_decode)>"

cat <<VERSION
$PROG is part of ePiX, Version EPIX_VERSION_NUMBER

Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
Andrew D. Hwang  $SPAMMY
Department of Mathematics and Computer Science
College of the Holy Cross
Worcester, MA, 01610-2395, USA

VERSION
} # end of ePiX_version

function ePiX_license
{
cat <<LICENSE
  ePiX is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  ePiX is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
  License for more details.
 
  You should have received a copy of the GNU General Public License
  along with ePiX; if not, write to the Free Software Foundation, Inc.,
  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

LICENSE
} # End of ePiX_version
