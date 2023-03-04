MCU += -mthumb

CC = arm-none-eabi-gcc
FLAGS = -c -g -mcpu=cortex-m4

LFLAGS = $(MCU) -s -Wl,--gc-sections -nostdlib -Wl,--entry=Reset_Handler

PROG ?= st-flash
PROGFLAGS ?= --reset --format ihex write

# Компиляция исходника
# $1 Исходник
define COMPILE
$(OUTDIR)$(notdir $(patsubst %.c,%.o,$1)): $1
	$(if $V,,@printf "$(GREEN)BUILD SRC$(NORM) %s\n" $$< &&)$$(CC) $$(FLAGS) $$< -o $$@

endef

# Линковка
# $1 Наименования для цели
# $2 Список объектников
define LINK
$1: $2
	$(if $V,,@printf "$(GREEN)LINK $(NORM) %s\n" $$@ &&)$$(CC) $$(LFLAGS) $2 -o $(OUTDIR)$$@ 

endef