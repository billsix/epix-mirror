#!/bin/bash
# Run every reset_state() check; report all, fail if any failed.  Run from the repo
# root inside the container: make shell-exec SCRIPT=tasks/adhoc/reset-global-state-between-figures/run_checks.sh
here="$(dirname "$0")"
status=0
python3 "$here/repro.py" || status=1
pass=0; fail=0
for nb in notebooks/*.py; do
    name="$(basename "$nb" .py)"
    [ "$name" = histogram ] && continue  # its data file is absent upstream
    if python3 build-aux/verify_ports.py "$name" > /tmp/vp.out 2>&1; then
        pass=$((pass + 1))
    else
        fail=$((fail + 1)); cat /tmp/vp.out; status=1
    fi
done
echo "harness: $pass PASS, $fail FAIL"
python3 "$here/one_process_check.py" all || status=1
echo "CHECKS_EXIT=$status"
exit $status
