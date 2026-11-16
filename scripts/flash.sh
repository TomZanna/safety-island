#!/usr/bin/env bash
# Carica build/flash.bin IN RAM con uuu (SDPS boot): nessuna scrittura su SD/eMMC.
# uuu Linux nativo (UUU, scaricato dal target CMake): in WSL la ROM della board (1fc9:0027) deve essere
# attaccata con usbip (servizio safety-island-usb, 'make usb-setup'). UUU=uuu.exe usa invece uuu Windows.
source "$(dirname "$0")/common.sh"
img="${1:-$BUILD/flash.bin}"
[ -f "$img" ] || die "$img non esiste: esegui prima 'make image'"
UUU="${UUU:-$(command -v uuu || true)}"
[ -n "$UUU" ] || die "uuu non trovato (imposta UUU=...)"

if [[ "$UUU" == *.exe ]]; then
    # uuu.exe legge solo percorsi Windows "normali": copia in %TEMP%
    wintmp="$(cd /mnt/c && cmd.exe /c 'echo %TEMP%' 2>/dev/null | tr -d '\r')"
    tmpdir="$(wslpath -u "$wintmp")"
    cp "$img" "$tmpdir/safety-island-flash.bin"
    uuu_img="$wintmp\\safety-island-flash.bin"
    cd /mnt/c   # uuu.exe da una cwd UNC (\\wsl$) si lamenta
else
    uuu_img="$img"
fi

sdp_present() { "$UUU" -lsusb 2>&1 | grep -q 'MX943'; }

# Prima di caricare si resetta sempre la board (scripts/reset.sh, RESET_METHOD: di default il reset
# hardware via J-Link): funziona qualunque sia lo stato (SM bloccato, app in crash, ROM gia' in SDP).
# Non si puo' capire dallo stato USB se la ROM aspetta davvero: uuu -lsusb elenca la ROM (MX943)
# anche quando l'SM sta gia' girando. FLASH_NO_RESET=1 salta il reset (board appena resettata a mano).
load() {
    if [ -z "${FLASH_NO_RESET:-}" ]; then
        # l'hub RTT resterebbe collegato a core che si resettano: lo si chiude (i terminali si chiudono)
        "$ROOT/scripts/rtt.sh" --stop
        RESET_METHOD="${1:-$RESET_METHOD}" "$ROOT/scripts/reset.sh" || return 1
        sleep 1.5   # la ROM si ri-enumera: in WSL il servizio usbip la ri-attacca in ~1-2 s
    fi
    for _ in $(seq 1 10); do sdp_present && break; sleep 1; done   # di solito ~4 s
    sdp_present || { echo "nessuna board MX943 in serial download (boot switch? cavo USB? in WSL: 'make usb-status')" >&2; return 1; }
    info "uuu SDPS: boot -f $uuu_img"
    # Esito dal codice d'uscita di uuu (0 = ok, 255 = errore), non dal testo: in un terminale uuu stampa
    # anche "Success 0    Failure 0", che non e' un errore.
    local log rc
    log="$(mktemp)"
    rc=0
    timeout 120 "$UUU" SDPS: boot -f "$uuu_img" >"$log" 2>&1 || rc=$?
    tr '\r' '\n' < "$log" | sed 's/\x1b\[[0-9;?]*[A-Za-z]//g' | grep -E 'Okay|Error|Fail[^u]' | tail -3
    rm -f "$log"
    [ $rc -eq 124 ] && echo "uuu: timeout (120 s)" >&2
    return $rc
}

: "${RESET_METHOD:=auto}"
if ! load; then
    # Secondo tentativo. Se il primo reset ha seguito i boot switch (J-Link, monitor) e la board e' partita
    # da SD/eMMC con un'immagine valida, la ROM non va in serial download: BCU invece forza il boot "usb".
    retry="$RESET_METHOD"
    if [ -n "${BCU_BOARD:-}" ] && [ "$RESET_METHOD" != bcu ]; then retry=bcu; fi
    info "caricamento non riuscito: secondo tentativo con reset $retry"
    load "$retry" || die "caricamento fallito (vedi sopra). Prova 'make reset' o un reset a mano"
fi
info "caricata in RAM"
exit 0
