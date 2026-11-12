#!/usr/bin/env bash
# Configura il preset "docker" ed esegue un target dentro il container, con il repo montato
# in /work e gli stessi uid/gid dell'host (i file in build/docker/ restano tuoi).
#   docker/run.sh <target> [-DVAR=valore ...]
set -euo pipefail
cd "$(dirname "$0")/.."
target="${1:?uso: docker/run.sh <target> [-DVAR=valore ...]}"; shift
docker build -q -t safety-island-build docker >/dev/null
exec docker run --rm -u "$(id -u):$(id -g)" -e HOME=/tmp -v "$PWD:/work" safety-island-build \
    sh -c 'cmake --preset docker "$@" >/dev/null && cmake --build --preset docker --target "$0"' "$target" "$@"
