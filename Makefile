OBJS   := list.o heap.o
CC     := gcc
CFLAGS := -Wall -Wextra -pedantic -Werror -g

.PHONY: all clean test-list test-heap dijkstra astar

all: test-list test-heap dijkstra astar

astar: $(OBJS) astar.o
	$(CC) -o $@ $^

dijkstra: $(OBJS) dijkstra.o
	$(CC) -o $@ $^

test-list: $(OBJS) test_list.o
	$(CC) -o $@ $^

test-heap: $(OBJS) test_heap.o
	$(CC) -o $@ $^

clean:
	-$(RM) test-list test-heap dijkstra *.o

%.o: %.c
	$(CC) -std=c99 -c $(CFLAGS) -o $@ $<

