SRC = main.c
TARGET = a
CFLAGS = -Wall -Wextra
CC = gcc

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC)  $(SRC) -o $(TARGET) $(CFLAGS)

clean:
	rm -f $(TARGET)
