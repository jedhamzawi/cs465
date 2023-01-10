CC=clang
CC_FLAGS := -g

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   :=
EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE)

run: clean all
		clear
		./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c
		$(CC) $(CC_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean: -rm $(BIN)/*
