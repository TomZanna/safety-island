# Configurazione del flow. Sovrascrivibile da riga di comando:
#   make image SM_CONFIG=mx94alt M70=blink.bin
# (niente commenti sulla stessa riga: Make conserverebbe gli spazi nel valore)

# --- Sorgenti NXP, pinnati ai tag verificati ---------------------------------
# SM_TAG deve combaciare con il firmware ELE (2.0.2 -> release LF 6.12.20)
SM_TAG       ?= lf-6.12.20-2.0.0
# MKIMAGE_TAG: stesso commit (1b57785) usato nei test
MKIMAGE_TAG  ?= lf-6.18.20-2.0.0
SM_REPO      ?= https://github.com/nxp-imx/imx-sm.git
MKIMAGE_REPO ?= https://github.com/nxp-imx/imx-mkimage.git
# Se valorizzati si usa un repo locale (git archive) invece di clonare
SM_SRC       ?=
MKIMAGE_SRC  ?=
# Se valorizzato si usa questo mkimage_imx8 invece di scaricarlo e compilarlo
MKIMAGE      ?=

# --- Firmware ELE (proprietario NXP, NON committato: lo scarica 'make ele') ----
# 2.0.2 e' la versione della release LF 6.12.20 e contiene mx943a0-ahab-container.img
# (la 1.3.0 non ha container per i.MX94). Percorso da yocto: firmware-ele-imx
ELE_URL      ?= https://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-ele-imx-2.0.2-89161a8.bin
# Se valorizzato si usa questo container invece di build/ele/
ELE_IMG      ?=

# --- Immagine ----------------------------------------------------------------
# SM_CONFIG: mx94evk (Linux + M7) oppure mx94alt (config per i demo del SDK)
SM_CONFIG    ?= mx94evk
REV          ?= A0
# MSEL viene passato all'SM e sceglie il blocco MODE msel=N del suo cfg
MSEL         ?= 0
# Binari opzionali per gli altri core (percorso a un .bin)
M33S         ?=
M70          ?=
M71          ?=

# --- Board -------------------------------------------------------------------
# UART del debug monitor dell'SM
SM_UART      ?= /dev/ttyUSB3
# JLINK_CORE: M33_0 | M33_1 | M7_0 | M7_1 (device MIMX94398_<core>)
JLINK_CORE   ?= M33_0
JLINK_PORT   ?= 2331
# ELF per gdb (per l'SM: build/sm/sm_<config>.elf dopo 'make sm')
ELF          ?=

# --- Toolchain ---------------------------------------------------------------
# TOOLS = directory che contiene arm-gnu-toolchain-<ver>-x86_64-arm-none-eabi
# (vuoto = cerca in /opt/tools e in /)
TOOLS        ?=

# --- Docs (typst) ------------------------------------------------------------
# Se 'typst' e' nel PATH si usa quello, altrimenti si scarica questa versione
TYPST_VERSION ?= 0.13.1
TYPST_SHA256  ?= 7d214bfeffc2e585dc422d1a09d2b144969421281e8c7f5d784b65fc69b5673f
# Visualizzatore per docs-live (es. wslview); vuoto = nessuna apertura automatica
DOCS_VIEWER   ?=
