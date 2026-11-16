#!/usr/bin/env bash
# Sincronizza le dipendenze west di deps/west.yml (SDK MCUXpresso minimo) in ext/sdk/.
# Uso: deps.sh <stamp>   (il target CMake "deps" passa il file stamp da aggiornare a fine sync)
# Crea al bisogno un venv con west (ext/venv), il workspace west (.west/, topdir = radice del repo) e
# poi "west update". Idempotente; i progetti sono clone shallow al commit pinnato.
source "$(dirname "$0")/common.sh"
stamp="${1:?uso: deps.sh <stamp>}"
WEST_VERSION="${WEST_VERSION:-1.5.0}"
PYLINK_VERSION="${PYLINK_VERSION:-2.1.0}"   # pylink-square: RTT API della DLL J-Link (make rtt)
venv="$ROOT/ext/venv"
west="$venv/bin/west"
pkgs=("west==$WEST_VERSION" "pylink-square==$PYLINK_VERSION")

venv_ok() {
    "$west" --version 2>/dev/null | grep -q "v$WEST_VERSION" &&
        "$venv/bin/python" -c "import pylink, sys; sys.exit(pylink.__version__ != '$PYLINK_VERSION')" 2>/dev/null
}
if [ ! -x "$west" ] || ! venv_ok; then
    info "venv con ${pkgs[*]} in $venv"
    if command -v uv >/dev/null; then
        uv venv -q --allow-existing "$venv"
        uv pip install -q --python "$venv/bin/python" "${pkgs[@]}"
    else
        python3 -m venv "$venv"
        "$venv/bin/python" -m pip install -q "${pkgs[@]}"
    fi
fi

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
