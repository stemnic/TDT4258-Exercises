# Example Makefile

TOP = $(shell pwd)
BUILDDIR = $(TOP)/build
TARGET_NAME = ex2
MAKEFLAGS = -j8

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CPFLAGS += -I$(TOP)/include
CFLAGS=-mcpu=cortex-m3 -mthumb -g -std=gnu99 -Wall
LDFLAGS=-mcpu=cortex-m3 -mthumb -g -lgcc -lc -lcs3 -lcs3unhosted -lefm32gg -Llib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g
LINKERSCRIPT=lib/efm32gg.ld

# Add object files here
obj-y :=
obj-y += /src/gpio.o
obj-y += /src/dac.o
obj-y += /src/timer.o
obj-y += /src/main.o

OBJ = $(addprefix $(BUILDDIR), $(obj-y))

.SECONDARY: $(OBJ)
.PHONY : clean upload
all: $(BUILDDIR)/$(TARGET_NAME).elf $(BUILDDIR)/$(TARGET_NAME).bin $(BUILDDIR)/$(TARGET_NAME).lss

$(BUILDDIR)/%.lss: $(BUILDDIR)/%.elf
	@$(OBJDUMP) -h -S $< > $@

$(BUILDDIR)/%.bin : $(BUILDDIR)/%.elf
	@$(OBJCOPY) -O binary $< $@

$(BUILDDIR)/%.elf : $(OBJ)
	@$(LD) -T $(LINKERSCRIPT) $^ -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o : %.c
	@mkdir -p $(dir $@)
	@echo " >" $(notdir $<)
	@$(CC) $(CFLAGS) $(CPFLAGS) -c $< -o $@

upload: all
	-eACommander.sh -r --address 0x00000000 -f $(BUILDDIR)/$(TARGET_NAME).bin -r

clean:
	@rm -r -f $(BUILDDIR)
