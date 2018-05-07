
PROG = einhorn
CC = gcc

CFLAGS ?= -Wall -std=c11 -g -Wextra -D_BSD_SOURCE
LDFLAGS ?= -lm

# LUA
CFLAGS += $(shell pkg-config --cflags lua5.3)
LDFLAGS += $(shell pkg-config --libs lua5.3)


all: $(PROG)

boot.h: boot.lua
	xxd -i boot.lua > boot.h

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

$(PROG): main.o config.o engine.o framebuffer.o net.o
	$(CC) -o $(PROG) $^ $(LDFLAGS)


clean:
	rm -f *.o
	rm -rf *.o*
	rm -f $(PROG) 
