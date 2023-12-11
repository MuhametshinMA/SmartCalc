CC=gcc
FLAGS=-Werror -Wall -Wextra -std=c11
FLAG_GCOV=-fprofile-arcs -ftest-coverage
FLAGS_TEST_LM=`pkg-config --cflags --libs check` -lm

UNAME:=$(shell uname -s)

ifeq ($(UNAME),Darwin)
	FLAG_TEST=-lcheck
endif
ifeq ($(UNAME),Linux)
	FLAG_TEST=-lcheck -lm -lrt -pthread -lsubunit
	# FLAG_TEST=`pkg-config --cflags --libs check` -lm
endif

APP_NAME = smart_calc
ifeq ($(UNAME), Darwin)
	APP_NAME = smart_calc.app
endif

QT_INSTALL_PREFIX = $(shell $(QMAKE) -query QT_INSTALL_PREFIX)

SRC_FILES=queue.c parser.c lexem.c calc.c

DSRC_FILES=queue.c parser.c lexem.c calc.c tst.c

CHECK_FILES=calc_test.check

TEST_FILES=test.c
O_TEST_FILES=test.o
OUT_FILE=test.out

BUILD_DIR=../build
APP_DIR = ../app

all: gcov_report install run dvi

install: build
	mkdir -p $(APP_DIR)
	cp -r $(BUILD_DIR)/$(APP_NAME) $(APP_DIR)/$(APP_NAME)

uninstall:
	rm -rf $(APP_DIR)

run:
	open ../app/smart_calc.app

clean:
	rm -rf build* *.out *.o *.a test.c *.gcov *.gcno *.gcda *.gch *.info lcover_rep $(BUILD_DIR) $(APP_DIR)

dvi:
	open DVI.html

build:
	mkdir -p $(BUILD_DIR)
	cmake -DCMAKE_PREFIX_PATH=$(QT_INSTALL_PREFIX) CMakeLists.txt -B $(BUILD_DIR)
	make -C $(BUILD_DIR)

dist:
	cd .. && tar -zcvf smart_calc.tar.gz src

test: calc.a
	checkmk $(CHECK_FILES) > $(TEST_FILES)
	$(CC) $(FLAGS) -c $(TEST_FILES) $(FLAG_TEST)
	$(CC) $(FLAGS) $(O_TEST_FILES) calc.a -o $(OUT_FILE) $(FLAG_TEST)
	./$(OUT_FILE)

gcov_report: test
	$(CC) $(SRC_FILES) $(TEST_FILES) $(FLAGS) $(FLAG_TEST) $(FLAG_GCOV)
	./a.out
	gcov $(SRC_FILES)

lcov_report:
	lcov --capture --directory . --output-file lcover.info
	genhtml lcover.info --output-directory lcover_rep
	open lcover_rep/index.html

calc.a:
	$(CC) $(FLAGS) -c $(SRC_FILES)
	ar rc calc.a *.o

style:
	clang-format -n --style=google *.c

d:
	$(CC) $(FLAG) $(DSRC_FILES) -o $(OUT_FILE) -lm -g
	./$(OUT_FILE)

val:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./test.out

istyle:
	clang-format -i --style=google *.c

nstyle:
	clang-format -n --style=google *.c

brew:
	#cd ~/goinfre
	#git clone https://github.com/Homebrew/brew homebrew
	#eval "$(~/goinfre/littleca/homebrew/bin/brew shellenv)"

lcov:
	brew install lcov
