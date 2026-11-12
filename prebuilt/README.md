# Binari committati

Servono per costruire l'immagine senza scaricare nulla (`make image`). Le somme di controllo
in `SHA256SUMS` vengono verificate a ogni `make image`.

| File | Origine |
|---|---|
| `sm/sm_mx94evk.bin` | `imx-sm` tag `lf-6.12.20-2.0.0`, `make config=mx94evk all M=1`, toolchain arm-gnu 15.3.rel2 (176860 byte) |
| `sm/sm_mx94alt.bin` | stesso tag, `config=mx94alt` (171100 byte) |

Il container **ELE non è committato**: lo scarica `make ele` in `build/ele/` e se ne verifica lo sha256
con `ele.sha256`.

## Firmware ELE

- Pacchetto `firmware-ele-imx-2.0.2-89161a8.bin` (stesso percorso del mirror Yocto NXP), versione della
  release LF 6.12.20-2.0.0. Contiene `mx943a0-ahab-container.img` (solo silicio **A0**).
- La **1.3.0** (`firmware-ele-imx-1.3.0-17945fc.bin`) non contiene container per i.MX94
  (solo 8ULP, 91, 93, 95): non va bene per questa board.
- È un binario proprietario sotto EULA NXP. Il suo `SCR.txt` cita "LA_OPT_NXP_Software_License -
  Additional distribution license granted - Section 2.3 applies": non ho letto la sezione 2.3, quindi
  non do per scontato che si possa ridistribuire. Per questo lo si scarica invece di committarlo.
- `make ele` mostra la EULA e te la chiede; senza terminale (CI, docker) serve `ACCEPT_ELE_EULA=1`.
- Sul mirror c'è anche la 1.3.0 (`firmware-ele-imx-1.3.0-17945fc.bin`), senza i.MX94.

## Compatibilità

SM e firmware ELE vanno presi dalla **stessa release**. L'SM di `master` si è dimostrato
incompatibile con ELE 2.0.2 sulla A0 (la board resettava in ciclo). Se cambi `SM_TAG`,
cambia anche il container ELE e ripeti il test.

## Aggiornare un binario

`make sm SM_CONFIG=<cfg>` poi `make sm-promote SM_CONFIG=<cfg>`; aggiorna qui sopra tag e toolchain.
Le ricompilazioni **non sono identiche al byte** (stringhe con percorsi dei sorgenti e data di build
nel binario), ma funzionalmente equivalenti.
