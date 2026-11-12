#!/usr/bin/env bash
# Compila l'SM dal tag NXP -> build/sm/sm_<config>.bin (+ .elf per il debug)
source "$(dirname "$0")/common.sh"

src="$(fetch_tag imx-sm "$SM_REPO" "$SM_TAG" "$SM_SRC")"
export TOOLS; TOOLS="$(find_tools)"
info "build SM config=$SM_CONFIG tag=$SM_TAG TOOLS=$TOOLS"
make -C "$src" config="$SM_CONFIG" all M=1 -j"$(nproc)" >"$BUILD/sm-build.log" 2>&1 \
    || { tail -30 "$BUILD/sm-build.log"; die "build SM fallita (log: build/sm-build.log)"; }
mkdir -p "$BUILD/sm"
cp "$src/build/$SM_CONFIG/m33_image.bin" "$BUILD/sm/sm_$SM_CONFIG.bin"
cp "$src/build/$SM_CONFIG/m33_image.elf" "$BUILD/sm/sm_$SM_CONFIG.elf"
info "ok: build/sm/sm_$SM_CONFIG.bin ($(stat -c %s "$BUILD/sm/sm_$SM_CONFIG.bin") byte)"
