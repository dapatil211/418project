CC = g++
DEBUG = 0
LOG=0
CFLAGS = -O3 -g -Wall -DDEBUG=$(DEBUG) -DLOG=$(LOG) -std=c++11
LDFLAGS = -lm

COMP_CFILES = compression.cpp model.cpp trie.cpp
COMP_HFILES = model.h aricompressor.h trie.h
DECOMP_CFILES = decompression.cpp model.cpp trie.cpp
DECOMP_HFILES = model.h aridecompressor.h trie.h


all: comp decomp

comp: $(COMP_CFILES) $(COMP_HFILES)
	$(CC) $(CFLAGS) -o comp $(COMP_CFILES) $(LDFLAGS)

decomp: $(DECOMP_CFILES) $(DECOMP_HFILES)
	$(CC) $(CFLAGS) -o decomp $(DECOMP_CFILES) $(LDFLAGS)

clean: 
	rm -f comp decomp
