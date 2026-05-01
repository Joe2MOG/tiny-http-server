CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=gnu89 -g

# Source and object files (all inside src/)
SRC = src/main.c src/server.c src/socket_utils.c src/response.c src/request_parser.c src/file_handler.c src/dispatcher.c
OBJ = $(SRC:.c=.o)

TARGET = server

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

betty:
	betty src/*.c src/*.h

.PHONY: all clean betty
