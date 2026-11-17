#!/usr/bin/env bash
# Console RTT da terminale (scripts/rtt.py nel venv di ext/, pylink), fuori dal debug VS Code.
# RTT_CHANNEL=m33s|m70|m71 per un solo core, vuoto per tutti. RTT_CORE = core a cui si collega il J-Link
# (default M7_1). Durante il debug in VS Code l'RTT lo legge Cortex-Debug: non aprire la console sugli
# stessi canali (due lettori si rubano i dati).
source "$(dirname "$0")/common.sh"
py="$ROOT/ext/venv/bin/python"
[ -x "$py" ] || die "venv di ext/ assente: 'make py-deps' (o 'make deps')"
exec "$py" -I "$ROOT/scripts/rtt.py" ${RTT_CHANNEL:+--channel "$RTT_CHANNEL"} ${RTT_CORE:+--core "$RTT_CORE"}
