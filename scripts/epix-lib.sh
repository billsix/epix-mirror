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

declare EPIX_NOTFOUND

declare LATEX_PKGS

QUIET=0

function ePiX_echo {
    if [ "$QUIET" -eq 0 ];
    then
        echo "$@" | tee -a "$EPIX_LOGFILE"
    else
        echo "$@" >> "$EPIX_LOGFILE"
    fi
}

# in increasing order of severity
function ePiX_msg {
    echo -e "$PROG: $@" | tee -a "$EPIX_LOGFILE"
}

function ePiX_warn {
    echo -e "$PROG: WARNING: $@" | tee -a "$EPIX_LOGFILE" >&2
}

function ePiX_fail {
    echo -e "$PROG: WARNING: $@" | tee -a "$EPIX_LOGFILE" >&2
    if [ "$EPIX_OUTROOT" = "$EPIX_INROOT" ]; then unset EPIX_OUTROOT; fi
    let failure=failure+1
}

function ePiX_die {
    echo -e "$PROG: ERROR: $@" | tee -a "$EPIX_LOGFILE" >&2
    echo -e "Transcript written on ${EPIX_LOGFILE}\n"
    exit 1;
}

# Echo command to stderr/stdout and logfile, then run command
#
function ePiX_command {
    ePiX_msg "$@"
    ("$@" 2>&1) | tee -a "$EPIX_LOGFILE"
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


# Get extra LaTeX package names from file
function ePiX_usepackages()
{
    TMP_PKGS=$(grep "%% usepackages " $1)
    LATEX_PKGS=${TMP_PKGS#"%% usepackages "}
}

function ePiX_usage()
{
cat <<HELP
Usage: $1 [options] <file(s)>[$2]

HELP
}


function ePiX_version()
{
cat <<VERSION
$1 (ePiX 1.2.22)

Copyright (C) 2007  Andrew D. Hwang

VERSION
}

function ePiX_license()
{
cat <<LICENSE
License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>
ePiX is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

LICENSE
}

function ePiX_bugreport()
{
    my_decode="tr '[a-m][n-z]' '[n-z][a-m]'" # Mac OS X has no 'rot'
    SPAMMY="<$(echo nujnat|$my_decode)@$(echo zngupf|$my_decode)."
    SPAMMY="${SPAMMY}$(echo ubylpebff|$my_decode).$(echo rqh|$my_decode)>"

cat <<BUG
Please report bugs to $SPAMMY
BUG
}