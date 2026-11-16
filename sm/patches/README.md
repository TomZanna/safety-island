# Patch locali all'SM NXP

Il Docker dell'SM (`../Dockerfile`) applica qui ogni `*.patch` (in ordine, `git apply` sulla radice di
imx-sm al tag `SM_TAG`) e rigenera con `configs/configtool.pl` le config il cui `.cfg` cambia. Le patch
finiscono in `BUILDINFO` con il loro sha256.

- `0001-mx94evk-rtt-shared-ocram.patch`: aggiunge `OCRAM DATA, begin=0x0204D0000, size=16K` alle LM
  `M33S`, `M7` e `M71` (l'area resta anche dell'AP). Ci sta il control block RTT condiviso, un canale per
  core (`fw/common/rtt_shared.h`). `sm_mx94alt.bin` non cambia.

Esempio (usato in passato per avere la console di M7_0 su USB, vedi README "Console seriali"):
spostare `LPUART1`, `PIN_UART1_RXD`, `PIN_UART1_TXD` dalla LM `AP` alla LM `M7` in `configs/mx94evk.cfg`
(e togliere `PERLPI_LPUART1` dall'agente `AP-S`). Poi `make sm-update` e prova sulla board.
