#!/usr/bin/env bash
# Prepara il venv di ext/ con west e pylink (servono a "make deps" e "make rtt").
# Idempotente: se le versioni sono gia' quelle attese non fa nulla.
# Usa uv se c'e', altrimenti python3 -m venv (su Ubuntu/WSL serve il pacchetto python3-venv).
# Fail-fast: se nessuno dei due e' utilizzabile dice cosa installare, non lascia un errore di pip.
source "$(dirname "$0")/common.sh"
WEST_VERSION="${WEST_VERSION:-1.5.0}"
PYLINK_VERSION="${PYLINK_VERSION:-2.1.0}"   # pylink-square: RTT API della DLL J-Link (make rtt)
venv="$ROOT/ext/venv"
west="$venv/bin/west"
pkgs=("west==$WEST_VERSION" "pylink-square==$PYLINK_VERSION")

venv_ok() {
    "$west" --version 2>/dev/null | grep -q "v$WEST_VERSION" &&
        "$venv/bin/python" -c "import pylink, sys; sys.exit(pylink.__version__ != '$PYLINK_VERSION')" 2>/dev/null
}
if [ -x "$west" ] && venv_ok; then
    exit 0
fi

info "venv con ${pkgs[*]} in $venv"
if command -v uv >/dev/null; then
    uv venv -q --allow-existing "$venv" || die "uv venv fallito"
    uv pip install -q --python "$venv/bin/python" "${pkgs[@]}" || die "uv pip install fallito (rete?)"
else
    if ! python3 -m venv --clear "$venv" 2>/dev/null; then
        die "python3 -m venv non disponibile: installa 'uv' (consigliato) oppure python3-venv (sudo apt install python3-venv)"
    fi
    "$venv/bin/python" -m pip install -q "${pkgs[@]}" || die "pip install fallito (rete?)"
fi
"$venv/bin/python" -c "import pylink" || die "pylink non importabile nel venv"
