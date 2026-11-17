#!/usr/bin/env bash
# Sincronizza le dipendenze west di deps/west.yml (SDK MCUXpresso minimo) in ext/sdk/.
# Uso: deps.sh <stamp>   (il target CMake "deps" passa il file stamp da aggiornare a fine sync)
# Il venv con west (ext/venv) lo prepara py-deps.sh (target "py-deps", dipendenza di deps-sdk).
# Crea al bisogno il workspace west (.west/, topdir = radice del repo), poi "west update".
# Idempotente; i progetti sono clone shallow al commit pinnato.
source "$(dirname "$0")/common.sh"
stamp="${1:?uso: deps.sh <stamp>}"
venv="$ROOT/ext/venv"
west="$venv/bin/west"
[ -x "$west" ] || die "venv di ext/ assente: esegui 'make py-deps' (o 'make deps')"

cd "$ROOT"
if [ ! -d "$ROOT/.west" ]; then
    info "west init -l deps (topdir: $ROOT)"
    "$west" init -l deps
fi
info "west update (deps/west.yml)"
"$west" update --narrow || die "west update fallito"
# (il manifest non e' la radice di un repo git: west non sa calcolarne lo sha, si elencano solo i progetti)
"$west" list -f '{name:18} {sha:.12} {path}' $("$west" list -f '{name}' | grep -vx manifest)
touch "$stamp"
