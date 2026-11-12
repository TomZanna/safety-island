# shellcheck shell=bash
# Funzioni comuni agli script. Va "sourced", non eseguito.
# Gli script sono chiamati dai target CMake, che passano la configurazione nell'ambiente
# (cmake -E env ...). I default qui sotto servono solo per lanciarli a mano.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="${SI_BUILD_DIR:-$ROOT/build/mx94evk}"
DEPS="$BUILD/_deps"
mkdir -p "$DEPS"

: "${SM_UART:=/dev/ttyUSB3}"
: "${SM_CONFIG:=mx94evk}"
: "${JLINK_CORE:=M33_0}"
: "${JLINK_PORT:=2331}"
: "${ELF:=}"
: "${TOOLS:=}"

die()  { echo "ERRORE: $*" >&2; exit 1; }
info() { echo "==> $*"; }

is_wsl() { grep -qi microsoft /proc/version 2>/dev/null; }

find_tools() {
    if [ -n "${TOOLS:-}" ]; then echo "$TOOLS"; return; fi
    local d
    for d in /opt/tools /; do
        if compgen -G "$d/arm-gnu-toolchain-*-x86_64-arm-none-eabi" >/dev/null; then echo "$d"; return; fi
    done
    die "toolchain arm-gnu-toolchain-*-x86_64-arm-none-eabi non trovato: imposta ARM_TOOLS (cache CMake)"
}
