CLEANUP=rm -f
MKDIR=mkdir -p
TARGET_EXTENSION=out


PATHU=unity/src/
PATHS=src/periph/* src/testcases/* ../src/fonts/* ../src/bitmaps/*
PATHI=inc/periph/ ../inc/ inc/testcases/ ../src/ ../inc/fonts/ ../inc/bitmaps/
PATHO=bin/
PATHT=./


INC_DIRS_F = -I. -I$(PATHU) $(patsubst %, -I%, $(PATHI))

SRC_FILES=$(filter-out ../src/main.c ../src/system_stm32f4xx.c, $(wildcard $(PATHS)*.c)) $(wildcard $(PATHU)*.c)
SRCT_FILES=$(wildcard $(PATHT)*.c)

COMPILE=gcc -C
LINK=gcc
CFLAGS=

$(OBJS) : Makefile

all: clean default

default: $(OBJS) $(SRC_FILES) $(SRCT_FILES)
	$(COMPILE) $(CFLAGS) $(INC_DIRS_F) $(SRC_FILES) $(SRCT_FILES) -o $(PATHO)tests
	- $(PATHO)tests

clean:
	$(CLEANUP) $(PATHO)*

.PHONY: clean