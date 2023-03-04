CC = avr-gcc
#FLAGS  = -g -c -Wall -mmcu=$(MCU)
FLAGS = -c -Wall -mmcu=$(MCU) -Wall -std=gnu11 -MMD -MP -g -o
LFLAGS = -mmcu=atxmega256a3 -g -o

# Компиляция исходника
# $1 Исходник
define COMPILE
$(OUTDIR)$(notdir $(patsubst %.c,%.o,$1)): $1
	$(if $V,,@printf "$(GREEN)BUILD SRC$(NORM) %s\n" $$< &&)$$(CC) $$(FLAGS) $$@ $$<

endef

# Линковка
# $1 Наименования для цели
# $2 Список объектников
define LINK
$1: $2
	$(if $V,,@printf "$(GREEN)LINK $(NORM) %s\n" $$< &&)$$(CC) $$(LFLAGS) $(OUTDIR)$(patsubst %,%.elf,$$@) $2

endef