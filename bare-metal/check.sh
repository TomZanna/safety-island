#!/bin/sh
# Host validation matrix.
set -u

BIN=${BIN:-./build/flow-host}
DURATION=${1:-1500}

if [ ! -x "$BIN" ]; then
   echo "error: $BIN not found, run 'make host' first" >&2
   exit 1
fi

pass=0
fail=0

run_case() {
   name=$1
   shift

   output=$("$BIN" --duration "$DURATION" "$@" 2>&1)
   rc=$?

   if [ "$rc" -eq 0 ] && printf '%s' "$output" | grep -q "RESULT: PASS"; then
      printf '  [PASS] %s\n' "$name"
      pass=$((pass + 1))
   else
      printf '  [FAIL] %s (exit %d)\n' "$name" "$rc"
      printf '%s\n' "$output" | tail -30
      fail=$((fail + 1))
   fi
}

echo
echo "imx943-flow bare metal host validation (${DURATION} ms per run)"
echo "-------------------------------------------------------------"

run_case "clean run" --fault none
run_case "divergent egress PDU (M33)" --fault pdu
run_case "divergent decoded SafeInputs (M33)" --fault inputs
run_case "divergent SafeOutputs (M33)" --fault logic
run_case "Session ID divergence (M33)" --fault session
run_case "SHMEM tx counter divergence (M33 stall)" --fault stall

echo "-------------------------------------------------------------"
echo "  $pass passed, $fail failed"
echo

[ "$fail" -eq 0 ]
