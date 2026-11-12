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

# Il monitor SM risponde? (prompt ">$"). Serve perche' uuu -lsusb continua a elencare
# la ROM (MX943) anche quando l'SM sta gia' girando: non e' un indicatore affidabile.
monitor_alive() {
    [ -c "$SM_UART" ] || return 1
    local tmp; tmp="$(mktemp)"
    stty -F "$SM_UART" 115200 raw -echo
    exec 4<>"$SM_UART"
    timeout 1.5 cat <&4 >"$tmp" & local rd=$!
    sleep 0.2; printf '\r\n' >&4; wait $rd || true
    exec 4>&-
    grep -q '>\$' "$tmp"; local rc=$?; rm -f "$tmp"; return $rc
}

if monitor_alive; then
    info "SM in esecuzione: reset dal monitor per tornare in serial download"
    "$ROOT/scripts/reset.sh"
    sleep 3
fi
for _ in $(seq 1 15); do sdp_present && break; sleep 1; done
sdp_present || die "nessuna board MX943 in serial download (SW boot-mode? cavo USB?)"

info "uuu SDPS: boot -f $uuu_img"
log="$(timeout 120 "$UUU" SDPS: boot -f "$uuu_img" 2>&1 | tr '\r' '\n')" || true
echo "$log" | grep -E 'Okay|Fail|Error' | tail -3
echo "$log" | grep -q 'Fail\|Error' && die "caricamento fallito: reset manuale della board e riprova"
info "caricata in RAM"
exit 0
