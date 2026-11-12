#!/bin/sh
# date "congelata" alla data del commit (SOURCE_DATE_EPOCH) per build riproducibili
if [ -n "${SOURCE_DATE_EPOCH:-}" ]; then exec /bin/date -u -d "@${SOURCE_DATE_EPOCH}" "$@"; fi
exec /bin/date "$@"
