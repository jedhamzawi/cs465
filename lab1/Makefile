CC=gcc
CC_FLAGS := -g

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   :=
EXECUTABLE  := ham-aes


all: $(BIN)/$(EXECUTABLE)

run: clean all
		clear
		./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c
		$(CC) $(CC_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean: -rm $(BIN)/*
