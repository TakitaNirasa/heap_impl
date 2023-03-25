#
#	Сборка и запуск тестов
#
#

# Общие флаги компилятора
TEST_FLAGS = -g -Wno-unused-result -Wall -Wno-narrowing -gdwarf -DDEBUG -D__HOST__ -D_GNU_SOURCE  -MMD -I$(TESTS_DIR) -I. -fno-exceptions -fno-inline

# Флаг генерации .gcda файлов
TEST_FLAG_GEN_GCDA = -fprofile-arcs -ftest-coverage

# Компилятор С++ и его флаги
TEST_CXX = g++
TEST_CXXFLAGS = -c -std=c++17 $(TEST_FLAGS) $(COMPILE_FLAGS)

# Линкер и его флаги
TEST_LINKER = g++
TEST_LINKFLAGS = -g -lgtest -lgtest_main -lgcov --coverage -lpthread

# Исполняемые файлы
TEST_OUT = $(addprefix $(OUTDIR),$(notdir $(patsubst %.cpp,%,$(TESTS))))

# Запуск всего процесса сборки и выполнения тестов
.PHONY: tests
tests : runtests

# Выполнение тестов
.PHONY: runtests
runtests : $(notdir $(TEST_OUT))

# Компиляция исходника, линковка, запуск
# $1 Исходник
define TEST_COMPILE
$(notdir $(patsubst %.cpp,%,$1)): $1
	$(if $V,,@printf "$(YELLOW)MAKE TEST$(NORM) %s\n" $$< &&)$(TEST_CXX) $(TEST_CXXFLAGS) $(TEST_FLAG_GEN_GCDA) -o $(OUTDIR)$(notdir $(patsubst %.cpp,%.o,$1)) $$<
	$(if $V,,@printf ""&&)$(TEST_LINKER) $(OUTDIR)$(notdir $(patsubst %.cpp,%.o,$1)) $(TEST_LINKFLAGS) -o $(OUTDIR)$$@
	./$(OUTDIR)$(notdir $(patsubst %.cpp,%,$1))

endef
