# Firmware ELE (EdgeLock Enclave)

Il container ELE **non è committato**: il target `ele` (incluso in `image`) lo scarica in `build/<preset>/ele/` e ne verifica lo sha256 con
`ele.sha256`.

- Pacchetto `firmware-ele-imx-2.0.2-89161a8.bin` dal mirror Yocto NXP: è la versione dichiarata dall'SM
  (`SM_ELE_VER 2.0.2`). Contiene `mx943a0-ahab-container.img`, solo per silicio **A0**.
- La 1.3.0 (`firmware-ele-imx-1.3.0-17945fc.bin`) non contiene container per i.MX94: non va bene.
- È un binario proprietario sotto EULA NXP. Il suo `SCR.txt` cita "LA_OPT_NXP_Software_License -
  Additional distribution license granted - Section 2.3 applies": non ho letto la sezione 2.3, quindi non
  lo do per ridistribuibile. Per questo si scarica invece di committarlo.
- Da terminale l'installer NXP ti mostra la EULA e te la chiede. Senza terminale (CI, Docker) serve
  configurare con `-DACCEPT_ELE_EULA=ON`.
