#!/usr/bin/env bash
# =============================================================================
#  imx943-flow - fault injection matrix
# =============================================================================
#
#  Runs the prototype against the failure modes of the sequence diagram and
#  checks that:
#    - the run terminates with the PASS verdict (exit code 0)
#    - the expected detection path was triggered
#
#  Usage: ./check.sh [duration_ms]
# =============================================================================

set -u

BIN=${BIN:-./build/imx943-flow}
DURATION=${1:-1500}

if [ ! -x "$BIN" ]; then
   echo "error: $BIN not found, run 'make' first" >&2
   exit 1
fi

pass=0
fail=0

run_case() {
   local name=$1
   local marker=$2
   shift 2

   local output
   local rc

   output=$("$BIN" --duration "$DURATION" "$@" 2>&1)
   rc=$?

   if [ "$rc" -eq 0 ] && printf '%s' "$output" | grep -q "$marker"; then
      printf '  [PASS] %-46s (%s)\n' "$name" "$marker"
      pass=$((pass + 1))
   else
      printf '  [FAIL] %-46s (exit %d, marker "%s")\n' "$name" "$rc" "$marker"
      printf '%s\n' "$output" | tail -25
      fail=$((fail + 1))
   fi
}

echo
echo "imx943-flow - fault injection matrix (${DURATION} ms per run)"
echo "-------------------------------------------------------------"

run_case "clean run" \
   "RESULT: PASS" \
   --fault none

run_case "injected Session ID divergence" \
   "EGRESS CROSS-CHECK FAILED" \
   --fault session --fault-cycle 2

run_case "divergent egress PDU (M33)" \
   "EGRESS CROSS-CHECK FAILED" \
   --fault pdu --fault-cycle 30

run_case "divergent decoded SafeInputs (M33)" \
   "INPUTS CROSS-CHECK FAILED" \
   --fault inputs --fault-cycle 30

run_case "divergent SafeOutputs (M33)" \
   "divergent SafeOutputs (safe logic)" \
   --fault logic --fault-cycle 30

run_case "black channel frame loss (watchdog)" \
   "WD_EXPIRED" \
   --fault busdrop --fault-cycle 20 --fault-count 60

run_case "SHMEM tx counter divergence (M33 stall)" \
   "SHMEM COUNTER MISMATCH" \
   --fault stall --fault-cycle 30

run_case "slave rejects the assembled frame (corrupt data half)" \
   "INVALID_CRC" \
   --fault frame --fault-cycle 30

echo "-------------------------------------------------------------"
echo "  $pass passed, $fail failed"
echo

[ "$fail" -eq 0 ]
