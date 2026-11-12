#!/usr/bin/env bash
# Esegue "make <target>" dentro il container, con il repo montato in /work
# e gli stessi uid/gid dell'host (i file in build/ restano tuoi).
set -euo pipefail
cd "$(dirname "$0")/.."
target="${1:?uso: docker/run.sh <sm|image> [VAR=valore ...]}"; shift
exec docker run --rm -u "$(id -u):$(id -g)" -e HOME=/tmp -e ACCEPT_ELE_EULA -v "$PWD:/work" \
    safety-island-build make "$target" "$@"
