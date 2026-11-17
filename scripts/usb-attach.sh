#!/usr/bin/env bash
# Attacca a WSL, con il client Linux "usbip", i dispositivi di board/usb-devices.conf esportati dal server
# usbipd-win, identificandoli per VID:PID. Senza --once gira in ciclo (servizio systemd): un dispositivo
# che sparisce e ricompare (la ROM SDP a ogni reset della board) viene ri-attaccato da solo.
#
# Uso: usb-attach.sh [--once] [--conf <file>]      (serve root: usbip attach)
# Ambiente: USBIP_HOST (default: gateway di default, cioe' Windows con rete WSL NAT; 127.0.0.1 se mirrored)
#           USB_POLL (secondi tra un giro e l'altro, default 1)
# Setup una tantum e policy AutoBind lato Windows: scripts/usb-setup.sh.
set -uo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
conf="$here/../board/usb-devices.conf"
once=0
while [ $# -gt 0 ]; do
    case "$1" in
        --once) once=1 ;;
        --conf) conf="$2"; shift ;;
        *) echo "argomento sconosciuto: $1" >&2; exit 2 ;;
    esac
    shift
done
command -v usbip >/dev/null || { echo "usbip non trovato: 'make usb-setup'" >&2; exit 1; }
[ -f "$conf" ] || { echo "$conf non trovato" >&2; exit 1; }

host() {
    if [ -n "${USBIP_HOST:-}" ]; then echo "$USBIP_HOST"; return; fi
    if grep -qi '^networkingMode *= *mirrored' /mnt/c/Users/*/.wslconfig 2>/dev/null; then echo 127.0.0.1; return; fi
    ip route show default | awk '{print $3; exit}'
}

mapfile -t wanted < <(awk '!/^ *(#|$)/ {print tolower($1)}' "$conf")
declare -A name
while read -r id rest; do name[${id,,}]="$rest"; done < <(grep -v '^ *\(#\|$\)' "$conf")

declare -A last_state
log() { echo "$(date '+%H:%M:%S') $*"; }

one_round() {
    local h remote attached_busids=() attached_ids=() busid id
    h="$(host)"
    # "        5-4: NXP Semiconductors : unknown product (1fc9:0027)"
    remote="$(usbip list -r "$h" 2>/dev/null | sed -n 's/^ *\([0-9][0-9.-]*\): .*(\([0-9a-f]\{4\}:[0-9a-f]\{4\}\))$/\1 \2/p')"
    # Gia' attaccati: per busid remoto se usbip lo sa (attach fatti da qui), altrimenti per VID:PID
    # (attach fatti da usbipd.exe: "unknown host, remote port and remote busid")
    local port cur_id=""
    port="$(usbip port 2>/dev/null)"
    while IFS= read -r line; do
        if [[ "$line" =~ \(([0-9a-f]{4}:[0-9a-f]{4})\)$ ]]; then cur_id="${BASH_REMATCH[1]}"; fi
        if [[ "$line" =~ usbip://[^/]+/([0-9.-]+) ]]; then attached_busids+=("${BASH_REMATCH[1]}"); fi
        if [[ "$line" == *"unknown host"* && -n "$cur_id" ]]; then attached_ids+=("$cur_id"); fi
    done <<< "$port"

    for id in "${wanted[@]}"; do
        local found=0
        while read -r busid rid; do
            [ "$rid" = "$id" ] || continue
            found=1
            if [[ " ${attached_busids[*]} " == *" $busid "* || " ${attached_ids[*]} " == *" $id "* ]]; then
                state="attaccato ($busid)"
            elif out="$(usbip attach -r "$h" -b "$busid" 2>&1)"; then
                state="attaccato ($busid)"; log "attach $id ${name[$id]} da $h busid $busid"
            else
                state="errore attach ($busid): $out"
            fi
        done <<< "$remote"
        [ $found = 1 ] || state="assente (scollegato, o per la ROM: board non in serial download; se resta cosi': policy AutoBind su Windows?)"
        if [ "${last_state[$id]:-}" != "$state" ]; then
            log "$id ${name[$id]}: $state"
            last_state[$id]="$state"
        fi
    done
}

if [ $once = 1 ]; then one_round; exit 0; fi
log "usb-attach: server $(host), dispositivi: ${wanted[*]}"
while true; do one_round; sleep "${USB_POLL:-1}"; done
