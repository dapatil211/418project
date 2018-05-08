CC = g++
DEBUG = 0
LOG=0
CFLAGS = -O3 -g -Wall -DDEBUG=$(DEBUG) -DLOG=$(LOG) -std=c++11
LDFLAGS = -lm

CFILES = compression.cpp model.cpp trie.cpp
HFILES = model.h aricompressor.h trie.h
all: comp

comp: $(CFILES) $(HFILES)
	$(CC) $(CFLAGS) -o comp $(CFILES) $(LDFLAGS)

clean: 
	rm -f comp
