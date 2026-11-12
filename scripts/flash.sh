#!/usr/bin/env bash
# Carica build/flash.bin IN RAM con uuu (SDPS boot): nessuna scrittura su SD/eMMC.
# Su WSL usa uuu.exe (la ROM della board resta lato Windows).
source "$(dirname "$0")/common.sh"
img="${1:-$BUILD/flash.bin}"
[ -f "$img" ] || die "$img non esiste: esegui prima 'make image'"

if is_wsl; then
    UUU="${UUU:-$(command -v uuu.exe || true)}"
    [ -n "$UUU" ] || die "uuu.exe non trovato nel PATH (imposta UUU=...)"
    # uuu.exe legge solo percorsi Windows "normali": copia in %TEMP%
    wintmp="$(cd /mnt/c && cmd.exe /c 'echo %TEMP%' 2>/dev/null | tr -d '\r')"
    tmpdir="$(wslpath -u "$wintmp")"
    cp "$img" "$tmpdir/safety-island-flash.bin"
    uuu_img="$wintmp\\safety-island-flash.bin"
    cd /mnt/c   # uuu.exe da una cwd UNC (\\wsl$) si lamenta
else
    UUU="${UUU:-$(command -v uuu || true)}"
    [ -n "$UUU" ] || die "uuu non trovato (installalo o imposta UUU=...)"
    uuu_img="$img"
fi

sdp_present() { "$UUU" -lsusb 2>&1 | grep -q 'MX943'; }

if ! sdp_present; then
    info "board non in SDP: provo il reset dal monitor SM"
    ( "$ROOT/scripts/reset.sh" ) || true
    for _ in $(seq 1 15); do sleep 1; sdp_present && break; done
fi
sdp_present || die "nessuna board MX943 in serial download (SW boot-mode? cavo USB?)"

info "uuu SDPS: boot -f $uuu_img"
timeout 120 "$UUU" SDPS: boot -f "$uuu_img" 2>&1 | tr '\r' '\n' | grep -E 'Okay|Fail|Error' | tail -3
