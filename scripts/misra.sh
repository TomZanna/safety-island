#!/usr/bin/env bash
# cppcheck con addon MISRA C:2012 (non certificato) sui sorgenti fw/.
#
# Un solo passaggio: cppcheck produce il report COMPLETO (senza soppressioni),
# poi scripts/misra_check.py lo confronta con quality/misra-suppressions.txt.
# Il confronto e' nei due sensi: finding nuovi = regressione, righe di baseline
# senza finding = deviazione obsoleta (da rivalutare, non da copiare).
#
# Uso: misra.sh [--check]
#   senza --check: genera il report e il riepilogo, non fallisce sui finding
#   --check:       fallisce su finding nuovi o soppressioni obsolete
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
# --project relativo: cppcheck riporta i path relativi alla radice del repo,
# cosi' report e baseline restano leggibili e confrontabili.
db_arg="${db#"$ROOT"/}"
args=(
    --project="$db_arg"
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
    check_args=(--check)
else
    check_args=()
fi
info "cppcheck MISRA (non certificato) -> $report"
"$tool" "${args[@]}" || die "cppcheck fallito: vedi l'output sopra"
python3 "$ROOT/scripts/misra_check.py" \
    --report "$report" --suppressions "$supp" "${check_args[@]}" \
    || die "MISRA: baseline non rispettata (dettagli sopra, report in $report)"
exit 0
