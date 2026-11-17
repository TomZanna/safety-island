#!/usr/bin/env bash
# Setup una tantum (idempotente) di WSL per USB/IP, uuu e attach automatico. Usa sudo.
#   1. client usbip di Ubuntu (linux-tools-virtual, hwdata) in /usr/local/bin/usbip; libusb per uuu
#   2. regole udev per uuu e bcu (dispositivi NXP e FTDI della EVK accessibili senza root)
#   3. servizio systemd safety-island-usb (scripts/usb-attach.sh) abilitato e avviato
# La parte Windows (policy AutoBind di usbipd-win) NON e' automatica: alla fine stampa i comandi
# `usbipd policy add ...` per i VID:PID di board/usb-devices.conf. Serve PowerShell da amministratore,
# una volta per PC. Richiede systemd in WSL ([boot] systemd=true in /etc/wsl.conf).
# Dopo: `make usb-status` per lo stato, `journalctl -u safety-island-usb -f` per il log.
# Ambiente: UUU (uuu Linux, dal target CMake), USB_CONF.
source "$(dirname "$0")/common.sh"
conf="${USB_CONF:-$ROOT/board/usb-devices.conf}"

# 1. usbip
if ! command -v usbip >/dev/null; then
    info "installo il client usbip (linux-tools-virtual, hwdata)"
    sudo apt-get install -y linux-tools-virtual hwdata
    bin="$(ls /usr/lib/linux-tools/*/usbip 2>/dev/null | sort -V | tail -n1)"
    [ -x "$bin" ] || die "usbip non trovato in /usr/lib/linux-tools/*/"
    sudo update-alternatives --install /usr/local/bin/usbip usbip "$bin" 20
fi
info "usbip: $(command -v usbip) ($(usbip version 2>&1 | head -1))"

# uuu e bcu (release NXP) sono linkati dinamicamente a libusb e libftdi
for lib in libusb-1.0.so.0:libusb-1.0-0 libftdi1.so.2:libftdi1-2; do
    if ! ldconfig -p | grep -q "${lib%%:*}"; then
        info "installo ${lib#*:} (serve a uuu/bcu)"
        sudo apt-get install -y "${lib#*:}"
    fi
done

# picocom per le console (make uart*, task VS Code "UART: ...")
command -v picocom >/dev/null || { info "installo picocom"; sudo apt-get install -y picocom; }

# 2. udev per uuu
rules=/etc/udev/rules.d/70-uuu.rules
if [ -n "${UUU:-}" ] && [ -x "$UUU" ]; then
    tmp="$(mktemp)"
    "$UUU" -udev 2>/dev/null | grep '^SUBSYSTEM' > "$tmp" || true
    if [ -s "$tmp" ] && ! sudo cmp -s "$tmp" "$rules"; then
        info "regola udev uuu -> $rules"
        sudo install -m 0644 "$tmp" "$rules"
        sudo udevadm control --reload-rules && sudo udevadm trigger || true
    fi
    rm -f "$tmp"
else
    info "uuu non ancora scaricato: salto la regola udev (rilancia dopo 'make flash')"
fi

# Le regole di uuu usano TAG+="uaccess", che vale solo con una sessione "seat" (logind): in WSL non
# c'e'. Regola nostra al gruppo plugdev: dispositivi NXP (ROM SDP, fastboot) e FTDI quad della EVK (bcu).
nxp=/etc/udev/rules.d/71-safety-island-nxp.rules
line='SUBSYSTEM=="usb", ATTRS{idVendor}=="1fc9", MODE="0660", GROUP="plugdev"
SUBSYSTEM=="usb", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6011", MODE="0660", GROUP="plugdev"'
if [ "$(cat "$nxp" 2>/dev/null)" != "$line" ]; then
    info "regola udev NXP (gruppo plugdev) -> $nxp"
    echo "$line" | sudo tee "$nxp" >/dev/null
    sudo udevadm control --reload-rules && sudo udevadm trigger --subsystem-match=usb || true
fi
id -nG | grep -qw plugdev || { sudo usermod -aG plugdev "$USER"; info "aggiunto $USER a plugdev: riapri la shell"; }

# 3. servizio systemd
unit=/etc/systemd/system/safety-island-usb.service
tmp="$(mktemp)"
sed -e "s|@SCRIPTS@|$ROOT/scripts|" -e "s|@CONF@|$conf|" "$ROOT/board/safety-island-usb.service.in" > "$tmp"
if ! sudo cmp -s "$tmp" "$unit"; then
    info "servizio $unit"
    sudo install -m 0644 "$tmp" "$unit"
    sudo systemctl daemon-reload
fi
rm -f "$tmp"
sudo systemctl enable --now safety-island-usb.service >/dev/null
sudo systemctl restart safety-island-usb.service
info "servizio attivo: 'make usb-status' per lo stato, 'journalctl -u safety-island-usb -f' per il log"

cat <<EOF

Da fare UNA VOLTA su ogni PC Windows (PowerShell da amministratore), se non l'hai gia' fatto:
EOF
awk '!/^ *(#|$)/ {printf "  usbipd policy add --effect Allow --operation AutoBind --hardware-id %s\n", $1}' "$conf"
cat <<EOF
Le policy restano salvate in usbipd-win: i dispositivi vengono condivisi su qualunque porta USB.
Controllo: 'usbipd policy list' (Windows) e 'make usb-status' (WSL).
EOF
