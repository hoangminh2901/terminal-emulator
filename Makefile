CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -Wl,--allow-multiple-definition
SOURCES = main.c read.c split.c execute.c execute/builtin.c execute/non-builtin.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = terminal

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
