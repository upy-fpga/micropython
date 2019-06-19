include ../../py/mkenv.mk

GIT_VERSION := $(shell git describe --tags)
CROSS := 1

# qstr definitions (must come before including py.mk)
QSTR_DEFS = include/qstrdefsport.h

# include py core make definitions
include $(TOP)/py/py.mk

LDSCRIPT := ld/fomu-spi.ld

# There is no 64-bit gcc on Raspberry Pi, so use the 32-bit version
ifneq (,$(wildcard /usr/bin/riscv32-unknown-elf-gcc))
TRGT       ?= riscv32-unknown-elf-
else
TRGT       ?= riscv64-unknown-elf-
endif
CC         := $(TRGT)gcc
CXX        := $(TRGT)g++
OBJCOPY    := $(TRGT)objcopy

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -Iinclude

DFU := $(TOP)/tools/dfu.py
PYDFU := $(TOP)/tools/pydfu.py
CFLAGS := $(INC) \
			-Wall -Wextra \
			-ggdb -g \
			-fno-common \
			-fomit-frame-pointer \
			-flto \
			-fdata-sections -ffunction-sections \
			-march=rv32i \
			-DGIT_VERSION=u\"$(GIT_VERSION)\" -std=gnu11 \
			-D__vexriscv__ -march=rv32i  -mabi=ilp32 \
 			$(COPT)
LDFLAGS := $(CFLAGS) \
			-nostartfiles \
			-Wl,--gc-sections \
			-Wl,--no-warn-mismatch \
			-Wl,--script=$(LDSCRIPT) \
			-Wl,--build-id=none

# Tune for Debugging or Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -O0
else
CFLAGS += -Os -DNDEBUG
CFLAGS += -fdata-sections -ffunction-sections
endif

LIBS =

SRC_C = \
	lib/libc/string0.c \
	lib/mp-readline/readline.c \
	lib/utils/printf.c \
	lib/utils/pyexec.c \
	src/main.c \
	src/py.c \
	src/rgb.c \
	src/rgb-py.c \
	src/spi.c \
	src/spi-py.c \
	src/stdout-helpers.c \
	src/time.c \
	src/uart-core.c \
	src/usb-epfifo.c \
	src/usb-dev.c \
	src/usb-desc.c \

SRC_C_PY = \
	$(BUILD)/_frozen_mpy.c \

SRC_S = \
	src/crt0-vexriscv.S \

SRC_QSTR += $(SRC_C)

VPATH += src include

OBJ = $(PY_CORE_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o) $(SRC_S:.S=.o) $(SRC_C_PY:.c=.o))

ifeq ($(CROSS), 1)
all: $(BUILD)/firmware.dfu
else
all: $(BUILD)/firmware.elf
endif

$(BUILD)/_frozen_mpy.c: frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(ECHO) "MISC freezing bytecode"
	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=mpz $< > $@

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(Q)$(OBJCOPY) -O binary -j .isr_vector -j .text -j .data $^ $(BUILD)/firmware.bin

$(BUILD)/firmware.dfu: $(BUILD)/firmware.bin
	$(ECHO) "Create $@"
	$(Q)$(PYTHON) $(DFU) -b 0:$(BUILD)/firmware.bin $@

deploy: $(BUILD)/firmware.dfu
	$(ECHO) "Writing $< to the board"
	$(Q)$(PYTHON) $(PYDFU) -u $<

# Run emulation build on a POSIX system with suitable terminal settings
run:
	stty raw opost -echo
	build/firmware.elf
	@echo Resetting terminal...
# This sleep is useful to spot segfaults
	sleep 1
	reset

test: $(BUILD)/firmware.elf
	$(Q)/bin/echo -e "print('hello world!', list(x+1 for x in range(10)), end='eol\\\\n')\\r\\n\\x04" | $(BUILD)/firmware.elf | tail -n2 | grep "^hello world! \\[1, 2, 3, 4, 5, 6, 7, 8, 9, 10\\]eol"

include $(TOP)/py/mkrules.mk
