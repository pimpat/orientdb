MAKE = make
CPP = g++
CC = gcc

SRC_DIR = ./src/
INCLUDE_DIR = ./include/
LIB_DIR = ./lib/

FILENAME:= $(wildcard $(SRC_DIR)*.c) $(wildcard $(SRC_DIR)*.cpp)
CPP_FLAG = -c
C_FLAG = -c

all:$(notdir $(FILENAME))
	
clean:
	rm $(LIB_DIR)*.o
%.c:
	$(CC) $(C_FLAG)  -I$(INCLUDE_DIR) $(SRC_DIR)$@ -o $(LIB_DIR)$(patsubst %.c,%.o,$@)

%.cpp:
	$(CPP) $(CPP_FLAG)  -I$(INCLUDE_DIR) $(SRC_DIR)$@ -o $(LIB_DIR)$(patsubst %.cpp,%.o,$@)
