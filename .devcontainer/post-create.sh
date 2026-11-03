#!/usr/bin/env bash
# Setup del container di sviluppo (devcontainer.json -> postCreateCommand).
#
# Sistema i permessi di globalStorage di Cline, che può esistere come root perché montato da un volume.
set -euo pipefail

dir="$HOME/.vscode-server/data/User/globalStorage/saoudrizwan.claude-dev"
if [ -d "$dir" ]; then
  sudo chown -R "$(id -u):$(id -g)" "$dir" || true
fi
