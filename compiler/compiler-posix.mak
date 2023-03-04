CC  = gcc
FLAGS  = -g -c -Wall
LFLAGS = -g

# Компиляция исходника
# $1 Исходник
define COMPILE
$(OUTDIR)$(notdir $(patsubst %.c,%.o,$1)): $1
	$(if $V,,@printf "$(GREEN)COMPILE SRC$(NORM) %s\n" $$< &&)$$(CC) $$(FLAGS) $$< -o $$@

endef

# Линковка
# $1 Наименования для цели
# $2 Список объектников
define LINK
$1: $2
	$(if $V,,@printf "$(GREEN)LINK $(NORM) %s\n" $$@ &&)$$(CC) $$(LFLAGS) $2 -o $(OUTDIR)$$@ 

endef
