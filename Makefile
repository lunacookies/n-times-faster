CC=clang

CFLAGS=\
	-std=c99 \
	-g3 \
	-O3 \
	-Wall \
	-Wpedantic \
	-Wconversion \
	-Wdouble-promotion

NAME=n-times-faster
BUILD_DIR=out
HEADERS=$(wildcard *.h)
SOURCES=$(wildcard *.c)
OBJECTS=$(addprefix $(BUILD_DIR)/, $(SOURCES:.c=.o))

all: $(BUILD_DIR)/$(NAME) tidy

$(BUILD_DIR)/$(NAME): $(OBJECTS)
	@ mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.c $(HEADERS)
	@ mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

tidy: main.c
	clang-format -i $^

clean:
	rm -r $(BUILD_DIR)
