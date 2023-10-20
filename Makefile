SRC := src
OBJ := obj
BIN := bin
EXECUTABLE:= shell
EXCLUDE_FILE:= src/timeout.c

SRCS := $(wildcard $(SRC)/*.c)
SRCS := $(filter-out $(EXCLUDE_FILE), $(SRCS))
OBJS := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
INCS := -Iinclude/
DIRS := $(OBJ)/ $(BIN)/
EXEC := $(BIN)/$(EXECUTABLE)

CC := gcc
CFLAGS := -g -Wall -std=c99 $(INCS)
LDFLAGS :=

all: $(EXEC) bin/mytimeout

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

bin/mytimeout: obj/timeout.o
	$(CC) $(CFLAGS) obj/timeout.o -o bin/mytimeout

obj/timeout.o: src/timeout.c include/timeout.h
	$(CC) -g -D_POSIX_C_SOURCE -Wall -std=c99 $(INCS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXEC) bin/mytimeout
	$(EXEC)

clean:
	rm $(OBJ)/*.o $(EXEC) bin/mytimeout

$(shell mkdir -p $(DIRS))

.PHONY: run clean all
