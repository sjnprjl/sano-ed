CC=gcc
CFLAGS=-Wall -Wextra -lncurses -g3
sano: sano.h viewport.c text.c editor.c sano.c
	$(CC) $(CFLAGS) $^ -o sano
	
	
