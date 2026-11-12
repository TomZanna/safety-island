#!/usr/bin/env bash
# Scarica il firmware ELE NXP e ne estrae il container i.MX94 A0 in build/ele/.
# Il pacchetto e' sotto EULA NXP: l'accettazione e' tua, mai implicita.
#   - da terminale: l'installer NXP mostra la EULA e te la chiede
#   - non interattivo (CI, docker): ACCEPT_ELE_EULA=1 significa "ho letto e accetto la EULA NXP"
source "$(dirname "$0")/common.sh"

name="$(basename "$ELE_URL")"
work="$DEPS/ele"
mkdir -p "$work" "$BUILD/ele"
if [ ! -f "$work/$name" ]; then
    info "scarico $ELE_URL"
    curl -fsSL -o "$work/$name.part" "$ELE_URL" || die "download fallito"
    mv "$work/$name.part" "$work/$name"
fi

if [ "${ACCEPT_ELE_EULA:-}" = 1 ]; then
    flag=(--auto-accept)
elif [ -t 0 ]; then
    flag=()
else
    die "terminale non interattivo: per accettare la EULA NXP imposta ACCEPT_ELE_EULA=1"
fi

pkg="${name%.bin}"
rm -rf "$work/$pkg"
# l'installer estrae nella cwd: directory dedicata, script non eseguito da dentro i sorgenti
( cd "$work" && sh "./$name" "${flag[@]}" ) || die "estrazione fallita (EULA non accettata?)"

src="$work/$pkg/mx943a0-ahab-container.img"
[ -f "$src" ] || die "$pkg non contiene mx943a0-ahab-container.img (versione senza i.MX94?)"
cp "$src" "$BUILD/ele/"
(cd "$BUILD/ele" && sha256sum -c --quiet "$ROOT/prebuilt/ele.sha256") \
    || die "sha256 diverso da prebuilt/ele.sha256: versione ELE cambiata? Verifica la compatibilita' con SM_TAG e aggiorna il file"
info "ok: build/ele/mx943a0-ahab-container.img (sha256 verificato)"
