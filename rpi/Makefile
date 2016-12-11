ARMGNU ?= arm-none-eabi
BUILD = build/$(TARGET)
# TARGET = kernel.img

LIBDIRS = -L./lib/arm/lib/gcc
LIBDIRS += -L./lib/arm/lib/newlib
LIBS = -lc -lgcc
INCLUDE = -Ilib -I./lib/arm/incude/newlib

SRCS := $(shell find -name '*.c' -o -name '*.S')
DIRS = $(dir $(SRCS))
BUILDDIRS = $(addprefix $(BUILD)/, $(DIRS))

OBJS := $(SRCS:.S=.o)
OBJS := $(OBJS:.c=.o)
OBJS := $(addprefix $(BUILD)/, $(OBJS))

CC = $(ARMGNU)-gcc
AS = $(ARMGNU)-as
LD = $(ARMGNU)-ld

CFLAGS = -O2 -c -ffreestanding -fno-delete-null-pointer-checks
ifdef BOARD
CFLAGS += -D $(BOARD)
endif
ifeq ($(BOARD), RPI2)
CFLAGS += -mfpu=neon-vfpv4 -mfloat-abi=softfp -march=armv7-a -mtune=cortex-a7
else
CFLAGS += -mfpu=vfp -mfloat-abi=softfp -march=armv6zk -mtune=arm1176jzf-s
endif

ASFLAGS = -c
ifdef BOARD
ASFLAGS += --defsym $(BOARD)=1
endif

LDFLAGS = -static -nostartfiles -T kernel.ld

.PHONY: default all compile clean

default:
	make all --no-print-directory TARGET=kernel
	make all --no-print-directory TARGET=kernel7 BOARD=RPI2

all:
	[ -d $(BUILD) ] || mkdir -p $(BUILD)
	[ -d "$(BUILDDIRS)" ] || mkdir -p $(BUILDDIRS)
	make compile --no-print-directory

compile: $(TARGET).img

$(TARGET).img: $(BUILD)/output.elf
	$(ARMGNU)-objcopy $< -O binary $@

$(BUILD)/output.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBDIRS) $(LIBS)

$(BUILD)/%.o: %.S
	$(AS) $(ASFLAGS) -c -o $@ $<

$(BUILD)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $<

clean:
	rm -rf kernel.img kernel7.img
	rm -rf build
