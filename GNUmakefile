# Файл с настройками
include config.mak
# По умолчанию целевая платформа posix
ifeq (,$(findstring $(TARGET),avr stm32 posix))
TARGET = posix

endif

# Директория назначения
OUTDIR = build/
# Исходники
SRC = $(wildcard src/*/*.c)
# Объектники 
OBJS = $(addprefix $(OUTDIR),$(notdir $(patsubst %.c,%.o,$(SRC))))
# Папка с исходниками тестов
TESTS_DIR = tests/
# Тесты (один, так как лень ещё один макрос писать)
TESTS = heap_test.cpp
# Наименование выходного файла
OUT = main

.PHONY: help
help:
	@printf "$(GREEN)Для корректной сборки проекта под целевую платформу требуется: $(NORM)\n"
	@printf "$(BLUE)	- Наличие компилятора под требуемую платформу - gcc(posix), arm-none-eabi-gcc(stm32), avr-gcc(avr)$(NORM)\n"
	@printf "$(BLUE)	- Наличие  компилятора g++, gtests, lcov (только для сборки и покрытия тестов)$(NORM)\n"
	@printf "$(GREEN)Для выбора платформы - укажите её в config.mak в переменной TARGET, контроллер(если требуется) указать в поле MCU$(NORM)\n\n"
	@printf "$(YELLOW)Описание целей сборки проекта: $(NORM)\n"
	@printf "$(BLUE)	all: $(GREEN)компиляция и линковка исходников  $(NORM)\n"
	@printf "$(BLUE)	clean: $(GREEN)очистка артефактов  $(NORM)\n"
	@printf "$(BLUE)	tests: $(GREEN)сборка и выполнение тестов  $(NORM)\n"
	@printf "$(BLUE)	coverage: $(GREEN)генерация html страницы с картой покрытия  $(NORM)\n"
	@printf "$(BLUE)	help: $(GREEN)отображение этой справки  $(NORM)\n"

# Файл с esc последовательностями цветов
include compiler/colors.mak
# Файл с макросами компиляции и линковки
include compiler/compiler-$(TARGET).mak
# Файл с целями сборки тестов
include compiler/test_build_cov.mak
# Файл с целями для генерации покрытия
include compiler/coverage.mak

# Генерация правил компилции
$(eval $(foreach _,$(SRC),$(call COMPILE,$_,$1)))

# Генерация правила линковки
$(eval $(foreach _,$(OUT),$(call LINK,$(OUT),$(OBJS))))

# Фейковая цель для сборки проекта
.PHONY: all
all: $(OUTDIR) $(OUT)

# Цель для создания директории с артефактами
$(OUTDIR):
	$(if $V,, &&)mkdir $(OUTDIR)

# Фейковая цель clean для очистки папки build
.PHONY: clean
clean:
	$(if $V,,@printf "$(RED)CLEAN$(NORM)\n" &&)rm -rf $(OUTDIR)*
