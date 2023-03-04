#
#	Сборка и запуск тестов
#
#

# Общие флаги компилятора
TEST_FLAGS = -g -Wno-unused-result -Wall -gdwarf -DDEBUG -D__HOST__ -D_GNU_SOURCE  -MMD -I$(TESTS_DIR) -I. -fno-exceptions -fno-inline

# Флаг генерации .gcda файлов
TEST_FLAG_GEN_GCDA = -fprofile-arcs -ftest-coverage

# Компилятор С++ и его флаги
TEST_CXX = g++
TEST_CXXFLAGS = -c -std=c++17 $(TEST_FLAGS) $(COMPILE_FLAGS)

# Линкер и его флаги
TEST_LINK = g++
TEST_LINKFLAGS = -g -lgtest -lgtest_main -lgmock -lgcov --coverage -lpthread

# Исходники
TEST_SRC = $(TESTS_DIR)$(TESTS)
# Объектники
TEST_OBJ = $(OUTDIR)$(patsubst %.cpp,%.o,$(TESTS))
# Исполняемый файл
TEST_OUT = $(OUTDIR)$(patsubst %.cpp,%,$(TESTS))

# Запуск всего процесса сборки и выполнения теста
.PHONY: tests
tests : runtests

# Выполнение теста
.PHONY: runtests
runtests : $(TEST_OUT)
	$(if $V,,@printf "$(GREEN)RUNNING TESTS$(NORM)\n" &&)./$(TEST_OUT)

# Компиляция
$(TEST_OBJ) : $(TEST_SRC)
	$(if $V,,@printf "$(GREEN)COMPILING TESTS$(NORM)\n" &&)$(TEST_CXX) $(TEST_CXXFLAGS) $(TEST_FLAG_GEN_GCDA) -o $@ $<

# Линковка
$(TEST_OUT) : $(TEST_OBJ)
	$(if $V,,@printf "$(GREEN)LINK TESTS$(NORM)\n" &&)$(TEST_LINK) $^ $(TEST_LINKFLAGS) -o $@
