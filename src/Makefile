CC=gcc
CFLAGS= -Wall -Wextra -pedantic -g -O3 -std=c11
CLIBS= -lm
INSTALL_PATH?=/usr/local

all: clean build

clean:
	rm -f ../bin/*

../bin/%.o: %.c
	$(CC) -o $@ -c  $< $(CFLAGS) $(CLIBS)

build: ../bin/main.o
	$(CC) -o ../bin/primg $^ $(CFLAGS) $(CLIBS)

install:
	cp ../bin/primg $(INSTALL_PATH)/bin/primg
