OBJS   := test.o list.o
CC     := gcc
CFLAGS := -Wall -Wextra -pedantic -Werror -g

.PHONY: all clean test

all: test

test: $(OBJS)
	$(CC) -o $@ $^

clean:
	-$(RM) test $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

