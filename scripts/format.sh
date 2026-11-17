#!/usr/bin/env bash
# clang-format sui sorgenti C/H nostri (fw/, esclusa la libreria FSoE vendored).
#
# Uso: format.sh [--check|--fix]
#   --check  dry-run con --Werror: esce 1 e stampa i file da formattare
#   --fix    applica clang-format -i
# Ambiente: SI_CLANG_FORMAT, SI_CLANG_MIN_VERSION (default 19.1.7).
source "$(dirname "$0")/common.sh"

mode="--check"
case "${1:-}" in
    --check) mode="--check" ;;
    --fix) mode="--fix" ;;
    "") ;;
    *) die "argomento sconosciuto: $1 (usa --check o --fix)" ;;
esac

tool="${SI_CLANG_FORMAT:-clang-format-19}"
if [ ! -x "$tool" ] && ! command -v "$tool" >/dev/null 2>&1; then
    die "clang-format assente: sudo apt install clang-format-19"
fi
command -v "$tool" >/dev/null 2>&1 && tool="$(command -v "$tool")"

cd "$ROOT"
mapfile -t files < <(git ls-files 'fw/*.c' 'fw/*.h' | grep -v '^fw/common/fsoe/')
[ "${#files[@]}" -gt 0 ] || die "nessun sorgente C/H da formattare"

if [ "$mode" = "--fix" ]; then
    info "clang-format -i su ${#files[@]} file"
    "$tool" -i "${files[@]}"
    exit 0
fi

failed=()
for f in "${files[@]}"; do
    if ! "$tool" --dry-run --Werror "$f" >/dev/null 2>&1; then
        failed+=("$f")
    fi
done
if [ "${#failed[@]}" -gt 0 ]; then
    printf 'da formattare:\n' >&2
    printf '  %s\n' "${failed[@]}" >&2
    die "${#failed[@]} file non formattati: esegui 'make format'"
fi
info "clang-format: ${#files[@]} file gia' formattati"
