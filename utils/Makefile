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
# update 24.05.2020 rrp: remove unnused and redundant variables, use variables from
#                   included libopencm3 makefiles to set platform depended info,
#                   add `info` target
#
# The contents of this file are public domain work.
########################################

# Serial device for stm32flash
SERIAL ?= /dev/ttyUSB0

# Device specific things
DEVICE ?= stm32f103c8t6

# LibopenCM3 location
OPENCM3_DIR ?= ../libopencm3

# ARM toolchain path
TOOLCHAIN_PATH ?= /usr/bin/


# what is the main file 
BINARY = program

CFILES = $(wildcard *.c)
HFILES = $(wildcard *.h)
OBJS = $(CFILES:%.c=%.o)


PREFIX		?= $(TOOLCHAIN_PATH)arm-none-eabi
CC		:= $(PREFIX)-gcc
LD		:= $(PREFIX)-gcc
AR		:= $(PREFIX)-ar
AS		:= $(PREFIX)-as
OBJCOPY		:= $(PREFIX)-objcopy
OBJDUMP		:= $(PREFIX)-objdump
GDB		:= $(PREFIX)-gdb


CFLAGS += -g
CFLAGS += -Wextra -Wshadow -Wimplicit-function-declaration -Wredundant-decls 
CFLAGS += -Wall -Wundef
CFLAGS += -fno-common -ffunction-sections -fdata-sections
CFLAGS += -MD

LDFLAGS += --static -nostartfiles
LDFLAGS += -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map=$(*).map
LDFLAGS += -Wl,--gc-sections

LDLIBS += -lm
LDLIBS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group 


ifeq ("$(wildcard $(OPENCM3_DIR)/lib/libopencm3_stm32f1.a)","")
$(info   Nie znaleziono biblioteki opencm3.)
$(info   )
$(info   Utwórz link $(abspath $(OPENCM3_DIR)))
$(info   wskazujący na katalog z skompilowaną biblioteką opencm3, np.:)
$(info     ln -s /opt/libopencm3 $(abspath $(OPENCM3_DIR)))
$(info   lub użyj zmiennej OPENCM3_DIR aby wskazać położenie tego katalogu, np.:)
$(info     export OPENCM3_DIR=/opt/libopencm3; make;)
$(info   )
$(info   Pamiętaj, że biblioteka opencm3 musi być skompilowana.)
$(info   W tym celu, po zainstalowaniu wymaganych zależności,)
$(info   będąc w jej katalogu należy wydać polecenie make)
$(info   )
$(error  Could NOT find libopencm3)
endif

.SUFFIXES: .elf .bin .hex .srec .list .map .images
.SECONDEXPANSION:
.SECONDARY:

all: elf bin
elf: $(BINARY).elf
bin: $(BINARY).bin
flash: $(BINARY).flash

# libopencm3 makefiles that generate .ld file for the specified device and:
#  - set $(LDSCRIPT) variable and add rule to generate this file
#  - update $(ARCH_FLAGS) variable (set -mcpu and other platform depended flags)
#  - update $(CPPFLAGS) and $(LDFLAGS) variables (set -I and -L to libopencm3 dirs)
#  - update $(LDLIBS) variable (add platform depended lib, see $(LIBNAME))
include $(OPENCM3_DIR)/mk/genlink-config.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk

info:
	@echo DEVICE=$(DEVICE)
	@echo SERIAL=$(SERIAL)
	@echo
	@echo ARCH_FLAGS=$(ARCH_FLAGS)
	@echo CPPFLAGS=$(CPPFLAGS)
	@echo LDLIBS=$(LDLIBS)
	@echo LDFLAGS=$(LDFLAGS)


%.bin: %.elf
	@#printf "  OBJCOPY $(*).bin\n"
	$(OBJCOPY) -Obinary $(*).elf $(*).bin

%.elf %.map: $(OBJS) $(HFILES) $(LDSCRIPT)
	@#printf "  LD      $(*).elf\n"
	$(LD) $(LDFLAGS) $(OBJS) $(LDLIBS) $(ARCH_FLAGS) -o $(BINARY).elf

%.o: %.c
	@#printf "  CC      $(*).c\n"
	$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH_FLAGS) -o $(*).o -c $(*).c

clean:
	@#printf "  CLEAN\n"
	$(RM) $(OBJS)
	$(RM) *.o *.d *.elf *.bin *.hex *.srec *.list *.map generated.*

install: $(BINARY).bin
	stm32flash -w $< $(SERIAL)
	stm32flash -g 0x0 $(SERIAL)

run:
	stm32flash -g 0x0 $(SERIAL)
