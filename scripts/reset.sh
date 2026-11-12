#!/usr/bin/env bash
# Riporta la board in serial download (SDP) con il comando "reset" del monitor SM.
source "$(dirname "$0")/common.sh"
[ -c "$SM_UART" ] || die "$SM_UART non trovata (usbipd attach? imposta SM_UART=...)"
stty -F "$SM_UART" 115200 raw -echo
printf '\r\nreset\r\n' > "$SM_UART"
info "reset inviato su $SM_UART"
