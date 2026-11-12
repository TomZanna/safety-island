#!/usr/bin/env bash
# Riporta la board in serial download (SDP) con il comando "reset" del monitor SM.
source "$(dirname "$0")/common.sh"
[ -c "$SM_UART" ] || die "$SM_UART non trovata (usbipd attach? imposta SM_UART=...)"
stty -F "$SM_UART" 115200 raw -echo
# La porta resta aperta (fd 3) finche' i caratteri non sono partiti: un open-write-close
# immediato sull'adattatore USB-seriale li perde e il reset non arriva.
exec 3<>"$SM_UART"
printf 'reset\r\n' >&3
sleep 1
exec 3>&-
info "reset inviato su $SM_UART"
