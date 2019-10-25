CC=gcc
CFLAGS=`GraphicsMagick-config --ldflags --libs --cppflags`
CLIBS=

all: clean build

clean:
	rm -f main *.o ../bin/primg
build:
	$(CC) -o ../bin/primg ./main.c $(CFLAGS) $(CLIBS) -g -O3 -Wall -pedantic -Wextra -Wshadow

