CC=gcc
CFLAGS= -o
TARGETS=smsh
all: $(TARGETS)
.PHONY: all
%:
	$(CC) $(CFLAGS) $@ $@.c

clean:
	rm $(TARGETS)