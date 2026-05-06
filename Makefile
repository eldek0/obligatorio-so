CC       = gcc
CFLAGS   = -Wall -Wextra -std=c11 -g -I src
LDFLAGS  =
LDLIBS   =
TARGET   = ent

OBJ = main.o src/bigint.o src/utils.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJ) $(LDLIBS)

main.o: main.c src/bigint.h
	$(CC) $(CFLAGS) -c main.c

src/bigint.o: src/bigint.c src/bigint.h
	$(CC) $(CFLAGS) -c src/bigint.c -o src/bigint.o

src/utils.o: src/utils.c src/bigint.h
	$(CC) $(CFLAGS) -c src/utils.c -o src/utils.o

clean:
	rm -f $(OBJ) $(TARGET)