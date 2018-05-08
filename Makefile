CC = g++
DEBUG = 0
LOG=0
OMP =-fopenmp -DOMP
CFLAGS = -O3 -g -Wall -DDEBUG=$(DEBUG) -DLOG=$(LOG) -std=c++11
LDFLAGS = -lm

COMP_CFILES = compression.cpp model.cpp trie.cpp
COMP_HFILES = model.h aricompressor.h trie.h
DECOMP_CFILES = decompression.cpp model.cpp trie.cpp
DECOMP_HFILES = model.h aridecompressor.h trie.h


all: comp decomp comp-omp

comp-omp: $(COMP_CFILES) $(COMP_HFILES)
	$(CC) $(CFLAGS) $(OMP) -o comp-omp $(COMP_CFILES) $(LDFLAGS)

comp: $(COMP_CFILES) $(COMP_HFILES)
	$(CC) $(CFLAGS) -DOMP=0 -o comp $(COMP_CFILES) $(LDFLAGS)

decomp: $(DECOMP_CFILES) $(DECOMP_HFILES)
	$(CC) $(CFLAGS) -o decomp $(DECOMP_CFILES) $(LDFLAGS)

clean:
	rm -f comp decomp comp-omp
