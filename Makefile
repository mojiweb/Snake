CC = gcc

INC = -I./include
CFLAGS = -Wall -O2
DEPFLAGS = `sdl2-config --cflags`
LIBS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf

SRC_DIR = src
BUILD_DIR = build

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

BINARY = snake

all: $(BINARY)

$(BINARY): $(OBJS)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(INC) -o $@ $^ $(LIBS)
	chmod u+x $(BINARY)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $(DEPFLAGS) $(INC) $< -o $@ $(LIBS)

run: $(BINARY)
	./$(BINARY)

.PHONY: clean

debug: CFLAGS += -g -DDEBUG
debug: $(BINARY)

clean:
	rm -f $(OBJS) $(BINARY)