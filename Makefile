SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))
BIN =gctest
CC = gcc
CFLAGS = -ggdb3 -Wall -Wextra -I.
LD = gcc

all:$(BIN)

$(BIN):$(OBJS)
	$(LD) -o $@ $^

%.o:%.c gwgc.h gwgc_ds.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o $(BIN)
