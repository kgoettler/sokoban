CC=gcc
CFLAGS=--std=c89
LIBS=-lcurses

clean: sokoban
	rm -f $<

sokoban:
	$(CC) $(CFLAGS) ./src/sokoban.c $(LIBS) -o $@
