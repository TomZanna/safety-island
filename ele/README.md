# Firmware ELE (EdgeLock Enclave)

Il container ELE **non è committato**: il target `ele` (incluso in `image`) lo scarica in `build/<preset>/ele/` e ne verifica lo sha256 con
`ele.sha256`.

- Pacchetto `firmware-ele-imx-2.0.6-c0b284c.bin` dal mirror Yocto NXP (ricetta
  `firmware-ele-imx_2.0.6.bb` di meta-imx `wrynose-6.18.20-2.0.0`, sha256 del pacchetto
  `ff42b838c42448616d3f6dc5a7f0d47c207bb0bae1ae00bea091103ed4012c28`): è la versione dichiarata dall'SM
  (`SM_ELE_VER 2.0.6`). Contiene ancora `mx943a0-ahab-container.img` (oltre ad A1/A2): usiamo quello A0.
- Prima si usava la 2.0.2 (`firmware-ele-imx-2.0.2-89161a8.bin`), insieme all'SM lf-6.12.20.
- La 1.3.0 (`firmware-ele-imx-1.3.0-17945fc.bin`) non contiene container per i.MX94: non va bene.
- È un binario proprietario sotto EULA NXP. Il suo `SCR.txt` cita "LA_OPT_NXP_Software_License -
  Additional distribution license granted - Section 2.3 applies": non ho letto la sezione 2.3, quindi non
  lo do per ridistribuibile. Per questo si scarica invece di committarlo.
- Da terminale l'installer NXP ti mostra la EULA e te la chiede. Senza terminale (CI, Docker) serve
  configurare con `-DACCEPT_ELE_EULA=ON`.
