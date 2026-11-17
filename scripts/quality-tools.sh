#!/usr/bin/env bash
# Verifica i tool di qualita' opzionali. Fail-fast se manca un tool; warning se
# la versione e' sotto la minima attesa (le build delle distro possono
# differire, quindi non si confronta uno sha256).
#
# Uso: quality-tools.sh [all|format|tidy|misra]
# Ambiente (passato dai target CMake): SI_CLANG_FORMAT, SI_CLANG_TIDY,
# SI_CPPCHECK, SI_CLANG_MIN_VERSION, SI_CPPCHECK_MIN_VERSION.
source "$(dirname "$0")/common.sh"

mode="${1:-all}"
clang_min="${SI_CLANG_MIN_VERSION:-19.1.7}"
cppcheck_min="${SI_CPPCHECK_MIN_VERSION:-2.19.0}"

resolve() {
    local p="$1" fallback="$2"
    if [ -n "$p" ] && [ -x "$p" ]; then echo "$p"; return; fi
    if [ -n "$p" ] && command -v "$p" >/dev/null 2>&1; then command -v "$p"; return; fi
    if command -v "$fallback" >/dev/null 2>&1; then command -v "$fallback"; return; fi
    echo ""
}

version_of() {
    "$1" --version 2>&1 | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1
}

version_ge() {
    [ "$(printf '%s\n%s\n' "$2" "$1" | sort -V | head -1)" = "$2" ]
}

check() {
    local name="$1" path="$2" minimum="$3" install="$4" version
    path="$(resolve "$path" "$name")"
    [ -n "$path" ] || die "$name assente: $install"
    version="$(version_of "$path")" || true
    [ -n "$version" ] || die "$name: impossibile leggere la versione da $path"
    info "$name $version ($path)"
    if ! version_ge "$version" "$minimum"; then
        echo "ATTENZIONE: $name $version < $minimum; proseguo, ma la resa puo' cambiare" >&2
    fi
}

check_format() {
    check clang-format "${SI_CLANG_FORMAT:-clang-format-19}" "$clang_min" \
        "sudo apt install clang-format-19"
}

check_tidy() {
    check clang-tidy "${SI_CLANG_TIDY:-clang-tidy-19}" "$clang_min" \
        "sudo apt install clang-tidy-19"
}

check_misra() {
    check cppcheck "${SI_CPPCHECK:-cppcheck}" "$cppcheck_min" \
        "sudo apt install cppcheck"
}

case "$mode" in
    all) check_format; check_tidy; check_misra ;;
    format) check_format ;;
    tidy) check_tidy ;;
    misra) check_misra ;;
    *) die "modo sconosciuto: $mode (all|format|tidy|misra)" ;;
esac
