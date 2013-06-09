CC = gcc
CFLAGS = -Wall 
PROG = New

SRCS = main.c render.c ship.c matrix.c menu.c bmp.c starfield.c trigtable.c vl_array.c game.c

LIBS = -lSDL -lm

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
