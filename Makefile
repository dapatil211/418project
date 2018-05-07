CC = g++
DEBUG = 0
CFLAGS = -O0 -g -Wall -DDEBUG=$(DEBUG) -std=c++11
LDFLAGS = -lm

CFILES = compression.cpp model.cpp
HFILES = model.h aricompressor.h

all: comp

comp: $(CFILES) $(HFILES)
	$(CC) $(CFLAGS) -o comp $(CFILES) $(LDFLAGS)

clean: 
	rm -f comp
