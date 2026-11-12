# shellcheck shell=bash
# Funzioni e variabili comuni. Va "sourced", non eseguito.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="$ROOT/build"
DEPS="$BUILD/_deps"
mkdir -p "$DEPS"

die()  { echo "ERRORE: $*" >&2; exit 1; }
info() { echo "==> $*"; }

# Carica i default di config.mk ("VAR ?= valore  # commento") senza far valere
# quanto già presente nell'ambiente (il Makefile esporta le sue variabili).
load_config() {
    local line key val
    while IFS= read -r line; do
        [[ "$line" =~ ^([A-Z0-9_]+)[[:space:]]*\?=[[:space:]]*([^#]*) ]] || continue
        key="${BASH_REMATCH[1]}"; val="${BASH_REMATCH[2]}"
        val="${val%"${val##*[![:space:]]}"}"
        [ -n "${!key+x}" ] || export "$key=$val"
    done < "$ROOT/config.mk"
}
load_config

is_wsl() { grep -qi microsoft /proc/version 2>/dev/null; }

# Scarica (o copia dal repo locale) un tag in una directory; stampa il percorso.
fetch_tag() {  # fetch_tag <nome> <repo> <tag> [<sorgente locale>]
    local name="$1" repo="$2" tag="$3" src="${4:-}" dst="$DEPS/$1-$3"
    if [ ! -e "$dst/.fetched" ]; then
        rm -rf "$dst"; mkdir -p "$dst"
        if [ -n "$src" ]; then
            info "$name $tag da $src" >&2
            git -C "$src" archive "$tag" | tar -x -C "$dst"
        else
            info "clono $name $tag" >&2
            git clone -q -c advice.detachedHead=false --depth 1 --branch "$tag" "$repo" "$dst.git"
            git -C "$dst.git" archive HEAD | tar -x -C "$dst"
            git -C "$dst.git" rev-parse --short=8 HEAD > "$dst/.commit"
            rm -rf "$dst.git"
        fi
        touch "$dst/.fetched"
    fi
    echo "$dst"
}

find_tools() {
    if [ -n "${TOOLS:-}" ]; then echo "$TOOLS"; return; fi
    local d
    for d in /opt/tools /; do
        if compgen -G "$d/arm-gnu-toolchain-*-x86_64-arm-none-eabi" >/dev/null; then echo "$d"; return; fi
    done
    die "toolchain arm-gnu-toolchain-*-x86_64-arm-none-eabi non trovato: imposta TOOLS=<dir che lo contiene>"
}

# mkimage_imx8: usa $MKIMAGE se impostato, altrimenti scarica e compila.
get_mkimage() {
    if [ -n "${MKIMAGE:-}" ]; then echo "$MKIMAGE"; return; fi
    local dir
    dir="$(fetch_tag imx-mkimage "$MKIMAGE_REPO" "$MKIMAGE_TAG" "$MKIMAGE_SRC")"
    if [ ! -x "$dir/mkimage_imx8" ]; then
        info "compilo mkimage_imx8" >&2
        # build_info.h normalmente viene da git: lo generiamo dal commit noto
        local c; c="$(cat "$dir/.commit" 2>/dev/null || git -C "$MKIMAGE_SRC" rev-parse --short=8 "$MKIMAGE_TAG")"
        printf '#define MKIMAGE_COMMIT 0x%s\n' "$c" > "$dir/src/build_info.h"
        (cd "$dir" \
            && make -s CFLAGS="-O2 -Wall -std=c99" SOC=iMX94 bin >&2)  # il Makefile NXP usa $(PWD)
    fi
    echo "$dir/mkimage_imx8"
}

# typst: usa quello nel PATH, altrimenti scarica la versione pinnata (checksum verificato).
get_typst() {
    if command -v typst >/dev/null; then command -v typst; return; fi
    local dir="$DEPS/typst-$TYPST_VERSION" arch
    arch="$(uname -m)"; [ "$arch" = x86_64 ] || die "download typst previsto solo per x86_64: installa typst a mano"
    if [ ! -x "$dir/typst" ]; then
        info "scarico typst $TYPST_VERSION" >&2
        mkdir -p "$dir"
        curl -fsSL -o "$dir/typst.tar.xz" \
            "https://github.com/typst/typst/releases/download/v$TYPST_VERSION/typst-x86_64-unknown-linux-musl.tar.xz" \
            || die "download typst fallito"
        echo "$TYPST_SHA256  $dir/typst.tar.xz" | sha256sum -c --quiet - \
            || { rm -rf "$dir"; die "sha256 di typst non valido"; }
        tar -xf "$dir/typst.tar.xz" -C "$dir" --strip-components=1
    fi
    echo "$dir/typst"
}
