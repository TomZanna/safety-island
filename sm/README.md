# System Manager (imx-sm)

Binari di riferimento dell'SM per la i.MX94 EVK, usati dal target `image`. Ogni build di `image` ne verifica
lo sha256 con `SHA256SUMS`.

| File | Config | Uso |
|---|---|---|
| `sm_mx94evk.bin` | `mx94evk` | default: periferiche divise tra AP, M33S, M7_0 e M7_1 |
| `sm_mx94alt.bin` | `mx94alt` | una sola LM possiede tutto: config per i demo del SDK MCUXpresso |

Provenienza in `BUILDINFO`: tag `lf-6.12.20-2.0.0` di [imx-sm](https://github.com/nxp-imx/imx-sm)
(commit `707569f4`), toolchain Arm 14.2.rel1 (quello dichiarato da NXP nel tag), debug monitor attivo (`M=1`).
Il tag dichiara `SM_ELE_VER 2.0.2`: va usato con il firmware ELE 2.0.2 (vedi `../ele/`).

## Verificare i binari

```bash
make sm-verify
```

Ricostruisce l'SM da sorgenti in `sm/Dockerfile` e confronta gli sha256 con quelli committati.
Serve solo Docker: toolchain e sorgenti li scarica il container (toolchain con sha256 verificato).
La build è **riproducibile al byte**, provata con due build `--no-cache`:

- toolchain pinnato;
- clone completo del tag, così `SM_COMMIT`/`SM_BUILD` sono quelli di imx-sm;
- percorsi fissi nel container (`/src/imx-sm`, `/opt/tools`);
- `SM_DATE`/`SM_TIME` congelati alla data del commit del tag (`date-wrapper.sh` + `SOURCE_DATE_EPOCH`).

Gli `.elf` per il debug restano in `build/<preset>/sm-docker/` (non committati: si rigenerano identici);
`make debug` li usa per i simboli.

## Cambiare tag o config

```bash
cmake --preset mx94evk -DSM_TAG=<tag>
make sm-update                       # ricostruisce e sovrascrive i binari in sm/
make run                             # provalo sulla board prima di committare
```

Se cambi tag, controlla `TC_VERSION` in `sm/makefiles/common.mak` del nuovo tag (e aggiorna `TC_VERSION`/`TC_SHA256`
nel Dockerfile) e `SM_ELE_VER` in `sm/makefiles/build_info.mak` (e aggiorna il firmware ELE).
L'SM di `master` non è compatibile con ELE 2.0.2 sulla A0: la board resettava in ciclo.
