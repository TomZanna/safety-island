#!/usr/bin/env bash
# Resetta la board, che riparte dalla ROM in serial download (SDP) se i boot switch sono su serial
# download (o, con BCU, forzando il boot mode "usb").
#
# RESET_METHOD (default auto):
#   jlink    pin nRESET del connettore di debug (J-Link Commander r0/r1): come premere il pulsante di
#            reset. Hardware, non dipende dallo stato dell'SM. Serve solo la sonda, nessun core.
#   bcu      NXP Board Control Utility sull'FTDI della EVK: reset + boot mode "usb" forzato (resta
#            forzato: -keep; "bcu deinit -board=..." per tornare ai boot switch).
#            Serve BCU_BOARD (imx943evk19a0 + SW7-1 ON | imx943evk19b1 + SW7-1 OFF).
#   monitor  comando "reset" del debug monitor dell'SM: funziona solo se l'SM gira e risponde.
#   auto     jlink, poi bcu (se BCU_BOARD e' impostato), poi monitor: il primo che riesce.
source "$(dirname "$0")/common.sh"
: "${RESET_METHOD:=auto}"

reset_jlink() {
    command -v JLinkExe >/dev/null || { echo "JLinkExe non trovato"; return 1; }
    local cmd out
    cmd="$(mktemp)"
    printf 'r0\nsleep 200\nr1\nexit\n' > "$cmd"
    out="$(timeout 20 JLinkExe -nogui 1 -autoconnect 0 -CommandFile "$cmd" 2>&1)"; local rc=$?
    rm -f "$cmd"
    # Senza sonda JLinkExe esce comunque con 0: controlla che abbia parlato col J-Link
    if [ $rc -ne 0 ] || ! grep -q '^VTref=' <<< "$out"; then
        echo "J-Link non raggiungibile ($(grep -m1 -iE 'fail|error' <<< "$out"))"
        return 1
    fi
    grep -q '^VTref=0\.0' <<< "$out" && { echo "VTref 0 V: board spenta?"; return 1; }
    return 0
}

reset_bcu() {
    [ -n "${BCU_BOARD:-}" ] || { echo "BCU_BOARD non impostato"; return 1; }
    [ -x "${BCU:-}" ] || { echo "bcu non disponibile"; return 1; }
    local out id="${BCU_ID:-}"
    # location_id dell'FTDI quad della EVK (0403:6011): senza, bcu puo' scegliere un altro FTDI (es. il
    # cavo della M33S). Cambia con l'ordine di attach usbip, quindi si ricava ogni volta da sysfs.
    if [ -z "$id" ]; then
        for d in /sys/bus/usb/devices/*; do
            [ "$(cat "$d/idVendor" 2>/dev/null):$(cat "$d/idProduct" 2>/dev/null)" = 0403:6011 ] && { id="$(basename "$d")"; break; }
        done
    fi
    [ -n "$id" ] || { echo "FTDI quad della EVK (0403:6011) non trovato"; return 1; }
    out="$(timeout 30 "$BCU" reset usb -board="$BCU_BOARD" -id="$id" -keep 2>&1)"
    grep -qi 'failed' <<< "$out" && { echo "$(grep -m1 -i failed <<< "$out") (SW7-1? BCU_BOARD?)"; return 1; }
    return 0
}

reset_monitor() {
    [ -c "$SM_UART" ] || { echo "$SM_UART non trovata"; return 1; }
    stty -F "$SM_UART" 115200 raw -echo
    # La porta resta aperta (fd 3) finche' i caratteri non sono partiti: un open-write-close
    # immediato sull'adattatore USB-seriale li perde e il reset non arriva.
    exec 3<>"$SM_UART"
    printf 'reset\r\n' >&3
    sleep 1
    exec 3>&-
    # Nessuna conferma possibile: se l'SM non gira il comando si perde (lo dira' l'attesa della ROM)
    return 0
}

case "$RESET_METHOD" in
    auto)
        methods=(jlink)
        [ -n "${BCU_BOARD:-}" ] && methods+=(bcu)
        methods+=(monitor) ;;
    jlink|bcu|monitor) methods=("$RESET_METHOD") ;;
    *) die "RESET_METHOD non valido: $RESET_METHOD (auto | jlink | bcu | monitor)" ;;
esac
for m in "${methods[@]}"; do
    if why="$("reset_$m")"; then
        info "reset ($m)"
        exit 0
    fi
    info "reset $m non riuscito: $why"
done
die "nessun metodo di reset riuscito (${methods[*]})"
