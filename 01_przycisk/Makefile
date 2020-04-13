# STM32 - Makefile do ćwiczeń
# projekt ,,Python w Elektronicznej Sieci''
# Krzysztof Lasocki <krz.lasocki@gmail.com>

#this makefile is helped in part by https://github.com/libopencm3/libopencm3-examples/blob/master/examples/Makefile.rules

########################################
# based on rue's makefile
# small edits added by polprog for his conveinience
# update 19.03.2020 polprog: updated to work with 2 yrs worth of changes
#                   in locm3 the project dir is set up so that OPENCM3_DIR
#                   is a symlink to the actual locm3 dir cloned from github.
#                   You have to build that first ofc.
# update 01.04.2020 polprog: added -mcpu=cortex-m3 -mthumb to LDFLAGS as well
#                   to make sure newlib functions are being linked in with the
#                   correct instruction encoding
#
# The contents of this file are public domain work.
########################################


SERIAL = /dev/ttyUSB0

#what is the main file 
BINARY = program



CFILES = $(wildcard *.c)
HFILES = $(wildcard *.h)
OBJS = $(CFILES:%.c=%.o)

#LibopenCM3 location
OPENCM3_DIR = libopencm3
SRCLIBDIR = $(OPENCM3_DIR)
INCLUDE_DIR = $(OPENCM3_DIR)/include
LIB_DIR = $(OPENCM3_DIR)/lib


OPENOCD_SCRIPTS_DIR = /usr/share/openocd/scripts

# Device specific things
DEVICE = stm32f103c8t6

toolchainPath ?= /usr/bin/

PREFIX		?= $(toolchainPath)arm-none-eabi

CC		:= $(PREFIX)-gcc
LD		:= $(PREFIX)-gcc
AR		:= $(PREFIX)-ar
AS		:= $(PREFIX)-as
OBJCOPY		:= $(PREFIX)-objcopy
OBJDUMP		:= $(PREFIX)-objdump
GDB		:= $(PREFIX)-gdb


CFLAGS = 
#with thanks to libopencm3 makefile
CFLAGS += -g
CFLAGS += -Ilibopencm3/include/
CFLAGS += -Wextra -Wshadow -Wimplicit-function-declaration -Wredundant-decls 
CFLAGS += -fno-common -ffunction-sections -fdata-sections
CFLAGS += -MD
CFLAGS += -Wall -Wundef
CFLAGS += -mcpu=cortex-m3 -mthumb
CFLAGS += -T $(LDSCRIPT)

#Includes
CFLAGS += -I $(INCLUDE_DIR)


#######  LD Script #################
# libopencm3 makefile that does the ldscript
# uses libopencm3 to make a .ld file for the specified device
LDFLAGS		?= 
LDFLAGS		+= --static -nostartfiles
LDFLAGS		+= -L$(LIB_DIR)
LDFLAGS		+= -T$(LDSCRIPT)
LDFLAGS		+= -Wl,-Map=$(*).map
LDFLAGS		+= -Wl,--gc-sections


LDLIBS		+= -lopencm3_stm32f1
LDLIBS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group 
LDLIBS          += -lm -mcpu=cortex-m3 -mthumb


.SUFFIXES: .elf .bin .hex .srec .list .map .images
.SECONDEXPANSION:
.SECONDARY:

all: elf bin

include $(OPENCM3_DIR)/mk/genlink-config.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk


elf: $(BINARY).elf
bin: $(BINARY).bin
flash: $(BINARY).flash


%.bin: %.elf
	@#printf "  OBJCOPY $(*).bin\n"
	$(OBJCOPY) -Obinary $(*).elf $(*).bin

%.elf %.map: $(OBJS) $(HFILES) $(LDSCRIPT)
	@#printf "  LD      $(*).elf\n"
	$(LD) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(BINARY).elf

%.o: %.c
	@#printf "  CC      $(*).c\n"
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(*).o -c $(*).c

clean:
	@#printf "  CLEAN\n"
	$(RM) $(OBJS)
	$(RM) *.o *.d *.elf *.bin *.hex *.srec *.list *.map


install: program.bin
	stm32flash -w $< $(SERIAL)
	stm32flash -g 0x0 $(SERIAL)
