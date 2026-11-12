#!/usr/bin/env bash
# Console del debug monitor SM (Ctrl-A K per uscire da screen).
source "$(dirname "$0")/common.sh"
[ -c "$SM_UART" ] || die "$SM_UART non trovata"
if command -v picocom >/dev/null; then exec picocom -b 115200 "$SM_UART"; fi
exec screen "$SM_UART" 115200
