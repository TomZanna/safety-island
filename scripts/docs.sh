#!/usr/bin/env bash
# docs.sh [build|live]: compila docs/main.typ in build/docs/safety-island.pdf
source "$(dirname "$0")/common.sh"
mode="${1:-build}"
typst="$(get_typst)"
out="$BUILD/docs/safety-island.pdf"
mkdir -p "$BUILD/docs"
case "$mode" in
    build) "$typst" compile "$ROOT/docs/main.typ" "$out" && info "ok: build/docs/safety-island.pdf" ;;
    live)
        info "typst watch: salva docs/*.typ e il PDF si rigenera (Ctrl-C per uscire)"
        info "PDF: $out"
        is_wsl && info "da Windows: $(wslpath -w "$out")"
        open=(); [ -z "${DOCS_VIEWER:-}" ] || open=(--open "$DOCS_VIEWER")
        exec "$typst" watch "${open[@]}" "$ROOT/docs/main.typ" "$out" ;;
    *) die "uso: docs.sh [build|live]" ;;
esac
