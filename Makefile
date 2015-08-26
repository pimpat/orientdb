MAKE = make
CPP = g++
CC = gcc
.PHONY: dep

BUILD_FLAG = -w -c
DEBUG_FLAG = -w -c -g
TP_LIB = -L/usr/local/lib/ -ltransport -lzmq -I/usr/local/include

BUILD_DIR = build/
SRC_DIR = src/
BIN_DIR = build/bin/
DEBUG_DIR = build/debug/
DEPEN_DIR = dep/


DEPEN_FILE = $(wildcard $(DEPEN_DIR)lib/*.o)
INCLUDE_DIR = $(DEPEN_DIR)include/

#################### OUTPUT ####################
OUTPUT_O = app.o
################################################

#################### SOURCE ####################
SRC_C = $(wildcard $(SRC_DIR)*.cpp) $(wildcard $(SRC_DIR)*.c)
BIN_OBJ = $(notdir $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(SRC_C))))
################################################

bin:dep $(notdir $(SRC_C))
	$(CPP) $(TP_LIB) -o $(BIN_DIR)$(OUTPUT_O) $(DEPEN_FILE) $(addprefix $(BIN_DIR),$(BIN_OBJ))
	# g++ -w -c -g -I$(INCLUDE_DIR) main.cpp && g++ -o main main.o reqmsg.o swap_endian.o dmp.o ezxml.o && ./main
	#./build/bin/app.o

%.c:
	$(CC) $(BUILD_FLAG)  -I$(INCLUDE_DIR) $(SRC_DIR)$@ -o $(BIN_DIR)$(patsubst %.c,%.o,$@)

%.cpp:
	$(CC) $(DEBUG_FLAG) $(TP_LIB) -I$(INCLUDE_DIR) $(SRC_DIR)$@ -o $(BIN_DIR)$(patsubst %.cpp,%.o,$@)

dep:
	$(MAKE) -C $(DEPEN_DIR)

debug:


all: $(OUTPUT_O)


$(OUTPUT_O): $(SRC_C) $(SRC_H)
	$(CC) -o $(OUTPUT_O) $(SRC_C)

clean:
	$(MAKE) -C $(DEPEN_DIR) clean
	rm -rf $(OUTPUT_O)

