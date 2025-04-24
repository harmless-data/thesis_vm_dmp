#!/bin/bash

set -euo pipefail

# Output filenames
PERF_SCRIPT="perf.unfolded"
FOLDED="perf.folded"
SVG="perf.svg"

# Reset old data
rm -f "$PERF_SCRIPT" "$FOLDED" "$SVG"

echo "[+] Generating perf script output..."
sudo perf script > "$PERF_SCRIPT"

echo "[+] Collapsing stack frames..."
../../FlameGraph/stackcollapse-perf.pl "$PERF_SCRIPT" > "$FOLDED"

echo "[+] Top stacks (collapsed counts):"
cat "$FOLDED" | sort | uniq -c | sort -nr | head

echo "[+] Generating flamegraph SVG..."
../../FlameGraph/flamegraph.pl "$FOLDED" > "$SVG"

echo "[✓] Flamegraph created: $SVG"