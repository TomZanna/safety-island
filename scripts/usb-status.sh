#!/usr/bin/env bash
# Stato USB: servizio di attach, dispositivi attaccati a WSL, seriali stabili.
source "$(dirname "$0")/common.sh"
if command -v systemctl >/dev/null && [ -d /run/systemd/system ]; then
    echo "== servizio safety-island-usb: $(systemctl is-active safety-island-usb 2>/dev/null || true)"
    journalctl -u safety-island-usb -n 8 --no-pager -o cat 2>/dev/null || true
else
    echo "== servizio safety-island-usb: systemd non attivo (vedi 'make usb-setup')"
fi
echo "== attaccati a WSL (usbip port)"
usbip port 2>/dev/null | grep -E '\([0-9a-f]{4}:[0-9a-f]{4}\)' | sed 's/^ */  /' || echo "  (usbip non installato: make usb-setup)"
echo "== seriali (/dev/serial/by-id)"
ls -l /dev/serial/by-id/ 2>/dev/null | awk 'NR>1 {print "  " $9 " -> " $11}' || echo "  nessuna"
echo "== uuu"
if [ -n "${UUU:-}" ] && [ -x "$UUU" ]; then "$UUU" -lsusb 2>&1 | grep -E 'SDP|FB' | sed 's/^/  /' || echo "  nessuna board in SDP/fastboot"; fi
