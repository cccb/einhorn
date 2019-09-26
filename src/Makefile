
# Include local build adjustments
-include .Makefile.local

PROG = einhorn

CFLAGS ?= -Wall -std=c11 -g -Wextra -D_BSD_SOURCE
LDFLAGS ?= -lm

# LUA
CFLAGS += $(shell pkg-config --cflags lua53)
LDFLAGS += $(shell pkg-config --libs lua53)

all: $(PROG)

boot.h: boot.lua
	xxd -i boot.lua > boot.h

engine.o: boot.h

$(PROG): main.o config.o engine.o framebuffer.o net.o
	$(CC) -o $(PROG) $^ $(LDFLAGS)

clean:
	rm -rf *.o $(PROG) boot.h
