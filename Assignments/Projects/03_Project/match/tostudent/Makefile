GCC = gcc

CCFLAGS = -ggdb3 -Wall
CCFLAGS_DEBUG = -ggdb0 -Wall

all: match

match: main.c utils.c mmio.c sort.c oneway.c onewaywrapper.c nways.c nwayswrapper.c filter.c
	$(GCC) $(CCFLAGS) $^ -pthread -o $@

debug: main.c utils.c mmio.c sort.c oneway.c onewaywrapper.c nways.c nwayswrapper.c filter.c
	$(GCC) $(CCFLAGS_DEBUG) $^ -pthread -o match-sol

clean:
	rm -rf match

