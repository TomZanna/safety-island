# safety-island: flow di build e run per i.MX94 EVK (System Manager + core secondari)
include config.mk
export

.DEFAULT_GOAL := help
.PHONY: help ele docs docs-live sm sm-promote image flash run reset uart debug debug-flash inspect clean distclean \
        docker-env docker-sm docker-image

help:
	@echo "Uso: make <target> [VAR=valore]   (variabili in config.mk)"
	@echo ""
	@echo "  ele          scarica il firmware ELE NXP (EULA: l'accettazione e' tua) -> build/ele/"
	@echo "  sm           compila l'SM dal tag NXP ($(SM_TAG)) -> build/sm/"
	@echo "  sm-promote   copia l'SM compilato in prebuilt/ e aggiorna SHA256SUMS"
	@echo "  image        crea build/flash.bin (ELE + SM [+ M33S/M70/M71])"
	@echo "  flash        carica build/flash.bin IN RAM via uuu (reset automatico se serve)"
	@echo "  run          image + flash"
	@echo "  reset        riporta la board in serial download (comando 'reset' del monitor)"
	@echo "  uart         console del debug monitor SM ($(SM_UART))"
	@echo "  debug        J-Link GDB server + gdb ($(JLINK_CORE))"
	@echo "  debug-flash  flash poi debug"
	@echo "  inspect      elenca i container AHAB di build/flash.bin"
	@echo "  docs         compila docs/main.typ -> build/docs/safety-island.pdf (typst)"
	@echo "  docs-live    typst watch: rigenera il PDF a ogni salvataggio"
	@echo "  docker-env   costruisce il container di build (toolchain, git, gcc)"
	@echo "  docker-sm / docker-image   come sm / image ma dentro il container"
	@echo "  clean / distclean          pulizia (distclean toglie anche i sorgenti scaricati)"

ele:
	./scripts/fetch-ele.sh

docs:
	./scripts/docs.sh build

docs-live:
	./scripts/docs.sh live

sm:
	./scripts/build-sm.sh

sm-promote: sm
	cp build/sm/sm_$(SM_CONFIG).bin prebuilt/sm/
	cd prebuilt && sha256sum sm/*.bin ele/*.img > SHA256SUMS
	@echo "prebuilt aggiornato: ricorda di annotare tag/config in prebuilt/README.md"

image:
	./scripts/build-image.sh

flash:
	./scripts/flash.sh

run: image flash

reset:
	./scripts/reset.sh

uart:
	./scripts/uart.sh

debug:
	./scripts/debug.sh

debug-flash: flash debug

inspect:
	python3 -I scripts/ahab_walk.py build/flash.bin

clean:
	rm -f build/flash.bin build/*.log
	rm -rf build/sm

distclean:
	rm -rf build

# --- Docker: scarica il necessario e builda, senza toccare l'host -------------
# TOOLCHAIN_DIR: directory del toolchain ARM da copiare nell immagine (default: quello installato qui)
TOOLCHAIN_DIR ?= /arm-gnu-toolchain-15.3.rel2-x86_64-arm-none-eabi
docker-env:
	docker build --build-context toolchain=$(TOOLCHAIN_DIR) -t safety-island-build docker

docker-sm docker-image: docker-env
	./docker/run.sh $(patsubst docker-%,%,$@)
