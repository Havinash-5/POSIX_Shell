CC = g++
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lreadline
TARGET = myshell
OBJ = src/shell.o src/commands.o src/utils.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

src/shell.o: src/shell.cpp include/shell.h include/commands.h include/utils.h
	$(CC) $(CFLAGS) -c src/shell.cpp -o src/shell.o

src/commands.o: src/commands.cpp include/commands.h include/utils.h
	$(CC) $(CFLAGS) -c src/commands.cpp -o src/commands.o

src/utils.o: src/utils.cpp include/utils.h
	$(CC) $(CFLAGS) -c src/utils.cpp -o src/utils.o

clean:
	rm -f $(OBJ) $(TARGET)