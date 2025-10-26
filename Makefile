BIN = main
SRC = main.c

CFLAGS = -Wall

NP ?= 4

.PHONY: all compile run clean

all: clean compile 

compile: $(SRC)
	@mpicc $(CFLAGS) -o $(BIN) $(SRC)

run: $(BIN)
	@mpirun -np $(NP) ./$(BIN) $(N)

clean:
	@rm -f $(BIN)