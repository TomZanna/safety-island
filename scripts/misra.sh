#!/usr/bin/env bash
# cppcheck con addon MISRA C:2012 (non certificato) sui sorgenti fw/.
#
# Uso: misra.sh [--check]
#   senza --check: genera il report e non fallisce sui finding
#   --check:       fallisce sulle violazioni non soppresse
# Ambiente: SI_CPPCHECK, SI_CPPCHECK_MIN_VERSION, SI_BUILD_DIR.
source "$(dirname "$0")/common.sh"

mode="report"
case "${1:-}" in
    --check) mode="check" ;;
    "") ;;
    *) die "argomento sconosciuto: $1 (usa --check)" ;;
esac

tool="${SI_CPPCHECK:-cppcheck}"
if [ ! -x "$tool" ] && ! command -v "$tool" >/dev/null 2>&1; then
    die "cppcheck assente: sudo apt install cppcheck"
fi
command -v "$tool" >/dev/null 2>&1 && tool="$(command -v "$tool")"

db="$BUILD/compile_commands.json"
[ -f "$db" ] || die "compile_commands.json assente: esegui 'make fw'"
reports="$BUILD/reports"
mkdir -p "$reports"
report="$reports/misra.txt"
supp="$ROOT/quality/misra-suppressions.txt"
[ -f "$supp" ] || die "file di soppressioni mancante: $supp"

cd "$ROOT"
args=(
    --project="$db"
    --file-filter='fw/*'
    -i fw/common/fsoe
    -i ext
    -i build
    --platform=unix32
    --std=c11
    --enable=style
    --addon=misra
    --suppress=missingIncludeSystem
    --suppress=preprocessorErrorDirective
    --suppress=misra-config
    --inline-suppr
    --template='{file}:{line}:{column}: {severity}: {id}: {message}'
    --output-file="$report"
)
if [ "$mode" = "check" ]; then
    args+=(--suppressions-list="$supp" --error-exitcode=1)
fi
info "cppcheck MISRA (non certificato) -> $report"
set +e
"$tool" "${args[@]}"
status=$?
set -e
findings=0
if [ -f "$report" ]; then
    findings="$(grep -cE 'misra-c2012-' "$report" || true)"
fi
info "MISRA: $findings righe nel report (non e' una certificazione)"
if [ "$mode" = "check" ] && [ "$status" -ne 0 ]; then
    die "violazioni MISRA non soppresse: vedi $report"
fi
exit 0
