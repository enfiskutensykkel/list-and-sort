CC      := gcc
CFLAGS  := -Wall -Wextra -pedantic -Werror -g -Wno-unused-parameter
OBJS    := list.o heap.o graph.o
TARGETS := test-list test-heap dijkstra astar

.PHONY: all clean $(TARGETS)

all: $(TARGETS)

clean:
	-$(RM) $(TARGETS) $(TARGETS:%=%.o) $(OBJS)

%.o: %.c
	$(CC) -std=c99 -c $(CFLAGS) -o $@ $<

.SECONDEXPANSION:
$(TARGETS): $(OBJS) $$(@).o
	$(CC) -o $@ $^

