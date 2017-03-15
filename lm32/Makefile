include ../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include ../py/py.mk

CROSS_COMPILE ?= lm32-elf-

INC += -I.
INC += -I..
INC += -I../lib/mp-readline
INC += -I$(BUILD)

CFLAGS = $(INC) -Wall -Werror -std=gnu11 -ggdb $(COPT)
CFLAGS += -Og -Wdouble-promotion -Wall -Werror

ifneq ($(DEBUG), 1)
CFLAGS += -DNDEBUG
endif

LDFLAGS = -T lm32.ld -Wl,-Map=$@.map -Wl,--cref
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

SRC_S = crt0-lm32.S

OBJ = $(addprefix $(BUILD)/, $(SRC_S:.S=.o)) $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

all: $(BUILD)/firmware.bin

# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) $(SRC_LIB)

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
