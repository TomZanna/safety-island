# Scorciatoia per CMake: "make <target> [PRESET=mx94alt]" equivale a
#   cmake --preset <PRESET> && cmake --build --preset <PRESET> --target <target>
# Le opzioni si cambiano nella cache CMake (cmake --preset mx94evk -DM70_BIN=...) o in
# CMakeUserPresets.json (personale, non committato).
PRESET ?= mx94evk
CACHE := build/$(PRESET)/CMakeCache.txt

.DEFAULT_GOAL := help
.PHONY: help configure clean distclean clean-all FORCE

$(CACHE):
	cmake --preset $(PRESET)

configure:
	cmake --preset $(PRESET)

help: $(CACHE)
	@cmake --build --preset $(PRESET) --target si-help

# Pulizia, tre profondita' (non passa da CMake: niente configure).
#   clean      build del preset corrente; i download in ext/ restano
#   distclean  tutti i build; i download in ext/ restano
#   clean-all  anche i download (toolchain, SDK, venv) e .west: il build successivo riparte da zero
clean:
	@test -n "$(PRESET)" || { echo "PRESET vuoto: usa 'make distclean'" >&2; exit 1; }
	rm -rf "build/$(PRESET)"

distclean:
	rm -rf build

clean-all: distclean
	rm -rf ext .west

Makefile: ;
FORCE: ;

%: $(CACHE) FORCE
	cmake --build --preset $(PRESET) --target $@
