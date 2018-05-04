
PROG = einhorn
CC = gcc

CFLAGS ?= -Wall -std=c11 -g
LDFLAGS ?= -llua

all: $(PROG)

boot.h: boot.lua
	xxd -i boot.lua > boot.h

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

$(PROG): main.o config.o engine.o framebuffer.o
	$(CC) $(LDFLAGS) -o $(PROG) $^


clean:
	rm -f *.o
	rm -rf *.o*
	rm -f $(PROG) 
