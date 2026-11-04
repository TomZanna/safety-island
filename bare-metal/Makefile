# =============================================================================
#  imx943-flow bare metal safety firmware + A55 Linux transport
# =============================================================================
#
#  The rt-labs FSoE stack is built as a separate static library, from
#  ../bare-metal-code/fsoe, and linked at the end.
#
#  Targets:
#    make host       build and run the host validation test (default)
#    make m7         cross build the M7 safety firmware (needs arm-none-eabi)
#    make m33        cross build the M33 safety firmware
#    make a55        build the Linux kernel module (needs KDIR)
#    make abi        verify the shared mailbox layout on the host
#    make check      run the host validation matrix
#    make clean
# =============================================================================

CC ?= gcc
AR ?= ar
CROSS ?= arm-none-eabi-

FLOW_CORE_M7 := 0
FLOW_CORE_M33 := 1

BUILD := build
FSOE_DIR := ../bare-metal-code/fsoe

INCLUDES := -Iinclude -Iport -Ia55 -Iplatform/host \
            -I$(FSOE_DIR)/include -I$(FSOE_DIR)/src

WARNINGS := -Wall -Wextra -Werror -Wundef -Wno-unused-parameter
HOST_CFLAGS := -std=c11 -O2 -g -pthread -MMD -MP -D_GNU_SOURCE \
               -DFLOW_DEBUG=1 -DFLOW_HOST_TEST=1 \
               -DFLOW_MU_TIMEOUT_US=50000u \
               $(WARNINGS) $(INCLUDES)
HOST_LDFLAGS := -pthread

FSOE_SRC := \
   $(FSOE_DIR)/src/fsoemaster.c \
   $(FSOE_DIR)/src/fsoeslave.c \
   $(FSOE_DIR)/src/fsoeframe.c \
   $(FSOE_DIR)/src/fsoechannel.c \
   $(FSOE_DIR)/src/fsoewatchdog.c \
   $(FSOE_DIR)/src/fsoesracrc.c \
   $(FSOE_DIR)/src/fsoeutil.c \
   $(FSOE_DIR)/src/fsoeoptions.c \
   $(FSOE_DIR)/src/fsoeapp.c

HOST_SRC := \
   common/flow_mem.c \
   common/flow_crc.c \
   common/flow_shm.c \
   common/flow_mu.c \
   common/flow_safe_logic.c \
   safe/flow_safe.c \
   safe/fsoe_port.c \
   platform/host/flow_platform_host.c \
   a55/flow_a55.c \
   tests/host_field.c \
   tests/fsoe_app_host.c \
   tests/host_main.c

HOST_OBJ := $(patsubst %.c,$(BUILD)/host/%.o,$(HOST_SRC))
FSOE_HOST_OBJ := $(patsubst $(FSOE_DIR)/src/%.c,$(BUILD)/host/fsoe/%.o,$(FSOE_SRC))
HOST_LIB := $(BUILD)/libfsoe-host.a
HOST_BIN := $(BUILD)/flow-host

.PHONY: all host run check abi clean help m7 m33 a55

all: host

host: $(HOST_BIN)

run: host
	@./$(HOST_BIN)

$(HOST_BIN): $(HOST_OBJ) $(HOST_LIB)
	@echo "  LINK   $@"
	@$(CC) $(HOST_LDFLAGS) -o $@ $(HOST_OBJ) $(HOST_LIB)

$(HOST_LIB): $(FSOE_HOST_OBJ)
	@echo "  AR     $@"
	@$(AR) rcs $@ $^

$(BUILD)/host/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "  CC     $<"
	@$(CC) $(HOST_CFLAGS) -DFSOE_STATIC_DEFINE -c $< -o $@

$(BUILD)/host/fsoe/%.o: $(FSOE_DIR)/src/%.c
	@mkdir -p $(dir $@)
	@echo "  CC     [fsoe] $<"
	@$(CC) -std=c11 -O2 -g -Wall -Wextra -Wno-unused-parameter \
	   -DFSOE_STATIC_DEFINE $(INCLUDES) -c $< -o $@

abi: $(BUILD)/abi_layout
	@./tools/abi_check.sh

$(BUILD)/abi_layout:
	@mkdir -p $(BUILD)
	@$(CC) -std=c11 -Wall -Wextra -Iinclude tests/abi_layout.c -o $@

check: host abi
	@./check.sh

# --------------------------------------------------------------- target builds

TARGET_CFLAGS := -std=c11 -O2 -g -ffreestanding -fno-builtin \
   -fno-pic -fno-pie \
   -Wall -Wextra -Wno-unused-parameter -DFLOW_DEBUG=0 \
   -DFLOW_FREESTANDING=1 $(INCLUDES)
TARGET_SRC := \
   common/flow_mem.c \
   common/flow_crc.c \
   common/flow_shm.c \
   common/flow_mu.c \
   common/flow_safe_logic.c \
   safe/flow_safe.c \
   safe/fsoe_app.c \
   safe/fsoe_port.c \
   platform/target/flow_platform_target.c \
   platform/target/startup.c \
   app/main.c

define target_build
	@if ! command -v $(CROSS)gcc >/dev/null 2>&1; then \
	   echo "  SKIP   $(1): $(CROSS)gcc not found"; \
	else \
	   set -e; \
	   mkdir -p $(BUILD)/$(1); \
	   for src in $(TARGET_SRC); do \
	      obj=$(BUILD)/$(1)/$$(echo $$src | tr / _ | sed 's/\.c$$/.o/'); \
	      echo "  CC     [$(1)] $$src"; \
	      $(CROSS)gcc $(TARGET_CFLAGS) -DFLOW_CORE_ID=$(2) $(3) -c $$src -o $$obj; \
	   done; \
	   for src in $(FSOE_SRC); do \
	      obj=$(BUILD)/$(1)/fsoe_$$(basename $$src .c).o; \
	      echo "  CC     [$(1)] $$src"; \
	      $(CROSS)gcc -std=c11 -O2 -ffreestanding -fno-builtin \
	         -fno-pic -fno-pie $(INCLUDES) -DFSOE_STATIC_DEFINE -c $$src -o $$obj; \
	   done; \
	   $(AR) rcs $(BUILD)/libfsoe-$(1).a $(BUILD)/$(1)/fsoe_*.o; \
	   echo "  LINK   $(BUILD)/$(1)/flow-$(1).elf"; \
	   $(CROSS)gcc -nostdlib -no-pie -Wl,-T,platform/target/linker.ld \
	      -o $(BUILD)/$(1)/flow-$(1).elf $(BUILD)/$(1)/*.o \
	      $(BUILD)/libfsoe-$(1).a -lgcc; \
	fi
endef

m7:
	$(call target_build,m7,FLOW_CORE_M7,-mcpu=cortex-m7 -mthumb -mfloat-abi=soft)

m33:
	$(call target_build,m33,FLOW_CORE_M33,-mcpu=cortex-m33 -mthumb -mfloat-abi=soft)

a55:
	@if [ -z "$(KDIR)" ]; then \
	   echo "  SKIP   a55: KDIR not set"; \
	else \
	   $(MAKE) -C $(KDIR) M=$(CURDIR)/a55 modules; \
	fi

clean:
	@rm -rf $(BUILD)

help:
	@sed -n '2,25p' Makefile

-include $(HOST_OBJ:.o=.d)
-include $(FSOE_HOST_OBJ:.o=.d)
