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

# Файл с целями сборки тестов
include compiler/colors.mak
# Файл с макросами компиляции и линковки
include compiler/compiler-$(TARGET).mak
# Файл с целями сборки тестов
include compiler/test_build_cov.mak
# Файл с целями сборки тестов
include compiler/coverage.mak


# Генерация правил компилции
$(eval $(foreach _,$(SRC),$(call COMPILE,$_,$1)))

# Генерация правила линковки
$(eval $(foreach _,$(OUT),$(call LINK,$(OUT),$(OBJS))))

# Фейковая цель для сборки проекта
.PHONY: all
all: $(OUT)

# Фейковая цель clean для очистки от артефактов
.PHONY: clean
clean:
	rm -rf $(OUTDIR)*
