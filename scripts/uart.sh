#!/usr/bin/env bash
# Console seriale a 115200 (picocom: Ctrl-A Ctrl-X per uscire; screen: Ctrl-A K).
# Porta: UART_PORT se impostata (anche un glob, es. /dev/serial/by-id/usb-FTDI_TTL232R-*), altrimenti
# SM_UART (debug monitor SM).
source "$(dirname "$0")/common.sh"
port="${UART_PORT:-$SM_UART}"
[ -n "$port" ] || die "nessuna porta: imposta la variabile CMake della console (es. M71_UART_PORT)"
# shellcheck disable=SC2086
match=$(compgen -G "$port" | head -1 || true)
[ -n "$match" ] && [ -c "$match" ] || die "$port non trovata ('make usb-status' per le seriali presenti)"
info "console $match"
if command -v picocom >/dev/null; then exec picocom -q -b 115200 "$match"; fi
exec screen "$match" 115200
