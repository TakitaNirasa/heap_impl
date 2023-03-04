#
#	Генерация файлов покрытия (для работы требуется заранее запустить тест для генерациии *.gcda)
#
#

# Папка для сгенерированного покрытия
OUTCOV = $(OUTDIR)coverage/

# Файл покрытия
INFO_FILE = $(OUTCOV)test.info

# Модуль генерации файлов покрытия
COV = lcov
COV_CAPTURE_FLAGS = --rc lcov_branch_coverage=1 --directory $(OUTDIR) --capture --output-file $(INFO_FILE)
COV_FILTER_FLAGS = --rc lcov_branch_coverage=1 --remove $(INFO_FILE) '/usr/include/*' '/usr/local/include/*' '$(realpath $(TESTS_DIR))/*' -o $(INFO_FILE)

# Модуль генерации красивого вывода покрытия
GEN = genhtml
GEN_FLAGS = --output-directory $(OUTCOV) --branch-coverage $(INFO_FILE)

# Папка с покрытием
$(OUTCOV) :
	mkdir $(OUTDIR)coverage

# Генерация файлов покрытия и html страницы
.PHONY : coverage
coverage: tests | $(OUTCOV)
	$(if $V,,@printf "$(GREEN)COVERAGE CAPTURE %s $(NORM)\n" &&) $(COV) $(COV_CAPTURE_FLAGS)
	$(if $V,,@printf "$(GREEN)COVERAGE FILTER %s$(NORM)\n" &&) $(COV) $(COV_FILTER_FLAGS)
	$(if $V,,@printf "$(GREEN)COVERAGE GENERATING %s$(NORM)\n" &&) $(GEN) $(GEN_FLAGS)