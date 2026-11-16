# shellcheck shell=bash
# Funzioni comuni agli script. Va "sourced", non eseguito.
# Gli script sono chiamati dai target CMake, che passano la configurazione nell'ambiente
# (cmake -E env ...). I default qui sotto servono solo per lanciarli a mano.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="${SI_BUILD_DIR:-$ROOT/build/mx94evk}"
DEPS="$BUILD/_deps"
mkdir -p "$DEPS"

: "${SM_UART:=/dev/serial/by-id/usb-FTDI_Quad_RS232-HS-if03-port0}"
: "${SM_CONFIG:=mx94evk}"
: "${JLINK_CORE:=M33_0}"
: "${JLINK_PORT:=2331}"
: "${ELF:=}"

die()  { echo "ERRORE: $*" >&2; exit 1; }
info() { echo "==> $*"; }

is_wsl() { grep -qi microsoft /proc/version 2>/dev/null; }

