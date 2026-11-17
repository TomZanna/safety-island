#!/usr/bin/env bash
# Gate quality: precheck tool, clang-format, clang-tidy e (opzionale) MISRA.
#
# Uso: check.sh [--strict]
#   senza --strict: MISRA e' un report, non blocca
#   --strict:       anche misra-check deve passare
source "$(dirname "$0")/common.sh"

strict=0
case "${1:-}" in
    --strict) strict=1 ;;
    "") ;;
    *) die "argomento sconosciuto: $1 (usa --strict)" ;;
esac

# Fail-fast: se manca un tool non si arriva a format/lint.
"$ROOT/scripts/quality-tools.sh" all
"$ROOT/scripts/format.sh" --check
"$ROOT/scripts/lint.py"
if [ "$strict" -eq 1 ]; then
    "$ROOT/scripts/misra.sh" --check
else
    "$ROOT/scripts/misra.sh"
fi
