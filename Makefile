# Files
BUILD_DIR = build
NAME = playground

SRC = example.c crt0.S malloc.c memcpy.c memcpy2.S unscii-8-alt.S

SRC_C = $(filter %.c, $(SRC))
SRC_S = $(filter %.S, $(SRC))
OBJ_C = $(addprefix $(BUILD_DIR)/,$(SRC_C:%.c=%.o))
OBJ_S = $(addprefix $(BUILD_DIR)/,$(SRC_S:%.S=%.o))
OBJ = $(OBJ_S) $(OBJ_C)

HEADER_FILES = $$(find . -name '*.h')
DEP = $(OBJ:.o=.d)
-include $(DEP)

LSCRIPT = link.lds

# Toolchain
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
EMU = mgba-qt
GDB = arm-none-eabi-gdb
TERMINAL = kitty
LINT = cppcheck

# Flags
CFLAGS = \
-Wall -Wextra -Wpedantic -Wconversion -Wfloat-equal -Wconversion -Wlogical-op -Wundef -Wredundant-decls -Wshadow -Wstrict-overflow=2 -Wwrite-strings -Wpointer-arith -Wcast-qual -Wformat=2 -Wformat-truncation -Wmissing-include-dirs -Wcast-align -Wswitch-enum -Wsign-conversion -Wdisabled-optimization -Winline -Winvalid-pch -Wmissing-declarations -Wdouble-promotion -Wshadow -Wtrampolines -Wvector-operation-performance -Wshift-overflow=2 -Wnull-dereference -Wduplicated-cond -Wshift-overflow=2 -Wnull-dereference -Wduplicated-cond -Wcast-align=strict
EMUFLAGS =
GDBFLAGS =
TERMFLAGS = >/dev/null 2>&1

CFLAGS_ALL = -Wall -Wextra -Wpedantic -fomit-frame-pointer -mcpu=arm7tdmi -nostartfiles -nostdlib -ffreestanding -mthumb -mthumb-interwork -ggdb3 $(CFLAGS)
CPPFLAGS = -Iinclude
LDFLAGS = -T$(LSCRIPT) -nostdlib
OBJCOPYFLAGS = -Obinary -S --set-section-flags .bss=contents,alloc,load,data
OBJDUMPFLAGS = -dfzh
LINTFLAGS = $(SRC_C) $(HEADER_FILES)

# Default target
.PHONY: all
all: elf dump

# Single file targets
$(OBJ_C):$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_ALL) $(CPPFLAGS) -MMD -MP -c $< -o $@

$(OBJ_S):$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_ALL) $(CPPFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/$(NAME).elf: $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/$(NAME).gba: $(BUILD_DIR)/$(NAME).elf
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

$(BUILD_DIR)/$(NAME)_dump.s: $(BUILD_DIR)/$(NAME).elf
	$(OBJDUMP) $(OBJDUMPFLAGS) $< > $@

# General targets
.PHONY: gba elf dump debug emu gdb_window gdb lint compile_commands clean
gba: lint $(BUILD_DIR)/$(NAME).gba dump
elf: lint $(BUILD_DIR)/$(NAME).elf dump
dump: $(BUILD_DIR)/$(NAME)_dump.s
debug: gdb_window emu_debug

emu: lint $(BUILD_DIR)/$(NAME).gba dump
	$(EMU) $(EMUFLAGS) $(BUILD_DIR)/$(NAME).gba

emu_debug: lint $(BUILD_DIR)/$(NAME).gba
	$(EMU) $(EMUFLAGS) -g $(BUILD_DIR)/$(NAME).gba

gdb_window: $(BUILD_DIR)/$(NAME).gba
	$(TERM) $(TERMFLAGS) -e bash -c 'make gdb && killall $(EMU)' &

gdb: $(BUILD_DIR)/$(NAME).elf dump
	$(GDB) $(GDBFLAGS) -ex "target remote localhost:2345" $<

lint:
	$(LINT) $(LINTFLAGS)

compile_commands:
	make clean
	bear -- make dump


clean:
	rm -rf $(BUILD_DIR)
