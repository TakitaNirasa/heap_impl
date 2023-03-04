CC = avr-gcc
#FLAGS  = -g -c -Wall -mmcu=$(MCU)
FLAGS = -c -Wall -mmcu=$(MCU) -Wall -std=gnu11 -MMD -MP -g -o
LFLAGS = -mmcu=atxmega256a3 -g -o

# Компиляция исходника
# $1 Исходник
define COMPILE
$(OUTDIR)$(notdir $(patsubst %.c,%.o,$1)): $1
	$(if $V,,@echo COMPILE $$< &&)$$(CC) $$(FLAGS) $$@ $$<

endef

# Линковка
# $1 Наименования для цели
# $2 Список объектников
define LINK
$1: $2
	$(if $V,,@echo LINK $$@ &&)$$(CC) $$(LFLAGS) $(OUTDIR)$(patsubst %,%.elf,$$@) $2

endef