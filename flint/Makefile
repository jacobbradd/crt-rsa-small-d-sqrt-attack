CC = gcc
FLINTFLAGS := $(shell pkg-config --cflags flint)
FLINTLIBS := $(shell pkg-config --libs flint)
CFLAGS = $(FLINTFLAGS) $(FLINTLIBS) -std=c2x

.PHONY: all clean

all: flint_rsa

flint_rsa: main.c
	$(CC) main.c -o $@ $(CFLAGS)

clean:
	rm flint_rsa

