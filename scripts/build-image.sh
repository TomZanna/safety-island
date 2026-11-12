#!/usr/bin/env bash
# Crea build/flash.bin: container ELE (AHAB) + SM (+ M33S/M7 opzionali).
source "$(dirname "$0")/common.sh"

# SM: preferisce quello compilato in build/, altrimenti il prebuilt verificato
sm="$BUILD/sm/sm_$SM_CONFIG.bin"
if [ ! -f "$sm" ]; then
    sm="$ROOT/prebuilt/sm/sm_$SM_CONFIG.bin"
    (cd "$ROOT/prebuilt" && grep " sm/sm_$SM_CONFIG.bin\$" SHA256SUMS | sha256sum -c --quiet -) \
        || die "checksum del prebuilt SM non valido"
fi
[ "$REV" = A0 ] || die "il container ELE e' previsto solo per REV=A0"
ele="${ELE_IMG:-$BUILD/ele/mx943a0-ahab-container.img}"
[ -f "$ele" ] || die "container ELE mancante ($ele): esegui 'make ele' (scarica il firmware NXP) o imposta ELE_IMG="
if [ -z "${ELE_IMG:-}" ]; then
    (cd "$BUILD/ele" && sha256sum -c --quiet "$ROOT/prebuilt/ele.sha256") || die "checksum ELE non valido"
fi

mkimage="$(get_mkimage)"
out="$BUILD/flash.bin"
args=(-soc IMX9 -cntr_version 2 -append "$ele" -c -msel "$MSEL" -m33 "$sm" 0 0x1FFC0000)
[ -z "$M33S" ] || args+=(-m33 "$(realpath "$M33S")" 1 0x1FFC0000 0x309C0000)
[ -z "$M70"  ] || args+=(-m7  "$(realpath "$M70")"  0 0x0 0x303C0000)
[ -z "$M71"  ] || args+=(-m7  "$(realpath "$M71")"  1 0x0 0x302C0000)

info "SM: ${sm#"$ROOT"/}  (config=$SM_CONFIG, msel=$MSEL)"
"$mkimage" "${args[@]}" -out "$out" >"$BUILD/mkimage.log" 2>&1 \
    || { cat "$BUILD/mkimage.log"; die "mkimage fallito"; }
info "ok: build/flash.bin ($(stat -c %s "$out") byte)"
python3 -I "$ROOT/scripts/ahab_walk.py" "$out"
