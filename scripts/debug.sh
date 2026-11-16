#!/usr/bin/env bash
# J-Link GDB server (nativo in WSL, J-Link condiviso con usbipd) + gdb, su un core alla volta.
# M33_0 = SM; M33_1, M7_0, M7_1 = attach a un'app gia' in esecuzione (caricata da uuu con flash.bin):
# per quei core J-Link non fa reset ne' download.
#
# ATTENZIONE: se un core muxa LPUART8 sui pad DAP_TCLK_SWCLK/DAP_TMS_SWDIO (console di default del
# hello_world SDK per M33S) il DAP non risponde piu' e il ripiego "connect under reset" del J-Link
# resetta TUTTA la board (torna in SDP). Vedi README, "Debug J-Link".
source "$(dirname "$0")/common.sh"
device="MIMX94398_$JLINK_CORE"
case "$JLINK_CORE" in
    M33_0) app_elf="$BUILD/sm-docker/sm_$SM_CONFIG.elf" ;;
    M33_1) app_elf="${APPS_OUT:-$BUILD/fw}/m33s/m33s.elf" ;;
    M7_0)  app_elf="${APPS_OUT:-$BUILD/fw}/m70/m70.elf" ;;
    M7_1)  app_elf="${APPS_OUT:-$BUILD/fw}/m71/m71.elf" ;;
    *) die "JLINK_CORE non valido: $JLINK_CORE (M33_0 | M33_1 | M7_0 | M7_1)" ;;
esac
if [ -z "$ELF" ] && [ -f "$app_elf" ]; then ELF="$app_elf"; fi
if [ -n "$ELF" ]; then info "simboli: $ELF"; else info "nessun ELF per $JLINK_CORE: gdb senza simboli"; fi
gdb="${GDB:-$(command -v arm-none-eabi-gdb || true)}"
[ -x "$gdb" ] || die "arm-none-eabi-gdb non trovato ($gdb): 'make deps' scarica il toolchain"
command -v JLinkGDBServerCLExe >/dev/null || die "JLinkGDBServerCLExe non trovato (installa J-Link)"

log="$BUILD/jlink-gdbserver-$JLINK_CORE.log"
JLinkGDBServerCLExe -device "$device" -if SWD -speed 4000 -port "$JLINK_PORT" \
    -noir -nogui >"$log" 2>&1 &
srv=$!
trap 'kill $srv 2>/dev/null || true' EXIT
# Pronto quando il server aspetta gdb; se il connect fallisce esce da solo
for _ in $(seq 1 30); do
    grep -q 'Waiting for GDB connection' "$log" && break
    kill -0 $srv 2>/dev/null || break
    sleep 0.5
done
if ! grep -q 'Waiting for GDB connection' "$log"; then
    tail -20 "$log"
    die "gdb server non connesso a $device (la board potrebbe essere tornata in SDP: 'make run')"
fi
info "gdb server su :$JLINK_PORT ($device), log: $log"
# App di fw/: sblocca l'eventuale attesa del debugger (WAIT_DEBUGGER) appena attaccati
go=()
if [ "$JLINK_CORE" != M33_0 ] && [ -n "$ELF" ]; then go=(-ex "set var g_debugger_go = 1"); fi
if [ -n "${GDB_BATCH:-}" ]; then
    # istantanea non interattiva: dove si trova il core, poi lo fa ripartire
    # (l'attach lo ferma e il detach da solo non lo riavvia)
    "$gdb" -q -batch ${ELF:+"$ELF"} -ex "target remote :$JLINK_PORT" "${go[@]}" \
        -ex "info registers pc" -ex "bt 5" -ex "monitor go" -ex "detach"
else
    info "l'attach ha fermato il core: 'continue' per farlo ripartire prima di uscire"
    "$gdb" ${ELF:+"$ELF"} -ex "target remote :$JLINK_PORT" "${go[@]}"
fi
