ARMGNU ?= arm-none-eabi
BUILD = build
TARGET = kernel.img

LIBS = -L./lib/arm/lib/gcc
LIBS += -L./lib/arm/lib/newlib
INCLUDE = -Ilib -I./lib/arm/incude/newlib

SRCS := $(shell find -name '*.c' -o -name '*.S')
DIRS = $(dir $(SRCS))
BUILDDIRS = $(addprefix $(BUILD)/, $(DIRS))

OBJS := $(SRCS:.S=.o)
OBJS := $(OBJS:.c=.o)
OBJS := $(addprefix $(BUILD)/, $(OBJS))

.PHONY: default all clean

default:
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@[ -d "$(BUILDDIRS)" ] || mkdir -p $(BUILDDIRS)
	@make all --no-print-directory

all: $(TARGET)

$(TARGET): $(BUILD)/output.elf
	$(ARMGNU)-objcopy $< -O binary $@

$(BUILD)/output.elf: $(OBJS)
	$(ARMGNU)-ld -static -nostartfiles -T kernel.ld -o $@ $^ $(LIBS) -lc -lgcc

$(BUILD)/%.o: %.S
	$(ARMGNU)-as -o $@ $<

$(BUILD)/%.o: %.c
	$(ARMGNU)-gcc -mfpu=neon-vfpv4 -mfloat-abi=softfp -march=armv7-a -mtune=cortex-a7 -O2 -c -ffreestanding -fno-delete-null-pointer-checks $(INCLUDE) -o $@ $<

clean:
	rm -rf $(TARGET)
	rm -rf build
