BUILDINC_DIRECTORY ?= .
ifeq ($(wildcard $(BUILDINC_DIRECTORY)/generated/variables.mak),)
define ERROR_MSG

$(BUILDINC_DIRECTORY)/generated/variables.mak not found.

MicroPython for LiteX requires these generated files to build. It
is important that the files match the gateware you are using and
are specific to each board / platform.

The LiteX generated files are found in build output directory
under "software/include/generated".

To allow MicroPython to find these files, you must do one of the
following;
 * Set BUILDINC_DIRECTORY.
 * Copy the files into a local generated directory.

endef
$(error $(ERROR_MSG))
endif # no generated/variables.mak
include $(BUILDINC_DIRECTORY)/generated/variables.mak

include ../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include ../py/py.mk

CROSS_COMPILE ?= $(TRIPLE)-

INC += -I.
INC += -I..
INC += -I../lib/mp-readline
INC += -I$(BUILD)
INC += -I$(BUILDINC_DIRECTORY)

CFLAGS += $(CPUFLAGS)
CFLAGS += $(INC) -Wall -Werror -std=gnu11 -ggdb $(COPT)
CFLAGS += -Og -Wdouble-promotion -Wall -Werror

ifneq ($(DEBUG), 1)
CFLAGS += -DNDEBUG
endif

LDFLAGS += -T litex.ld -Wl,-Map=$@.map -Wl,--cref
LDFLAGS += -L$(BUILDINC_DIRECTORY)
LIBS = -Wl,-lc

SRC_C = \
	main.c \
	uart.c \
	isr.c \
	modmachine.c \
	modlitex.c \
	litex_leds.c \
	lib/utils/stdout_helpers.c \
	lib/utils/interrupt_char.c \
	lib/utils/pyexec.c \
	lib/libc/string0.c \
	lib/mp-readline/readline.c

# FIXME: Should use crt0 from newlib? However it seems to cause memory access
# at 0x0
SRC_S = crt0-$(CPU).S

OBJ = $(addprefix $(BUILD)/, $(SRC_S:.S=.o)) $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

all: $(BUILD)/firmware.bin

# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) $(SRC_LIB)

$(BUILD)/crt0-$(CPU).S: $(LIBBASE_DIRECTORY)/crt0-$(CPU).S
	cp $< $@

$(BUILD)/_frozen_mpy.c: frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(ECHO) "MISC freezing bytecode"
	$(Q)../tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(Q)$(OBJCOPY) -O binary $^ $(BUILD)/firmware.bin

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

include ../py/mkrules.mk
