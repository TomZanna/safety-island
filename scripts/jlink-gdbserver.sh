#!/usr/bin/env bash
# JLinkGDBServerCLExe con ritentativi, per Cortex-Debug ("serverpath" in .vscode/launch.json).
# La DLL J-Link rifiuta l'apertura della sonda se nello stesso istante la sta aprendo un altro programma
# (altro GDB server di una sessione multi-core, hub RTT): il server stampa "Connecting to J-Link failed"
# ed esce subito, e gdb resta in attesa. Qui si riprova fino a 10 volte con una pausa casuale.
# Tutti gli argomenti passano a JLinkGDBServerCLExe; l'output resta quello del server (Cortex-Debug ci
# cerca "Waiting for GDB connection").
log="$(mktemp)"
trap 'rm -f "$log"' EXIT
for attempt in $(seq 1 10); do
    JLinkGDBServerCLExe "$@" 2>&1 | tee "$log"
    rc=${PIPESTATUS[0]}
    if grep -q 'Connecting to J-Link failed' "$log" && ! grep -q 'Waiting for GDB connection' "$log"; then
        printf "\n"; echo "[jlink-gdbserver.sh] sonda occupata (tentativo $attempt/10), riprovo..."
        sleep "$(awk 'BEGIN{srand(); printf "%.1f", 0.5 + rand()*1.5}')"
        continue
    fi
    exit "$rc"
done
printf "\n"; echo "[jlink-gdbserver.sh] J-Link non disponibile: la sonda accetta al massimo 5 connessioni (README)"
exit 1
