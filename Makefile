MAKE = make
CC = g++
.PHONY: dep

BUILD_FLAG = -w -c
DEBUG_FLAG = -w -c -g

BUILD_DIR = build/
SRC_DIR = src/
BIN_DIR = build/bin/
DEBUG_DIR = build/debug/
DEPEN_DIR = dep/


DEPEN_FILE = $(wildcard $(DEPEN_DIR)lib/*.o)
INCLUDE_DIR = $(DEPEN_DIR)include/

#################### OUTPUT ####################
OUTPUT_O = main
################################################

#################### SOURCE ####################
SRC_C = main.cpp
################################################

bin:dep
	$(CC) $(DEBUG_FLAG) -I$(INCLUDE_DIR) -o $(BIN_DIR)main.o $(SRC_DIR)$(SRC_C)
	$(CC) -o $(BIN_DIR)$(OUTPUT_O) $(DEPEN_FILE) $(BIN_DIR)main.o
	# g++ -w -c -g -I$(INCLUDE_DIR) main.cpp && g++ -o main main.o reqmsg.o swap_endian.o dmp.o ezxml.o && ./main

dep:
	$(MAKE) -C $(DEPEN_DIR)

debug:


all: $(OUTPUT_O)


$(OUTPUT_O): $(SRC_C) $(SRC_H)
	$(CC) -o $(OUTPUT_O) $(SRC_C)

clean:
	$(MAKE) -C $(DEPEN_DIR) clean
	rm -rf $(OUTPUT_O)

