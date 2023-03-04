GCC.MCU += -mthumb

CC = arm-none-eabi-gcc
FLAGS = -c -g -mcpu=cortex-m4

LD = arm-none-eabi-gcc
LDFLAGS.general = $(GCC.MCU) -Wl,--gc-sections -nostdlib -Wl,--entry=Reset_Handler
LDFLAGS.release = -s
LDFLAGS.debug = -g

PROG ?= st-flash
PROGFLAGS ?= --reset --format ihex write


# Линковка
# $1 Наименования для цели
# $2 Список объектников
define LINK
$1: $2
	$(if $V,,@echo LINK $$@ &&)$$(CC) $$(LFLAGS) -o $(OUTDIR)$$@  $2 

endef


# Компиляция исходника
# $1 Исходник
define COMPILE
$(OUTDIR)$(notdir $(patsubst %.c,%.o,$1)): $1
	$(if $V,,@echo CXX $$< &&)$$(CC) $$(FLAGS) -o $$@ $$<

endef # RULES.COMPILE

define RULES.RESULT.elf
$1: $2 $(foreach _,$(LIBS.$3),$(addprefix $(OUT),$(RESULT.$_))) $$(if $$(LDSCRIPT.$3),$$(LDSCRIPT.$3),$$(LDSCRIPT.default))
	$(if $V,,@echo LINK $$@ &&)$$(LD) $$(LDFLAGS) $$(LDFLAGS.$3) -o $$@ $$(filter-out %.ld,$$^) -T $$(filter %.ld,$$^) -Wl,-Map="$$(@:%.elf=%.map)"
	$(if $V,,@echo Firmware size: &&) arm-none-eabi-size -A $$@

endef # RULES.RESULT.elf

