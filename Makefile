CC		= gcc

CFLAGS		= -std=c99 -Wall -Wshadow -Wvla -g -pedantic

# CFLAGS	= -std=c99 -Wall -Wshadow -Wvla -O3 -pedantic

WSLFLAGS  = -I/include -L/lib

AVROFLAGS	= -lavro

hello_world:
		$(CC) $(CFLAGS) $(WSLFLAGS) hello_world.c -o hello_world $(AVROFLAGS)

mem: hello_world
		valgrind ./hello_world

clean:
		rm -f hello_world.o hello_world

%.o: %.c
		$(CC) $(CFLAGS) $(WSLFLAGS) $(AVROFLAGS) -c $<
