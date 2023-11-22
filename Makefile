# Files
BUILD_DIR = build
NAME = example

SRC = example.c crt0.S

SRC_C = $(filter %.c, $(SRC))
SRC_S = $(filter %.S, $(SRC))
OBJ_C = $(addprefix $(BUILD_DIR)/,$(SRC_C:%.c=%.o))
OBJ_S = $(addprefix $(BUILD_DIR)/,$(SRC_S:%.S=%.o))
OBJ = $(OBJ_S) $(OBJ_C)

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

# Flags
CFLAGS = -O3 -fomit-frame-pointer -marm -Werror
EMUFLAGS =
GDBFLAGS =
TERMFLAGS = >/dev/null 2>&1

CFLAGS_ALL = -Wpedantic -Wall -ggdb3 -mcpu=arm7tdmi -nostartfiles -ffreestanding -mthumb-interwork -mthumb $(CFLAGS)
CPPFLAGS = -Iinclude
LDFLAGS = -T$(LSCRIPT) -nostdlib
OBJCOPYFLAGS = -Obinary -S --set-section-flags .bss=contents,alloc,load,data
OBJDUMPFLAGS = -dfzh

# Default target
.PHONY: all
all: elf dump

-include $(OBJ:.o=.d)

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
.PHONY: gba elf dump debug emu gdb_window gdb clean
gba: $(BUILD_DIR)/$(NAME).gba
elf: $(BUILD_DIR)/$(NAME).elf
dump: $(BUILD_DIR)/$(NAME)_dump.s
debug: gdb_window emu_debug

emu: $(BUILD_DIR)/$(NAME).gba
	$(EMU) $(EMUFLAGS) $(BUILD_DIR)/$(NAME).gba

emu_debug: $(BUILD_DIR)/$(NAME).gba
	$(EMU) $(EMUFLAGS) $(BUILD_DIR)/$(NAME).gba

gdb_window: $(BUILD_DIR)/$(NAME).gba
	$(TERM) $(TERMFLAGS) -e bash -c 'make gdb && killall $(EMU)' &

gdb: $(BUILD_DIR)/$(NAME).elf
	$(GDB) $(GDBFLAGS) -ex "target remote localhost:2345" $<

clean:
	rm -rf $(BUILD_DIR)
