#!/usr/bin/env bash
# sm-docker.sh verify|update
#   verify: ricostruisce l'SM nel Docker dedicato (sm/Dockerfile) e confronta con i binari in sm/
#   update: come verify, ma poi copia i nuovi binari in sm/ (per cambiare SM_TAG o config)
source "$(dirname "$0")/common.sh"
mode="${1:?uso: sm-docker.sh verify|update}"
: "${SM_TAG:?passato dal target CMake sm-verify/sm-update}" "${SM_REPO:?}" "${SM_CONFIGS:?}"
out="$BUILD/sm-docker"
command -v docker >/dev/null || die "docker non trovato"
docker info >/dev/null 2>&1 || die "docker non raggiungibile (permission denied? prova 'newgrp docker')"

rm -rf "$out"
info "build SM nel container (tag $SM_TAG, config: $SM_CONFIGS)"
docker build -f "$ROOT/sm/Dockerfile" \
    --build-arg SM_TAG="$SM_TAG" --build-arg SM_REPO="$SM_REPO" --build-arg SM_CONFIGS="$SM_CONFIGS" \
    --output "type=local,dest=$out" "$ROOT/sm" >"$BUILD/sm-docker.log" 2>&1 \
    || { tail -30 "$BUILD/sm-docker.log"; die "build Docker fallita (log: $BUILD/sm-docker.log)"; }
info "artefatti in $out (anche gli .elf per il debug)"
cat "$out/BUILDINFO"

case "$mode" in
verify)
    if diff -u "$ROOT/sm/SHA256SUMS" "$out/SHA256SUMS"; then
        info "VERIFICATO: i binari in sm/ sono identici alla build da sorgenti"
    else
        die "i binari in sm/ NON corrispondono alla build da sorgenti (vedi diff sopra)"
    fi ;;
update)
    cp "$out"/sm_*.bin "$out/SHA256SUMS" "$out/BUILDINFO" "$ROOT/sm/"
    info "sm/ aggiornato: prova l'immagine sulla board prima di committare" ;;
*) die "modo sconosciuto: $mode" ;;
esac
