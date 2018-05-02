
PROG = einhorn
CC = gcc

CFLAGS ?= -Wall -std=c11 -g
LDFLAGS ?=


%.o: %.c
	$(CC)  -c $(CFLAGS) -o $@ $^

$(PROG): main.o
	$(CC) $(LDFLAGS) -o $(PROG) $^


clean:
	rm -f *.o
	rm -f $(PROG) 
