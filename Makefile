OBJS   := list.o heap.o
CC     := gcc
CFLAGS := -Wall -Wextra -pedantic -Werror -g

.PHONY: all clean test-list test-heap dijkstra

all: test-list test-heap 

dijkstra: $(OBJS) dijkstra.o
	$(CC) -o $@ $^

test-list: $(OBJS) test_list.o
	$(CC) -o $@ $^

test-heap: $(OBJS) test_heap.o
	$(CC) -o $@ $^

clean:
	-$(RM) test-list test-heap dijkstra $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

