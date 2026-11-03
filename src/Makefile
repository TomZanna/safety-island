# =============================================================================
#  imx943-flow - 1oo2 dual core FSoE prototype (PC emulation)
# =============================================================================
#
#  Build strategy (as required by pseudo_code.md, section "compile time"):
#
#   1. every source file of the rt-labs FSoE stack is compiled on its own and
#      archived into build/libfsoe.a  -> the library is a separate artefact
#      that is *linked* at the very end
#   2. every component of this prototype (SHMEM mailbox, messaging unit, safe
#      logic, safe tick, M7, M33, A55, glue, runtime) is compiled on its own
#   3. the executable is produced by linking all the objects with libfsoe.a
#
#  Useful targets:
#     make                  build everything
#     make run              build and run (default 2 s demonstration)
#     make demo             run with per cycle tracing
#     make check            run the fault injection matrix and report
#     make clean
#
#  Options:
#     CONNECTIONS=2         two independent FSoE connections
#     LOG=1                 enable the internal logging of the FSoE stack
#     SANITIZE=1            build with ThreadSanitizer
#     DEBUG=1               -O0
# =============================================================================

CC ?= gcc
AR ?= ar

.DEFAULT_GOAL := all

ROOT := ..
FSOE_DIR := $(ROOT)/example-code/fsoe
BUILD := build
BIN := $(BUILD)/imx943-flow
LIBFSOE := $(BUILD)/libfsoe.a

CONNECTIONS ?= 1

CFLAGS := -std=gnu11 -O2 -g -pthread -Wall -Wextra -Werror \
          -Wno-unused-parameter -MMD -MP
CFLAGS += -DFSOE_STATIC_DEFINE -DFLOW_NUM_CONNECTIONS=$(CONNECTIONS)u
CPPFLAGS += -Iinclude \
            -I$(FSOE_DIR)/include \
            -I$(FSOE_DIR)/src \
            -I$(FSOE_DIR)/src/port/linux
LDFLAGS := -pthread

ifeq ($(LOG),1)
CFLAGS += -DFSOE_LOG_ENABLE -DFSOE_LOG_LEVEL=0
endif

ifeq ($(SANITIZE),1)
CFLAGS += -fsanitize=thread
LDFLAGS += -fsanitize=thread
endif

ifeq ($(SANITIZE),addr)
CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS += -fsanitize=address,undefined
endif

ifeq ($(DEBUG),1)
CFLAGS += -O0
endif

# ---------------------------------------------------------------- FSoE stack

FSOE_SRC := \
	$(FSOE_DIR)/src/fsoemaster.c \
	$(FSOE_DIR)/src/fsoeslave.c \
	$(FSOE_DIR)/src/fsoeframe.c \
	$(FSOE_DIR)/src/fsoechannel.c \
	$(FSOE_DIR)/src/fsoewatchdog.c \
	$(FSOE_DIR)/src/fsoesracrc.c \
	$(FSOE_DIR)/src/fsoeutil.c \
	$(FSOE_DIR)/src/fsoeoptions.c \
	$(FSOE_DIR)/src/fsoeapp.c \
	$(FSOE_DIR)/src/port/linux/fsoeport.c

FSOE_OBJ := $(patsubst $(FSOE_DIR)/src/%.c,$(BUILD)/fsoe/%.o,$(FSOE_SRC))

# --------------------------------------------------------------- components

APP_SRC := \
	util/flow_log.c \
	util/flow_time.c \
	shmem/shmem_mailbox.c \
	mu/mu_channel.c \
	safe/safe_tick.c \
	safe/safe_logic.c \
	safe/safe_core.c \
	cores/m7_core.c \
	cores/m33_core.c \
	a55/a55_core.c \
	glue/fsoe_app_glue.c \
	app/fault_inject.c \
	app/flow_setup.c \
	app/runtime.c \
	app/main.c

APP_OBJ := $(patsubst %.c,$(BUILD)/obj/%.o,$(APP_SRC))

# ------------------------------------------------------------------- targets

# Configuration stamp: switching CONNECTIONS / LOG / SANITIZE invalidates all
# the objects (they are compiled with different -D flags).
CONFIG_STAMP := $(BUILD)/.config-$(CONNECTIONS)-$(LOG)-$(SANITIZE)

$(CONFIG_STAMP):
	@mkdir -p $(BUILD)
	@rm -f $(BUILD)/.config-*
	@touch $@

.PHONY: all run demo check clean help

all: $(BIN)

$(BIN): $(APP_OBJ) $(LIBFSOE)
	@echo "  LINK   $@"
	@$(CC) $(LDFLAGS) -o $@ $(APP_OBJ) $(LIBFSOE)

$(LIBFSOE): $(FSOE_OBJ)
	@echo "  AR     $@"
	@$(AR) rcs $@ $^

$(BUILD)/fsoe/%.o: $(FSOE_DIR)/src/%.c $(CONFIG_STAMP)
	@mkdir -p $(dir $@)
	@echo "  CC     [fsoe] $<"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD)/obj/%.o: %.c $(CONFIG_STAMP)
	@mkdir -p $(dir $@)
	@echo "  CC     $<"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

run: $(BIN)
	@echo
	@./$(BIN)

demo: $(BIN)
	@echo
	@./$(BIN) --duration 2500 --verbose

check: $(BIN)
	@./check.sh

clean:
	@rm -rf $(BUILD)

help:
	@sed -n '2,31p' Makefile

-include $(FSOE_OBJ:.o=.d)
-include $(APP_OBJ:.o=.d)
