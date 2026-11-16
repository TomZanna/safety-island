#!/usr/bin/env bash
# Terminale RTT (scripts/rtt.py nel venv di ext/, pylink): RTT_CHANNEL=m33s|m70|m71 per un core, vuoto per
# tutti. Avvia al bisogno l'hub (una sola connessione J-Link per tutti i terminali).
#   rtt.sh --stop   chiude l'hub (e quindi i terminali)
# RTT_CORE = core a cui si collega l'hub (default M7_1).
source "$(dirname "$0")/common.sh"
py="$ROOT/ext/venv/bin/python"
export RTT_HUB_LOG="$BUILD/rtt-hub.log" RTT_HUB_PID="$BUILD/rtt-hub.pid"
if [ "${1:-}" = --stop ]; then
    if [ -f "$RTT_HUB_PID" ] && kill -0 "$(cat "$RTT_HUB_PID")" 2>/dev/null; then
        kill -INT "$(cat "$RTT_HUB_PID")"
        for _ in $(seq 1 20); do kill -0 "$(cat "$RTT_HUB_PID")" 2>/dev/null || break; sleep 0.1; done
        info "hub RTT chiuso"
    fi
    rm -f "$RTT_HUB_PID"
    exit 0
fi
[ -x "$py" ] || die "venv di ext/ assente: 'make deps'"
exec "$py" -I "$ROOT/scripts/rtt.py" term "${RTT_CHANNEL:-all}" ${RTT_CORE:+--core "$RTT_CORE"}
