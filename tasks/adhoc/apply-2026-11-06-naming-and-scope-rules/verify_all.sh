#!/bin/bash
# Run the byte-identity harness on every notebook, one process per name.
cd /epix
for f in notebooks/*.py; do
    n=$(basename "$f" .py)
    python3 build-aux/verify_ports.py "$n" 2>&1 | tail -1
done
