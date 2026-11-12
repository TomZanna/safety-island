#!/usr/bin/env bash
# J-Link GDB server (nativo in WSL, J-Link condiviso con usbipd) + gdb.
# ATTENZIONE: nei test l'attach J-Link ha resettato la board (torna in SDP) e
# funziona solo su M33_0. Vedi README, sezione "Limiti noti".
source "$(dirname "$0")/common.sh"
device="MIMX94398_$JLINK_CORE"
if [ -z "$ELF" ] && [ -f "$BUILD/sm-docker/sm_$SM_CONFIG.elf" ] && [ "$JLINK_CORE" = M33_0 ]; then
    ELF="$BUILD/sm-docker/sm_$SM_CONFIG.elf"; info "simboli SM: $ELF"
fi
tools="$(find_tools)"
gdb="${GDB:-$(compgen -G "$tools/arm-gnu-toolchain-*-x86_64-arm-none-eabi/bin/arm-none-eabi-gdb" | head -1)}"
[ -x "$gdb" ] || die "arm-none-eabi-gdb non trovato (imposta ARM_TOOLS)"
command -v JLinkGDBServerCLExe >/dev/null || die "JLinkGDBServerCLExe non trovato (installa J-Link)"

JLinkGDBServerCLExe -device "$device" -if SWD -speed 4000 -port "$JLINK_PORT" \
    -noir -nogui >"$BUILD/jlink-gdbserver.log" 2>&1 &
srv=$!
trap 'kill $srv 2>/dev/null || true' EXIT
sleep 3
kill -0 $srv 2>/dev/null || { tail -20 "$BUILD/jlink-gdbserver.log"; die "gdb server non partito"; }
info "gdb server su :$JLINK_PORT ($device)"
if [ -n "${GDB_BATCH:-}" ]; then
    # istantanea non interattiva: dove si trova il core, poi lo fa ripartire
    # (l'attach lo ferma e il detach da solo non lo riavvia)
    "$gdb" -q -batch ${ELF:+"$ELF"} -ex "target remote :$JLINK_PORT" \
        -ex "info registers pc" -ex "bt 5" -ex "monitor go" -ex "detach"
else
    info "l'attach ha fermato il core: 'continue' per farlo ripartire prima di uscire"
    "$gdb" ${ELF:+"$ELF"} -ex "target remote :$JLINK_PORT"
fi
