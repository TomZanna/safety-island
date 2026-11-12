# Scorciatoia per CMake: "make <target> [PRESET=mx94alt]" equivale a
#   cmake --preset <PRESET> && cmake --build --preset <PRESET> --target <target>
# Le opzioni si cambiano nella cache CMake (cmake --preset mx94evk -DM70_BIN=...) o in
# CMakeUserPresets.json (personale, non committato).
PRESET ?= mx94evk
CACHE := build/$(PRESET)/CMakeCache.txt

.DEFAULT_GOAL := help
.PHONY: help configure distclean FORCE

$(CACHE):
	cmake --preset $(PRESET)

configure:
	cmake --preset $(PRESET)

help: $(CACHE)
	@cmake --build --preset $(PRESET) --target si-help

distclean:
	rm -rf build

Makefile: ;
FORCE: ;

%: $(CACHE) FORCE
	cmake --build --preset $(PRESET) --target $@
