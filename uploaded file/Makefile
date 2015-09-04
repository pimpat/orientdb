.PHONY: dep


include Makefile.mk
BUILD_DIR = build/
SRC_DIR = src/
BIN_DIR = build/bin/
DEBUG_DIR = build/debug/
DEPEN_DIR = dep/

DEPEN_FILE = $(wildcard $(DEPEN_DIR)lib/*.o)
DEPEN_LIB_DIR = dep/lib/
INCLUDE_DIR = $(DEPEN_DIR)include/

#################### OUTPUT ####################
OUTPUT_O = app.o
################################################

#################### SOURCE ####################
SRC_C = $(wildcard $(SRC_DIR)*.cpp) $(wildcard $(SRC_DIR)*.c)
BIN_OBJ = $(notdir $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(SRC_C))))
################################################


bin:dep $(notdir $(SRC_C))
	$(CC) $(TP_LIB) -o $(BIN_DIR)$(OUTPUT_O) $(DEPEN_FILE) $(addprefix $(BIN_DIR),$(BIN_OBJ)) $(DEPEN_LIB)

debug: dep $(addsuffix _debug,$(notdir $(SRC_C)))
	@echo $(addsuffix _debug,$(notdir $(SRC_C)))
	$(CC) $(TP_LIB) -g -o $(DEBUG_DIR)$(OUTPUT_O) $(DEPEN_FILE) $(addprefix $(DEBUG_DIR),$(BIN_OBJ)) $(DEPEN_LIB)

%.c_debug:
	$(CC) $(DEBUG_FLAG)  -I$(INCLUDE_DIR) $(SRC_DIR)$(patsubst %.c_debug,%.c,$@) -o $(DEBUG_DIR)$(patsubst %.c_debug,%.o,$@)

%.cpp_debug:
	$(CC) $(DEBUG_FLAG)  -I$(INCLUDE_DIR) $(SRC_DIR)$(patsubst %.cpp_debug,%.cpp,$@) -o $(DEBUG_DIR)$(patsubst %.cpp_debug,%.o,$@)

%.c:
	$(CC) $(BUILD_FLAG)  -I$(INCLUDE_DIR) $(SRC_DIR)$@ -o $(BIN_DIR)$(patsubst %.c,%.o,$@)

%.cpp:
	$(CC) $(BUILD_FLAG)  -I$(INCLUDE_DIR) $(SRC_DIR)$@ -o $(BIN_DIR)$(patsubst %.cpp,%.o,$@)

dep:
	$(MAKE) -C $(DEPEN_DIR)


all: $(OUTPUT_O)


$(OUTPUT_O): $(SRC_C) $(SRC_H)
	$(CC) -o $(OUTPUT_O) $(SRC_C)

clean:
	$(MAKE) -C $(DEPEN_DIR) clean
	rm -rf $(OUTPUT_O)

