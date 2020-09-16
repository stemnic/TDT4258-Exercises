# Example Makefile
#
# Exercise 1, TDT4258

TOP         = $(shell pwd)
BUILDDIR    = $(TOP)/build
TARGET      = binary

LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

ifndef MODE
$(error > make MODE=interrupt/polling)
endif

ifeq ($(MODE), interrupt)
obj-asm += /ex1_interrupt.o
endif

ifeq ($(MODE), polling)
obj-asm += /ex1_polling.o
endif

BUILDOBJ = $(addprefix $(BUILDDIR), $(obj-asm))

LINKERSCRIPT=efm32gg.ld

.SECONDARY: $(BUILDOBJ)
all: clean $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/$(TARGET).bin


$(BUILDDIR)/%.bin : $(BUILDDIR)/%.elf
	@${OBJCOPY} -j .text -O binary $< $@

$(BUILDDIR)/%.elf: $(BUILDOBJ)
	@${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

$(BUILDDIR)/%.o : %.s
	@mkdir -p $(dir $@)
	@echo " >" $<
	@${AS} ${ASFLAGS} $< -o $@

.PHONY: upload
upload: all
	-eACommander.sh -r --address 0x00000000 -f "$(BUILDDIR)/$(TARGET).bin" -r

.PHONY: clean
clean:
	@rm -r -f $(BUILDDIR)
